#include "vtkIvPropConeDemo.h"

#include <QApplication>
#include <QMainWindow>

#include <QVTKWidget.h>

#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"

int main(int argc, char** argv)
{
  QApplication app(argc,argv);
  QMainWindow *mainWindow = new QMainWindow;
  mainWindow->setFixedSize(640,360);

  QVTKWidget *widget = new QVTKWidget;
  mainWindow->setCentralWidget(widget);

  vtkRenderer *ren1= vtkRenderer::New();
  vtkRenderWindow *renWin = vtkRenderWindow::New();
  renWin->AddRenderer( ren1 );

  widget->SetRenderWindow(renWin);

  //vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
  //iren->SetRenderWindow(renWin);

  //iren->SetInteractorStyle(NULL);

  setupConeDemo(ren1);

  mainWindow->show();
  app.aboutQt();

  return app.exec();
}
