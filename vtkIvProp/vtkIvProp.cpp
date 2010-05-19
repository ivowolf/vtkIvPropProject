/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2007-08-17 16:41:18 +0200 (Fr, 17 Aug 2007) $
Version:   $Revision: 11618 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include <Inventor/actions/SoHandleEventAction.h>
#include <Inventor/fields/SoField.h>
#include <Inventor/engines/SoEngine.h>

#ifdef _DEBUG
#include "windows.h" 
#include <iostream>
#include <sstream>
#include <string>



template<typename T>
std::string toString(const T& v)
{
	std::stringstream sstr;
	try 
	{
		sstr << v;	
	}
	catch ( ... )
	{
		return std::string();
	}

	return sstr.str();
	
}
#endif


#include <xip/inventor/coregl/SoXipGlowElement.h>


#include "vtkIvProp.h"

#include <vtkObjectFactory.h>
#include <vtkLODProp3D.h>
#include <vtkPropAssembly.h>
#include <vtkViewport.h>

#include "SoVTKRenderAction.h"
#include <Inventor/SbViewportRegion.h>
#include <Inventor/SoDB.h>
#include <Inventor/events/SoEvent.h>
 #include <Inventor/events/SoMouseButtonEvent.h>
 #include <Inventor/events/SoKeyboardEvent.h>
 #include <Inventor/events/SoLocation2Event.h>
#include <Inventor/fields/SoSFTime.h>
 #include <Inventor/events/SoMotion3Event.h>				 						
 #include <Inventor/events/SoMouseWheelEvent.h>
 #include <Inventor/events/SoMotion3Event.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>

#if ( ( VTK_MAJOR_VERSION < 5 ) || ( ( VTK_MAJOR_VERSION == 5 ) && (VTK_MINOR_VERSION<2)  ) )
#error VTK version >= 5.2 required
#endif

// ------------------------------------------------------------------
//
// OpenGL store/restore 

struct OpenGLState
{
	int frameBufferBinding;
	int	texUnitTarget[32];
	int	texUnitBinding[32];
	int texUnitEnabled[32];
	int fixedFuncTexEnabled[32];
	int	activeProgram;
	int viewport[4];
	int currentTexUnit;
	int matrixMode;
	float projMatrix[16];
	float mvMatrix[16];
	bool lighting;
	bool blending;
	bool depthTest;
	bool light[8];
	bool scissorTest;

//TODO: check for lighting!

};

OpenGLState oglState;

void pushOglState()
{

	oglState.blending   = glIsEnabled(GL_BLEND);
	oglState.depthTest = glIsEnabled(GL_DEPTH_TEST);
	oglState.lighting  = glIsEnabled(GL_LIGHTING);
	oglState.scissorTest = glIsEnabled(GL_SCISSOR_TEST);


	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &oglState.frameBufferBinding);
	glGetIntegerv(GL_VIEWPORT, &oglState.viewport[0]);
	glGetIntegerv(GL_ACTIVE_TEXTURE, &oglState.currentTexUnit);

	for(int i = 31; i >=0 ; --i)
	{
		oglState.texUnitTarget[i] = 0;
		oglState.texUnitBinding[i] = 0;
		oglState.texUnitEnabled[i] = 0;
		oglState.fixedFuncTexEnabled[i] = 0;

		glActiveTexture(GL_TEXTURE0 + i);

		if(glIsEnabled(GL_TEXTURE_1D))
			oglState.fixedFuncTexEnabled[i] = 1;
		
		if(glIsEnabled(GL_TEXTURE_2D))
			oglState.fixedFuncTexEnabled[i] = 1;

		if(glIsEnabled(GL_TEXTURE_3D))
			oglState.fixedFuncTexEnabled[i] = 1;



		glGetIntegerv(GL_TEXTURE_BINDING_1D, &oglState.texUnitBinding[i]);
		if(glGetError() == GL_NO_ERROR && oglState.texUnitBinding[i])
		{
			oglState.texUnitEnabled[i] = true;
			oglState.texUnitTarget[i] = GL_TEXTURE_1D;
			continue;
		}

		glGetIntegerv(GL_TEXTURE_BINDING_2D, &oglState.texUnitBinding[i]);
		if(glGetError() == GL_NO_ERROR && oglState.texUnitBinding[i])
		{
			oglState.texUnitEnabled[i] = true;
			oglState.texUnitTarget[i] = GL_TEXTURE_2D;
			continue;
		}
		
		glGetIntegerv(GL_TEXTURE_BINDING_3D, &oglState.texUnitBinding[i]);
		if(glGetError() == GL_NO_ERROR && oglState.texUnitBinding[i])
		{
			oglState.texUnitEnabled[i] = true;
			oglState.texUnitTarget[i] = GL_TEXTURE_3D;
			continue;
		}
	}

	for(int i = 0; i < 8; ++i)
	{
		oglState.light[i] = glIsEnabled(GL_LIGHT0 + i);
		if(oglState.light[i])
			glDisable(GL_LIGHT0 + i);
	}


	// get matrix state
	glGetIntegerv(GL_MATRIX_MODE, &oglState.matrixMode);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_SCISSOR_TEST);

	glAlphaFunc(GL_ALWAYS, 0.0f);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                




}

