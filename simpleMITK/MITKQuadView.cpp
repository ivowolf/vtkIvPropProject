//orgi_ven.pic.gz Gallenblase.vtp Liver.vtp
//in C:\home\data\Liver\Szene
#include <mitkStandaloneDataStorage.h>
#include <mitkDataNodeFactory.h>
#include <mitkGlobalInteraction.h>
#include <QApplication>
#include <QmitkRegisterClasses.h>
#include <QmitkStdMultiWidget.h>

#include <itksys/SystemTools.hxx>

int main(int argc, char* argv[])
{
  QApplication qtapplication( argc, argv );
  if(argc<2) {
    fprintf( stderr, "Usage:   %s [filename1] [filename2] ...\n\n", itksys::SystemTools::GetFilenameName(argv[0]).c_str() );
    return 1;
  }

  // Register Qmitk-dependent global instances
  QmitkRegisterClasses();

  mitk::StandaloneDataStorage::Pointer m_DataStorage = mitk::StandaloneDataStorage::New();

  mitk::DataNodeFactory::Pointer nodeReader =
    mitk::DataNodeFactory::New();
  int i;
  for (i = 1; i < argc; ++i)
  {
    const char * filename = argv[i];
    try
    {
      nodeReader->SetFileName(filename);
      nodeReader->Update();
      mitk::DataNode::Pointer node = nodeReader->GetOutput();
      m_DataStorage->Add(node);
    }
    catch (...)
    {
      fprintf(stderr, "Could not open file %s \n\n", filename);
      exit(2);
    }
  }

  QmitkStdMultiWidget* multiWidget = new QmitkStdMultiWidget(NULL);
  // Tell the multiWidget which DataStorage to render
  multiWidget->SetDataStorage(m_DataStorage);

  mitk::TimeSlicedGeometry::Pointer geo = m_DataStorage->ComputeBoundingGeometry3D(
    m_DataStorage->GetAll());
  mitk::RenderingManager::GetInstance()->InitializeViews(geo);

  // Initialize bottom-right view as 3D view
  multiWidget->GetRenderWindow4()->GetRenderer()->SetMapperID(
    mitk::BaseRenderer::Standard3D);

  // Enable standard handler for levelwindow-slider
  multiWidget->EnableStandardLevelWindow();

  // Add the displayed views to the DataStorage to see their positions in 2D and 3D
  multiWidget->AddDisplayPlaneSubTree();
  multiWidget->AddPlanesToDataStorage();
  multiWidget->SetWidgetPlanesVisibility(true);

  //*************************************************************************
  // Part II: Setup standard interaction with the mouse
  //*************************************************************************

  // Moving the cut-planes to click-point
  multiWidget->EnableNavigationControllerEventListening();

  // Zooming and panning
  mitk::GlobalInteraction::GetInstance()->AddListener(
    multiWidget->GetMoveAndZoomInteractor());


  multiWidget->show();

  return qtapplication.exec();
}
