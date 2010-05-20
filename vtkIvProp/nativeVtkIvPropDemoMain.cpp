#include "vtkIvPropConeDemo.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"


int main()
{
  vtkRenderer *ren1= vtkRenderer::New();
  vtkRenderWindow *renWin = vtkRenderWindow::New();
  renWin->AddRenderer( ren1 );
  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow(renWin);

  iren->SetInteractorStyle(NULL);

  setupConeDemo(ren1);

  renWin->SetSize( 512, 512 );

  iren->Start();
  
  ren1->Delete();
  renWin->Delete();

  return 0;
}