void popOglState()
{
	for(int i = 0; i < 32; ++i)
	{
		if(!oglState.texUnitEnabled[i])
			continue;

		glActiveTexture(GL_TEXTURE0 + i);
		if(oglState.fixedFuncTexEnabled[i])
			glEnable(oglState.texUnitTarget[i]);
	
		glBindTexture(oglState.texUnitTarget[i], oglState.texUnitBinding[i]);
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, oglState.frameBufferBinding);
	glViewport(oglState.viewport[0], oglState.viewport[1], oglState.viewport[2], oglState.viewport[3]);
	glActiveTexture(oglState.currentTexUnit);
	glPopAttrib();
	glPopClientAttrib();


	bool blendingEnabled = glIsEnabled(GL_BLEND);
	bool depthTestEanbled = glIsEnabled(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	glMatrixMode(oglState.matrixMode);

	if(oglState.blending)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);

	if(oglState.lighting)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	if(oglState.depthTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	if(oglState.scissorTest)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);

	for(int i = 0; i < 8; ++i)
	{
		if(oglState.light[i]) 
			glEnable(GL_LIGHT0 + i);
		else
			glDisable(GL_LIGHT0 + i);

	}




}




vtkStandardNewMacro(vtkIvProp);

vtkIvProp::vtkIvProp() : scene(NULL), Interactor(NULL)
{
  renderAction = new SoVTKRenderAction(SbVec2s(1,1));
  handleEventAction = new SoHandleEventAction(SbVec2s(1,1));
  
  this->EventCallbackCommand = vtkCallbackCommand::New();
  this->EventCallbackCommand->SetClientData(this); 
  this->EventCallbackCommand->SetCallback(vtkIvProp::ProcessVtkEvents);
}

vtkIvProp::~vtkIvProp()
{
}

double* vtkIvProp::GetBounds()
{
  static double b[6] = {0,1,0,1,0,1}; //FIXME
  return b;
}

#define RENDER_IV_MAIN_PASS
//#define RENDER_IV_OVERLAY_PASS
//#define RENDER_IV_TRANSPARENT_PASS
//#define RENDER_IV_VOLUME_PASS

