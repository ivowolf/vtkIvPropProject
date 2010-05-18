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

#include "vtkIvProp.h"

#include <vtkObjectFactory.h>
#include <vtkLODProp3D.h>
#include <vtkPropAssembly.h>
#include <vtkViewport.h>

#include "SoVTKRenderAction.h"
#include <gl/gl.h>
#include <Inventor/SbViewportRegion.h>

#if ( ( VTK_MAJOR_VERSION < 5 ) || ( ( VTK_MAJOR_VERSION == 5 ) && (VTK_MINOR_VERSION<2)  ) )
#error VTK version >= 5.2 required
#endif

vtkStandardNewMacro(vtkIvProp);

vtkIvProp::vtkIvProp() : scene(NULL)
{
  renderAction = new SoGLRenderAction(SbVec2s(1,1));
}

vtkIvProp::~vtkIvProp()
{
}

double* vtkIvProp::GetBounds()
{
  static double b[6] = {0,1,0,1,0,1}; //FIXME
  return b;
}

int vtkIvProp::RenderOpaqueGeometry(vtkViewport* viewport)
{
  if(scene==NULL)
    return 0;

  int* origin = viewport->GetOrigin();
  int* size = viewport->GetSize();
  SbVec2f ivorigin = SbVec2f(origin[0],origin[1]);
  SbVec2f ivsize = SbVec2f(size[0],size[1]);
  SbViewportRegion viewportRegion;
  viewportRegion.setViewport(ivorigin, ivsize);
  renderAction->setViewportRegion(viewportRegion);

	//const SbViewportRegion &theRegion = renderAction->getViewportRegion();
	//SbVec2s size   = theRegion.getViewportSizePixels();
	//SbVec2s origin = theRegion.getViewportOriginPixels();
	//glViewport(origin[0], origin[1], size[0], size[1]);

  //renderAction->setVTKRenderPassType(SoVTKRenderAction::RenderOpaqueGeometry);
  renderAction->apply(scene);
  return 1; 
}

int vtkIvProp::RenderOverlay(vtkViewport* /*viewport*/)
{
  if(scene==NULL)
    return 0;

  //renderAction->setVTKRenderPassType(SoVTKRenderAction::RenderOverlay);
  //renderAction->apply(scene);
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
  if(scene==NULL)
    return 0;

  //renderAction->setVTKRenderPassType(SoVTKRenderAction::RenderTranslucentPolygonalGeometry);
  //renderAction->apply(scene);
  return 0;
}

int vtkIvProp::RenderVolumetricGeometry( vtkViewport * )
{
  if(scene==NULL)
    return 0;

  //renderAction->setVTKRenderPassType(SoVTKRenderAction::RenderVolumetricGeometry);
  //renderAction->apply(scene);
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
