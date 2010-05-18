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

#if ( ( VTK_MAJOR_VERSION < 5 ) || ( ( VTK_MAJOR_VERSION == 5 ) && (VTK_MINOR_VERSION<2)  ) )
#error VTK version >= 5.2 required
#endif

vtkStandardNewMacro(vtkIvProp);

vtkIvProp::vtkIvProp()
{
}
vtkIvProp::~vtkIvProp()
{
}

double* vtkIvProp::GetBounds()
{
  static double b[6] = {0,1,0,1,0,1}; //FIXME
  return b;
}

int vtkIvProp::RenderOpaqueGeometry(vtkViewport* /*viewport*/)
{
  return 0; 
}

int vtkIvProp::RenderOverlay(vtkViewport* /*viewport*/)
{
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
  return 0;
}

int vtkIvProp::RenderVolumetricGeometry( vtkViewport * )
{
  return 0;
}