int vtkIvProp::RenderOpaqueGeometry(vtkViewport* viewport)
{

#ifndef RENDER_IV_MAIN_PASS
	return 0;
#endif

  if(scene==NULL)
    return 0;

  if(!viewport->IsA("vtkOpenGLRenderer"))
		return 0;

  SoXipGLOW::init();

#ifdef _DEBUG
	GLint err = GL_NO_ERROR;
	do {
		err = glGetError();
	}
	while(err != GL_NO_ERROR); 
#endif

  pushOglState();


  int* origin = viewport->GetOrigin();
  int* size = viewport->GetSize();
  SbVec2f ivorigin = SbVec2f(origin[0],origin[1]);
  SbVec2f ivsize = SbVec2f(size[0],size[1]);
  SbViewportRegion viewportRegion;
  viewportRegion.setWindowSize(size[0], size[1]);
  viewportRegion.setViewport(ivorigin, ivsize);
  viewportRegion.setViewportPixels(origin[0], origin[1], size[0], size[1]);
  renderAction->setViewportRegion(viewportRegion);
  handleEventAction->setViewportRegion(viewportRegion);

#ifdef _DEBUG
  
  std::string test ("Opaque pass, viewport is: "); 
  test += toString(origin[0]);
  test += " ";
  test += toString(origin[1]);
  test += " ";
  test += toString(ivsize[0]);
  test += " ";
  test += toString(ivsize[1]);
  test += "\n";


  OutputDebugStringA(test.c_str());
#endif
	//const SbViewportRegion &theRegion = renderAction->getViewportRegion();
	//SbVec2s size   = theRegion.getViewportSizePixels();
	//SbVec2s origin = theRegion.getViewportOriginPixels();
	//glViewport(origin[0], origin[1], size[0], size[1]);


  renderAction->setVTKRenderPassType(SoVTKRenderAction::RenderOpaqueGeometry);
  renderAction->apply(scene);


// get all opengl errors from inventor

 #ifdef _DEBUG
  {
	std::string openGLErorString;
	GLint err = GL_NO_ERROR;
#define ADD_TO_ERROR_STRING(errCode) case errCode: if(!openGLErorString.empty()) openGLErorString += ", "; openGLErorString +=  #errCode; break;
	do {
		err = glGetError();
		switch(err)
		{
		case GL_NO_ERROR:
				break;
			default:
				openGLErorString += "Unknown OpenGL Error" ;
				break;
			ADD_TO_ERROR_STRING(GL_INVALID_ENUM);
			ADD_TO_ERROR_STRING(GL_INVALID_VALUE);
			ADD_TO_ERROR_STRING(GL_INVALID_OPERATION);
			ADD_TO_ERROR_STRING(GL_STACK_OVERFLOW);
			ADD_TO_ERROR_STRING(GL_STACK_UNDERFLOW);
			ADD_TO_ERROR_STRING(GL_OUT_OF_MEMORY);
			ADD_TO_ERROR_STRING(GL_TABLE_TOO_LARGE);
			
		}

	}
	while(err != GL_NO_ERROR); 

	if(!openGLErorString.empty())
	{
		openGLErorString += "\n";
		 OutputDebugStringA(openGLErorString.c_str());
	}
		
  }
#endif



  try 
  {
		SoDB::getSensorManager()->processTimerQueue();
  }
  catch ( ... )
  {
  }

  try 
  {
		SoDB::getSensorManager()->processDelayQueue(TRUE);
  }
  catch ( ... )
  {
		
  }


try 
	{
		SoDB::getSensorManager()->processTimerQueue();
	}
	catch ( ... )
	{
	}

	try 
	{
		SoDB::getSensorManager()->processDelayQueue(TRUE);
	}
	catch ( ... )
	{
		
	}


  popOglState();

  return 1; 

}

int vtkIvProp::RenderOverlay(vtkViewport* /*viewport*/)
{
#ifndef RENDER_IV_OVERLAY_PASS
	return 0;
#endif


  if(scene==NULL)
    return 0;

  pushOglState();

  renderAction->setVTKRenderPassType(SoVTKRenderAction::RenderOverlay);
  renderAction->apply(scene);

  popOglState();
  
  return 0;
}

void vtkIvProp::ReleaseGraphicsResources(vtkWindow* window)
{
  
}

void vtkIvProp::InitPathTraversal()
{
  //m_VtkPropRenderer->InitPathTraversal();
}

