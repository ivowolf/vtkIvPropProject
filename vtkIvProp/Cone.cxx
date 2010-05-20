/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: Cone.cxx,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//
// This example creates a polygonal model of a cone, and then renders it to
// the screen. It will rotate the cone 360 degrees and then exit. The basic
// setup of source -> mapper -> actor -> renderer -> renderwindow is 
// typical of most VTK programs.
//

// First include the required header files for the VTK classes we are using.
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkProperty.h"

#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleTrackballCamera.h"

#include "vtkIvProp.h"
#include <Inventor/nodes/SoSeparator.h>

#include <string>
#include <vector>

#include "windows.h"


bool loadIvFile(const char* ivFileName, SoSeparator* root);

bool loadIVExtensions(const std::vector<std::string>& extensions );


int main()
{
  // 
  // Next we create an instance of vtkConeSource and set some of its
  // properties. The instance of vtkConeSource "cone" is part of a
  // visualization pipeline (it is a source process object); it produces data
  // (output type is vtkPolyData) which other filters may process.
  //
  vtkConeSource *cone = vtkConeSource::New();
  cone->SetHeight( 3.0 );
  cone->SetRadius( 1.0 );
  cone->SetResolution( 10 );
  
  
  // 
  // In this example we terminate the pipeline with a mapper process object.
  // (Intermediate filters such as vtkShrinkPolyData could be inserted in
  // between the source and the mapper.)  We create an instance of
  // vtkPolyDataMapper to map the polygonal data into graphics primitives. We
  // connect the output of the cone souece to the input of this mapper.
  //
  vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();
  coneMapper->SetInputConnection( cone->GetOutputPort() );

  // 
  // Create an actor to represent the cone. The actor orchestrates rendering
  // of the mapper's graphics primitives. An actor also refers to properties
  // via a vtkProperty instance, and includes an internal transformation
  // matrix. We set this actor's mapper to be coneMapper which we created
  // above.
  //
  vtkActor *coneActor = vtkActor::New();
  coneActor->SetMapper( coneMapper );
  coneActor->GetProperty()->SetOpacity(1.0);





  if(!SoDB::isInitialized())
    SoDB::init();
  
  // load extensions

  std::vector<std::string> xipIvExtensions;
 // xipIvExtensions.push_back("xipivcommond.dll");
  xipIvExtensions.push_back("xipivcored.dll");
  xipIvExtensions.push_back("xipivcoregld.dll");

  xipIvExtensions.push_back("xipivdicomd.dll");
  xipIvExtensions.push_back("xipivoverlayd.dll");
  xipIvExtensions.push_back("xipivrendererd.dll");
//  xipIvExtensions.push_back("xipivitkd.dll");
//  xipIvExtensions.push_back("xipivvtkd.dll");
  xipIvExtensions.push_back("xipivextrad.dll");

	
		

  bool rv = loadIVExtensions(xipIvExtensions);
  if(!rv)
  {
	  std::cerr << "Extension load failed";
	  return 1;
  }
  
  
  SoSeparator* root = new SoSeparator;

  //bool ok = loadIvFile("../vtkIvPropProject/simpleXIP/xipDVR.iv", root);
  bool ok = loadIvFile("../vtkIvPropProject/simpleXIP/TestSceneGraph_Opaque_Transparent_Annotations.iv", root);
  vtkIvProp * ivProp = vtkIvProp::New();
  if(ok)
  {
    ivProp->SetSceneGraph(root);
  }

  //
  // Create the Renderer and assign actors to it. A renderer is like a
  // viewport. It is part or all of a window on the screen and it is
  // responsible for drawing the actors it has.  We also set the background
  // color here.
  //
  vtkRenderer *ren1= vtkRenderer::New();
  ren1->AddActor( coneActor );
  ren1->AddActor( ivProp );
  //ren1->SetBackground( 0.1, 0.2, 0.4 );
  ren1->SetBackground( 0.0, 0.0, 0.0);
  
  //
  // Finally we create the render window which will show up on the screen.
  // We put our renderer into the render window using AddRenderer. We also
  // set the size to be 300 pixels by 300.
  //
  vtkRenderWindow *renWin = vtkRenderWindow::New();
  renWin->AlphaBitPlanesOn();
  //renWin->SetStencilCapable(1) ;
  renWin->SetSize( 512, 512 );


  renWin->AddRenderer( ren1 );

  // 
  // The vtkRenderWindowInteractor class watches for events (e.g., keypress,
  // mouse) in the vtkRenderWindow. These events are translated into
  // event invocations that VTK understands (see VTK/Common/vtkCommand.h
  // for all events that VTK processes). Then observers of these VTK
  // events can process them as appropriate.
  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renWin);

  //
  // By default the vtkRenderWindowInteractor instantiates an instance
  // of vtkInteractorStyle. vtkInteractorStyle translates a set of events
  // it observes into operations on the camera, actors, and/or properties
  // in the vtkRenderWindow associated with the vtkRenderWinodwInteractor. 
  // Here we specify a particular interactor style.
  vtkInteractorStyleTrackballCamera *style = 
    vtkInteractorStyleTrackballCamera::New();
  iren->SetInteractorStyle(NULL);
  
  //
  // Unlike the previous scripts where we performed some operations and then
  // exited, here we leave an event loop running. The user can use the mouse
  // and keyboard to perform the operations on the scene according to the
  // current interaction style. When the user presses the "e" key, by default
  // an ExitEvent is invoked by the vtkRenderWindowInteractor which is caught
  // and drops out of the event loop (triggered by the Start() method that
  // follows.
  //
  iren->Initialize();
  ivProp->SetInteractor(iren);
  iren->Start();
  
  //
  // Free up any objects we created. All instances in VTK are deleted by
  // using the Delete() method.
  //
  cone->Delete();
  coneMapper->Delete();
  coneActor->Delete();
  ren1->Delete();
  renWin->Delete();

  return 0;
}

#include <stdio.h>
#include <string>
 #include <Inventor/SoInput.h>


bool loadIvFile(const char * ivFileName, SoSeparator* root)
{
  FILE* f = fopen(ivFileName, "r");
  if(f==NULL)
  {
    return false;
  }
  fclose(f);

  std::string ivFileNameStr(ivFileName);
  const char *ivStr = (const char*) ivFileNameStr.c_str();

  SoInput in;
  in.openFile(ivStr); // file
  SoSeparator *topNode = SoDB::readAll(&in);
  if (topNode)
  {
    if(root)
    {
      root->removeAllChildren();
      root->addChild(topNode);

      return true;

    }
  }


  return false;

}

bool loadIVExtensions(const std::vector<std::string>& extensions )
 {


	 for(int i = 0; i < extensions.size(); ++i)
	 {
		 HMODULE lib = LoadLibraryA(extensions[i].c_str());
		 if(!lib)
			 return false;
		
	 }

	 return true;
}

