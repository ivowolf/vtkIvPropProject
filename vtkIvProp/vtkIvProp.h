/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2006-05-04 16:45:18 +0200 (Do, 04 Mai 2006) $
Version:   $Revision: 6790 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef vtkIvProp_H_HEADER_INCLUDED_C1C53723
#define vtkIvProp_H_HEADER_INCLUDED_C1C53723

#include "vtkProp.h"

class SoGLRenderAction;
class SoVTKRenderAction;
class SoNode;
class SoEvent;
class SoHandleEventAction;
class SoNode;
class SoEngine;
class SoField;
class SoSField;
class SoMField;

class /*VTK_IV_EXPORT*/ vtkIvProp : public vtkProp
{
  public:
    static vtkIvProp* New();
    vtkTypeMacro(vtkIvProp,vtkProp);

    void SetSceneGraph(SoNode *newScene);

    int RenderOpaqueGeometry(vtkViewport* viewport);
    
    int RenderOverlay(vtkViewport* viewport);

    double *GetBounds();

    void ReleaseGraphicsResources(vtkWindow* window);

    void InitPathTraversal();

    vtkAssemblyPath* GetNextPath();
 
    int HasTranslucentPolygonalGeometry();
    int RenderTranslucentPolygonalGeometry( vtkViewport *);
    int RenderVolumetricGeometry( vtkViewport *);

	bool processEvent(const SoEvent *event);

	
	void setSField(const char * fieldContainerName, const char * fieldName, const char * value);
	void setMField(const char * fieldContainerName, const char * fieldName, const char * value, int index);
	void setMFieldNumElements(const char * fieldContainerName, const char * fieldName, int num);

  protected:
 
	SoNode	 * getNode(const char * nodeName);
	SoEngine * getEngine(const char * engineName);
	SoField  * getSoField(const char * fieldContainerName, const char * fieldName);
	SoSField * getSField(const char * fieldContainerName, const char * fieldName);
	SoMField * getMField(const char * fieldContainerName, const char * fieldName);
    
    vtkIvProp();
    virtual ~vtkIvProp();

    SoVTKRenderAction* renderAction;
    SoNode* scene;

    SoHandleEventAction	*handleEventAction;

};


#endif /* vtkIvProp_H_HEADER_INCLUDED_C1C53723 */