vtkAssemblyPath* vtkIvProp::GetNextPath()
{
  return NULL; //return m_VtkPropRenderer->GetNextPath();
}

int vtkIvProp::HasTranslucentPolygonalGeometry()
{
  return 0;
}

int vtkIvProp::RenderTranslucentPolygonalGeometry( vtkViewport * )
{

#ifndef RENDER_IV_TRANSPARENT_PASS
	return 0;
#endif


  if(scene==NULL)
    return 0;

  pushOglState();


  renderAction->setVTKRenderPassType(SoVTKRenderAction::RenderTranslucentPolygonalGeometry);
  renderAction->apply(scene);

  popOglState();

  return 0;
}

int vtkIvProp::RenderVolumetricGeometry( vtkViewport * )
{

#ifndef RENDER_IV_VOLUME_PASS
	return 0;
#endif


  if(scene==NULL)
    return 0;

  renderAction->setVTKRenderPassType(SoVTKRenderAction::RenderVolumetricGeometry);
  renderAction->apply(scene);
  return 0;
}

void vtkIvProp::SetSceneGraph(SoNode *newScene)
{
  // ref the new scene
  if (newScene != NULL)
    newScene->ref();

  // check if there already is a scene graph
  if (scene != NULL)
    scene->unref();

  // now set the new scene graph
  scene = newScene;
}



bool vtkIvProp::processEvent(const SoEvent *event)
//
////////////////////////////////////////////////////////////////////////
{
    if ((scene != NULL) && (handleEventAction != NULL)) {    
	handleEventAction->setEvent(event);
	handleEventAction->apply(scene);
	
	return handleEventAction->isHandled();
    }
    else
	return FALSE;
}

void vtkIvProp::SetInteractor(vtkRenderWindowInteractor* iren)
{
  if(iren!=NULL)
  {
    this->Interactor = iren;

    float priority=0.0f;
    iren->AddObserver(vtkCommand::MouseMoveEvent, this->EventCallbackCommand,
                   priority);
    iren->AddObserver(vtkCommand::LeftButtonPressEvent, this->EventCallbackCommand,
                   priority);
    iren->AddObserver(vtkCommand::LeftButtonReleaseEvent, this->EventCallbackCommand,
                   priority);
    iren->AddObserver(vtkCommand::MiddleButtonPressEvent, this->EventCallbackCommand,
                   priority);
    iren->AddObserver(vtkCommand::MiddleButtonReleaseEvent, this->EventCallbackCommand,
                   priority);
    iren->AddObserver(vtkCommand::RightButtonPressEvent, this->EventCallbackCommand,
                   priority);
    iren->AddObserver(vtkCommand::RightButtonReleaseEvent, this->EventCallbackCommand,
                   priority);
  }
}

