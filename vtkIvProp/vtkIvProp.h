#ifndef vtkIvProp_H_HEADER_INCLUDED_C1C53723
#define vtkIvProp_H_HEADER_INCLUDED_C1C53723

#include "vtkProp.h"

class SoGLRenderAction;
class SoVTKRenderAction;
class SoNode;
class SoEvent;
class SoHandleEventAction;
class vtkRenderWindowInteractor;
class vtkCallbackCommand;
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

  virtual void SetInteractor(vtkRenderWindowInteractor* iren);    
  void setSField(const char * fieldContainerName, const char * fieldName, const char * value);
  void setMField(const char * fieldContainerName, const char * fieldName, const char * value, int index);
  void setMFieldNumElements(const char * fieldContainerName, const char * fieldName, int num);

protected:

  SoNode     * getNode(const char * nodeName);
  SoEngine * getEngine(const char * engineName);
  SoField  * getSoField(const char * fieldContainerName, const char * fieldName);
  SoSField * getSField(const char * fieldContainerName, const char * fieldName);
  SoMField * getMField(const char * fieldContainerName, const char * fieldName);

  vtkIvProp();
  virtual ~vtkIvProp();

  SoVTKRenderAction* renderAction;
  SoNode* scene;

  vtkRenderWindowInteractor* Interactor;
  SoHandleEventAction    *handleEventAction;
  vtkCallbackCommand* EventCallbackCommand;

  static void ProcessVtkEvents(vtkObject* object,
    unsigned long event,
    void* clientdata,
    void* calldata);
  int TimerId; //keep track of the timers that are created/destroyed
};


#endif /* vtkIvProp_H_HEADER_INCLUDED_C1C53723 */
