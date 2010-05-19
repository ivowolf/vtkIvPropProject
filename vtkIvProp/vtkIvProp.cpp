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

vtkIvProp::vtkIvProp() : scene(NULL)
{
  renderAction = new SoVTKRenderAction(SbVec2s(1,1));
  handleEventAction = new SoHandleEventAction(SbVec2s(1,1));
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