void vtkIvProp::ProcessVtkEvents(vtkObject* vtkNotUsed(object),
                                  unsigned long event,
                                  void* clientdata,
                                  void* vtkNotUsed(calldata))
{
  vtkIvProp* self = reinterpret_cast<vtkIvProp *>( clientdata );

  SbTime time;
  time.setToTimeOfDay();

  SoEvent* e;
  SoMouseButtonEvent mbEvent;
  SoLocation2Event locEvent;

  if(event==vtkCommand::MouseMoveEvent)
  {
    e=&locEvent;
  }
  else
  {
    e=&mbEvent;

    switch (event)
    {
    default:
    case vtkCommand::LeftButtonPressEvent:
      mbEvent.setState(SoButtonEvent::DOWN);
      mbEvent.setButton(SoMouseButtonEvent::BUTTON1); break;
    case vtkCommand::LeftButtonReleaseEvent:
      mbEvent.setState(SoButtonEvent::UP);
      mbEvent.setButton(SoMouseButtonEvent::BUTTON1); break;
    case vtkCommand::MiddleButtonPressEvent:
      mbEvent.setState(SoButtonEvent::DOWN);
      mbEvent.setButton(SoMouseButtonEvent::BUTTON2); break;
    case vtkCommand::MiddleButtonReleaseEvent:
      mbEvent.setState(SoButtonEvent::UP);
      mbEvent.setButton(SoMouseButtonEvent::BUTTON2); break;
    case vtkCommand::RightButtonPressEvent:
      mbEvent.setState(SoButtonEvent::DOWN);
      mbEvent.setButton(SoMouseButtonEvent::BUTTON3); break;
    case vtkCommand::RightButtonReleaseEvent:
      mbEvent.setState(SoButtonEvent::UP);
      mbEvent.setButton(SoMouseButtonEvent::BUTTON3); break;
    }
  }

  //if (event->modifiers() & Qt::ShiftModifier)
  //  mbEvent.setShiftDown(TRUE);
  //if (event->modifiers() & Qt::ControlModifier)
  //  mbEvent.setCtrlDown(TRUE);

  int X = self->Interactor->GetEventPosition()[0];
  int Y = self->Interactor->GetEventPosition()[1];

  e->setPosition(SbVec2s(X, Y));

  self->processEvent(e);
  self->Interactor->Render();
}



 SoNode	  * vtkIvProp::getNode(const char * nodeName)
 {
	 SoNode * ret = 0;
	 if(nodeName)
		 ret = (SoNode*) SoNode::getByName(nodeName);
	 return ret;

 }

 SoEngine  * vtkIvProp::getEngine(const char * engineName)
 {
	 SoEngine * ret = 0;
	 if(engineName)
		 ret = (SoEngine*) SoEngine::getByName(engineName);
	 return ret;

 }

 SoField * vtkIvProp::getSoField(const char * fieldContainerName, const char * fieldName)
 {
	 SoFieldContainer * fc = SoNode::getByName(fieldContainerName);
	 if(!fc)
		 fc = SoEngine::getByName(fieldContainerName);
	 if(!fc)
		 return 0;

	 SoField *field = fc->getField(fieldName);
	 return field;
 }

 SoSField * vtkIvProp::getSField(const char * fieldContainerName, const char * fieldName)
 {
	SoField * field = getSoField(fieldContainerName, fieldName);
	if(field->isOfType(SoSField::getClassTypeId()))
		return (SoSField *) field;
	return 0;
 }

 SoMField * vtkIvProp::getMField(const char * fieldContainerName, const char * fieldName)
 {
	 SoField * field = getSoField(fieldContainerName, fieldName);
	 if(field->isOfType(SoMField::getClassTypeId()))
		 return (SoMField *) field;
	 return 0;
 }


 void vtkIvProp::setSField(const char * fieldContainerName, const char * fieldName, const char * value)
 {
	 if(!value || !fieldContainerName || !fieldName)
		 return;

	 SoSField *field = getSField(fieldContainerName, fieldName);
	 if (field)
	 {
		 if (strlen(value) > 0)
			 field->set(value);
		 else
			 field->touch();
	 }
 }

 void vtkIvProp::setMField(const char * fieldContainerName, const char * fieldName, const char * value, int index)
 {
	 if(!value || !fieldContainerName || !fieldName)
		 return;

	 SoMField *field = getMField(fieldContainerName, fieldName);

	if (field)
	{
		if (strlen(value) > 0)
		{
			// If this is really a MF then use the index
			if (field->isOfType(SoMField::getClassTypeId()))
			{		
				// If this is really a MF then use the index
				SoMField *mfield = (SoMField*)field;
				mfield->set1(index, value);
			}
		}
		else
		{
			field->touch();
		}
	}

 }


 void vtkIvProp::setMFieldNumElements(const char * fieldContainerName, const char * fieldName, int num)
 {
	 if( !fieldContainerName || !fieldName)
		 return;

	 SoMField *mfield = getMField(fieldContainerName, fieldName);
	 if (mfield)
			 mfield->setNum(num);
 }
