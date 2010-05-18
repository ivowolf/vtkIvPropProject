 #include "QCtkXipSGWidget.h"


 #include <QtGui/QMouseEvent>
#include <QLibrary>

 #include <Inventor/SoInteraction.h>
 #include <Inventor/SoSceneManager.h>
 #include <Inventor/nodes/SoSeparator.h>
 #include <Inventor/actions/SoActions.h>
 #include <Inventor/fields/SoSFInt32.h>
 #include <Inventor/fields/SoMFUInt32.h>
 #include <Inventor/fields/SoSFBool.h>
 #include <Inventor/events/SoMouseButtonEvent.h>
 #include <Inventor/events/SoKeyboardEvent.h>
 #include <Inventor/events/SoLocation2Event.h>
#include <Inventor/fields/SoSFTime.h>
#include <Inventor/engines/SoEngine.h>

 #include <Inventor/events/SoMotion3Event.h>				 						
 #include <Inventor/events/SoMouseWheelEvent.h>
 #include <Inventor/errors/SoErrors.h>
 #include <Inventor/fields/SoSFBool.h>
 #include <Inventor/events/SoMotion3Event.h>

 #include <QtOpenGL/QtOpenGL>

 //#include <xip/system/standard.h>


 static void renderScene(SoSceneManager *sceneManager)
 {
	 sceneManager->render();
 }


 //QCtkXipSgGLWidget::QCtkXipSgGLWidget(SoSceneManager *sceneManager, QWidget *parent, QGLWidget * shareWidget) 
 //: QGLWidget(QGLFormat(QGL::AlphaChannel|QGL::StencilBuffer|QGL::DepthBuffer|QGL::Rgba), parent, shareWidget)
 //{
	//
	// mSceneManager = sceneManager;
 //}






 // ---------------------------------------------------------------------------------------------------
 //
 //
 //

 void QCtkXipSGWidget::paintGL()
 {

	 static bool testHack = true;
	 if(testHack)
	 {
		 QStringList test; 
		 test.append("xipivcored.dll");
		 test.append("xipivcoregld.dll");
		 test.append("xipivdicomd.dll");
		 test.append("xipivoverlayd.dll");
		 test.append("xipivrendererd.dll");
		 test.append("xipivextrad.dll");

		 loadIVExtensions(test);
		 loadIvFile("../vtkIvPropProject/simpleXIP/TestSceneGraph_Opaque_Transparent_Annotations.iv");
//		 loadIvFile("scenegraphs/cone.iv");

		 testHack = false;
	 }

  renderScene(mSceneManager);
  swapBuffers();
 }

 void QCtkXipSGWidget::initializeGL()
 {

	 QGLWidget::initializeGL();

	 // init inventor 

     if(!SoDB::isInitialized())
         SoDB::init();



     mRoot = new SoSeparator;


//     SoDB::createGlobalField("realTime", SoSFTime::getClassTypeId());

	 mSceneManager = new SoSceneManager;
	 mSceneManager->setRenderCallback(renderSceneCBFunc, this);
	 mSceneManager->setBackgroundColor(SbColor(0.0, 0.0, 0.0));
	 mSceneManager->getGLRenderAction()->setTransparencyType(SoGLRenderAction::BLEND);
	 mSceneManager->setSceneGraph(mRoot);
	 mSceneManager->activate();
	 mIsSceneManagerActive = true;

	 mInteractingField = (SoSFInt32*) SoDB::getGlobalField(SbName("XipInteracting"));
	 if(!mInteractingField) {
		 mInteractingField = (SoSFInt32*) SoDB::createGlobalField(SbName("XipInteracting"), SoSFInt32::getClassTypeId());
		 mInteractingField->setValue(0);
	 }

	 glEnable(GL_DEPTH_TEST);
	 glEnable(GL_LIGHTING);
	 glEnable(GL_BLEND);



 }


 QCtkXipSGWidget::QCtkXipSGWidget(QWidget *parent, QGLWidget * shareWidget)
 : QGLWidget(QGLFormat(QGL::AlphaChannel|QGL::StencilBuffer|QGL::DepthBuffer|QGL::Rgba), parent, shareWidget)
 {

	 setAutoBufferSwap(false);
	 setMouseTracking(true);



	 mCursorField = 0;
	 mSceneObject = 0;
	 mMouseDownPos = QPoint(-1, -1);

	 mWidth = 0;
	 mHeight = 0;

	 mSceneManager = 0;
	 mRoot = 0;
	 mIsSceneManagerActive = false;

	
     mQtInventorKeyMap[Qt::Key_0] = SoKeyboardEvent::NUMBER_0;
     mQtInventorKeyMap[Qt::Key_1] = SoKeyboardEvent::NUMBER_1;
     mQtInventorKeyMap[Qt::Key_2] = SoKeyboardEvent::NUMBER_2;
     mQtInventorKeyMap[Qt::Key_3] = SoKeyboardEvent::NUMBER_3;
     mQtInventorKeyMap[Qt::Key_4] = SoKeyboardEvent::NUMBER_4;
     mQtInventorKeyMap[Qt::Key_5] = SoKeyboardEvent::NUMBER_5;
     mQtInventorKeyMap[Qt::Key_6] = SoKeyboardEvent::NUMBER_6;
     mQtInventorKeyMap[Qt::Key_7] = SoKeyboardEvent::NUMBER_7;
     mQtInventorKeyMap[Qt::Key_8] = SoKeyboardEvent::NUMBER_8;
     mQtInventorKeyMap[Qt::Key_9] = SoKeyboardEvent::NUMBER_9;
     mQtInventorKeyMap[Qt::Key_A] = SoKeyboardEvent::A;
     mQtInventorKeyMap[Qt::Key_B] = SoKeyboardEvent::B;
     mQtInventorKeyMap[Qt::Key_C] = SoKeyboardEvent::C;
     mQtInventorKeyMap[Qt::Key_D] = SoKeyboardEvent::D;
     mQtInventorKeyMap[Qt::Key_E] = SoKeyboardEvent::E;
     mQtInventorKeyMap[Qt::Key_F] = SoKeyboardEvent::F;
     mQtInventorKeyMap[Qt::Key_G] = SoKeyboardEvent::G;
     mQtInventorKeyMap[Qt::Key_H] = SoKeyboardEvent::H;
     mQtInventorKeyMap[Qt::Key_I] = SoKeyboardEvent::I;
     mQtInventorKeyMap[Qt::Key_J] = SoKeyboardEvent::J;
     mQtInventorKeyMap[Qt::Key_K] = SoKeyboardEvent::K;
     mQtInventorKeyMap[Qt::Key_L] = SoKeyboardEvent::L;
     mQtInventorKeyMap[Qt::Key_M] = SoKeyboardEvent::M;
     mQtInventorKeyMap[Qt::Key_N] = SoKeyboardEvent::N;
     mQtInventorKeyMap[Qt::Key_O] = SoKeyboardEvent::O;
     mQtInventorKeyMap[Qt::Key_P] = SoKeyboardEvent::P;
     mQtInventorKeyMap[Qt::Key_Q] = SoKeyboardEvent::Q;
     mQtInventorKeyMap[Qt::Key_R] = SoKeyboardEvent::R;
     mQtInventorKeyMap[Qt::Key_S] = SoKeyboardEvent::S;
     mQtInventorKeyMap[Qt::Key_T] = SoKeyboardEvent::T;
     mQtInventorKeyMap[Qt::Key_U] = SoKeyboardEvent::U;
     mQtInventorKeyMap[Qt::Key_V] = SoKeyboardEvent::V;
     mQtInventorKeyMap[Qt::Key_W] = SoKeyboardEvent::W;
     mQtInventorKeyMap[Qt::Key_X] = SoKeyboardEvent::X;
     mQtInventorKeyMap[Qt::Key_Y] = SoKeyboardEvent::Y;
     mQtInventorKeyMap[Qt::Key_Z] = SoKeyboardEvent::Z;
     mQtInventorKeyMap[Qt::Key_Return] = SoKeyboardEvent::RETURN;
     mQtInventorKeyMap[Qt::Key_Enter] = SoKeyboardEvent::ENTER;
     mQtInventorKeyMap[Qt::Key_Insert] = SoKeyboardEvent::INSERT;
     mQtInventorKeyMap[Qt::Key_Delete] = SoKeyboardEvent::KEY_DELETE;
     mQtInventorKeyMap[Qt::Key_Pause] = SoKeyboardEvent::PAUSE;
     mQtInventorKeyMap[Qt::Key_Print] = SoKeyboardEvent::PRINT;
     mQtInventorKeyMap[Qt::Key_Home] = SoKeyboardEvent::HOME;
     mQtInventorKeyMap[Qt::Key_End] = SoKeyboardEvent::END;
     mQtInventorKeyMap[Qt::Key_Left] = SoKeyboardEvent::LEFT_ARROW;
     mQtInventorKeyMap[Qt::Key_Up] = SoKeyboardEvent::UP_ARROW;
     mQtInventorKeyMap[Qt::Key_Right] = SoKeyboardEvent::RIGHT_ARROW;
     mQtInventorKeyMap[Qt::Key_Down] = SoKeyboardEvent::DOWN_ARROW;
     mQtInventorKeyMap[Qt::Key_Escape] = SoKeyboardEvent::ESCAPE;
     mQtInventorKeyMap[Qt::Key_Tab] = SoKeyboardEvent::TAB;
     mQtInventorKeyMap[Qt::Key_Backspace] = SoKeyboardEvent::BACKSPACE;
     mQtInventorKeyMap[Qt::Key_PageUp] = SoKeyboardEvent::PAGE_UP;
     mQtInventorKeyMap[Qt::Key_PageDown] = SoKeyboardEvent::PAGE_DOWN;
     mQtInventorKeyMap[Qt::Key_Alt] = SoKeyboardEvent::LEFT_ALT;
     mQtInventorKeyMap[Qt::Key_CapsLock] = SoKeyboardEvent::CAPS_LOCK;
     mQtInventorKeyMap[Qt::Key_NumLock] = SoKeyboardEvent::NUM_LOCK;
     mQtInventorKeyMap[Qt::Key_ScrollLock] = SoKeyboardEvent::SCROLL_LOCK;
     mQtInventorKeyMap[Qt::Key_F1] = SoKeyboardEvent::F1;
     mQtInventorKeyMap[Qt::Key_F2] = SoKeyboardEvent::F2;
     mQtInventorKeyMap[Qt::Key_F3] = SoKeyboardEvent::F3;
     mQtInventorKeyMap[Qt::Key_F4] = SoKeyboardEvent::F4;
     mQtInventorKeyMap[Qt::Key_F5] = SoKeyboardEvent::F5;
     mQtInventorKeyMap[Qt::Key_F6] = SoKeyboardEvent::F6;
     mQtInventorKeyMap[Qt::Key_F7] = SoKeyboardEvent::F7;
     mQtInventorKeyMap[Qt::Key_F8] = SoKeyboardEvent::F8;
     mQtInventorKeyMap[Qt::Key_F9] = SoKeyboardEvent::F9;
     mQtInventorKeyMap[Qt::Key_F10] = SoKeyboardEvent::F10;
     mQtInventorKeyMap[Qt::Key_F11] = SoKeyboardEvent::F11;
     mQtInventorKeyMap[Qt::Key_F12] = SoKeyboardEvent::F12;
     mQtInventorKeyMap[Qt::Key_Space] = SoKeyboardEvent::SPACE;
     mQtInventorKeyMap[Qt::Key_Apostrophe] = SoKeyboardEvent::APOSTROPHE;
     mQtInventorKeyMap[Qt::Key_Asterisk] = SoKeyboardEvent::PAD_MULTIPLY;
     mQtInventorKeyMap[Qt::Key_Plus] = SoKeyboardEvent::PAD_ADD;
     mQtInventorKeyMap[Qt::Key_Comma] = SoKeyboardEvent::COMMA;
     mQtInventorKeyMap[Qt::Key_Minus] = SoKeyboardEvent::MINUS;
     mQtInventorKeyMap[Qt::Key_Period] = SoKeyboardEvent::PERIOD;
     mQtInventorKeyMap[Qt::Key_Slash] = SoKeyboardEvent::SLASH;
     mQtInventorKeyMap[Qt::Key_Semicolon] = SoKeyboardEvent::SEMICOLON;
     mQtInventorKeyMap[Qt::Key_BracketLeft] = SoKeyboardEvent::BRACKETLEFT;
     mQtInventorKeyMap[Qt::Key_BracketRight] = SoKeyboardEvent::BRACKETRIGHT;
     mQtInventorKeyMap[Qt::Key_Backslash] = SoKeyboardEvent::BACKSLASH;
     mQtInventorKeyMap[Qt::Key_BraceRight] = SoKeyboardEvent::BRACKETRIGHT;
     mQtInventorKeyMap[Qt::Key_BraceLeft] = SoKeyboardEvent::BRACKETLEFT;
     mQtInventorKeyMap[Qt::Key_Equal] = SoKeyboardEvent::EQUAL;
     mQtInventorKeyMap[Qt::Key_division] = SoKeyboardEvent::PAD_DIVIDE;
     mQtInventorKeyMap[Qt::Key_Agrave] = SoKeyboardEvent::GRAVE;

 

 }

 QCtkXipSGWidget::~QCtkXipSGWidget(void)
 {
	 try
	 {
		 if (mSceneManager)
		 {

			 makeCurrent();
			 mRoot->removeAllChildren();
			 mSceneManager->render(FALSE, FALSE);

			 mSceneManager->deactivate();
			 mIsSceneManagerActive = false;


			 delete mSceneManager;
			 mSceneManager = 0;
		 }
	 }
	 catch(...)
	 {
	 }
 }


 void QCtkXipSGWidget::updateCursor(bool fromScene)
 {
	 if (fromScene)
	 {
		 if (!mCursorField)
		 {
			 mCursorField = SoDB::getGlobalField(SbName("MouseCursor"));
		 }

		 if (mCursorField)
		 {
			 SbString str;
			 mCursorField->get(str);

			 const char *handlePtr = strstr(str.getString(), "handle\t");
			 if (handlePtr)
			 {
				 void *handle = 0;
				 if (sscanf(handlePtr, "handle\t%p", &handle) == 1)
				 {
					 if (handle)
					 {
 #ifdef WIN32
						 setCursor(QCursor((HCURSOR) handle));
 #endif /* WIN32 */

 #ifdef linux
						 setCursor(QCursor((Qt::HANDLE) handle));
 #endif /* LINUX */

 #ifdef DARWIN
						 setCursor(QCursor()); // FIXME: We must create a handle for Mac OS X
 #endif /* DARWIN */

						 return;
					 }
				 }
			 }
		 }
	 }

	 setCursor(Qt::ArrowCursor);
 }


 void QCtkXipSGWidget::mouseReleaseEvent(QMouseEvent * event)
 {
	 if (mInteractingField)
		 mInteractingField->setValue(0);
		
	 event->accept();

	 SbTime time;
	 time.setToTimeOfDay();
	 SoMouseButtonEvent e;
	 e.setTime(time);

	 switch (event->button())
	 {
	 default:
	 case Qt::LeftButton:	e.setButton(SoMouseButtonEvent::BUTTON1); break;
	 case Qt::MidButton:		e.setButton(SoMouseButtonEvent::BUTTON2); break;
	 case Qt::RightButton:	e.setButton(SoMouseButtonEvent::BUTTON3); break;
	 }
	 if (event->modifiers() & Qt::ShiftModifier)
		 e.setShiftDown(TRUE);
	 if (event->modifiers() & Qt::ControlModifier)
		 e.setCtrlDown(TRUE);

	 e.setState(SoButtonEvent::UP);
	 e.setPosition(SbVec2s(event->pos().x(), mHeight - event->pos().y()));

	 if (mSceneManager->processEvent(&e))
	 {
		 //processDelayQueue();
		 repaint();
		 updateCursor(true);
	 }
	 else
	 {
		 updateCursor(false);
	 }
 }

 void QCtkXipSGWidget::mousePressEvent(QMouseEvent *event)
 {
	 if(mInteractingField)
		 mInteractingField->setValue(1);
		
	 // store old pos for delta mode
	 mOldPos = SbVec2s(event->pos().x(), event->pos().y());



	 event->accept();

     // Pass the QT mouse button event to open inventor
	 SbTime time;
	 time.setToTimeOfDay();
	 SoMouseButtonEvent e;
	 e.setTime(time);

	 switch (event->button())
	 {
	 default:
	 case Qt::LeftButton:	e.setButton(SoMouseButtonEvent::BUTTON1); break;
	 case Qt::MidButton:		e.setButton(SoMouseButtonEvent::BUTTON2); break;
	 case Qt::RightButton:	e.setButton(SoMouseButtonEvent::BUTTON3); break;
	 }
	 if (event->modifiers() & Qt::ShiftModifier)
		 e.setShiftDown(TRUE);
	 if (event->modifiers() & Qt::ControlModifier)
		 e.setCtrlDown(TRUE);
	 e.setState(SoButtonEvent::DOWN);
	 e.setPosition(SbVec2s(event->pos().x(), mHeight - event->pos().y()));

	 if (mSceneManager->processEvent(&e))
	 {
		 //processDelayQueue();
         repaint();
		 updateCursor(true);
	 }
	 else
	 {
		 updateCursor(false);
	 }
 }

 void QCtkXipSGWidget::mouseMoveEvent(QMouseEvent *event)
 {
	 bool onResizeBorder = false;

	 if (!event->buttons())
	 {
		 if ((event->pos().x() > (this->width() - 10)) || (event->pos().y() > (this->height() - 10)))
		 {
			 if ((event->pos().x() > (this->width() - 10)) || (event->pos().y() > (this->height() - 10)))
			 {
				 if ((event->pos().x() > (this->width() - 10)) && (event->pos().y() > (this->height() - 10)))
				 {
					 setCursor(Qt::SizeFDiagCursor);
					 onResizeBorder = true;
				 }
				 else if (event->pos().x() > (this->width() - 10))
				 {
					 setCursor(Qt::SizeHorCursor);
					 onResizeBorder = true;
				 }
				 else if (event->pos().y() > (this->height() - 10))
				 {
					 setCursor(Qt::SizeVerCursor);
					 onResizeBorder = true;
				 }
			 }
		 }
	 }

	 event->accept();

	 SbTime time;
	 time.setToTimeOfDay();

	 // get mouse delta field
	 //SoSFBool *deltaField = (SoSFBool *) SoDB::getGlobalField("MOUSE_DELTA_MODE");
	 //if ( deltaField && deltaField->getValue() )
	 //{


	 if (mSceneManager && mIsSceneManagerActive) 
	 {
		 SbTime time;
		 time.setToTimeOfDay();
		 //SoLocation2Event event;
		 //event.setTime(time);
		 //if (event->modifiers() & Qt::ShiftModifier) 
			// event.setShiftDown(TRUE);
		 //if (event->modifiers() & Qt::ControlModifier) 
			// event.setCtrlDown(TRUE);
		 SbVec2s pos = SbVec2s(event->pos().x(),event->pos().y());
		 //SbVec2s delta =  pos - mOldPos;
		 //delta[1] = -delta[1];

		 mOldPos = pos;
		 SoLocation2Event e;
		 e.setTime(time);
		 if (event->modifiers() & Qt::ShiftModifier)
			 e.setShiftDown(TRUE);
		 if (event->modifiers() & Qt::ControlModifier)
			 e.setCtrlDown(TRUE);

		 e.setPosition(SbVec2s(event->pos().x(), mHeight - event->pos().y()));
		 if (mSceneManager->processEvent(&e))
		 {
			 //processDelayQueue();
			 repaint();
			 updateCursor(true);
		 }
		 else
		 {
			 updateCursor(false);
		 }
	 }


 }

 void QCtkXipSGWidget::keyEvent(QKeyEvent *event)
 {
    
     // Pass the QtKeyEvent to open inventor
     SoKeyboardEvent e;
     SbTime time;
     time.setToTimeOfDay();
     e.setTime(time);

     if (event->modifiers() & Qt::ShiftModifier)
         e.setShiftDown(TRUE);
     if (event->modifiers() & Qt::ControlModifier)
         e.setCtrlDown(TRUE);
     if (event->modifiers() & Qt::AltModifier)
         e.setAltDown(TRUE);

     if(event->type() == QEvent::KeyPress)
         e.setState(SoButtonEvent::DOWN);
     else if(event->type() == QEvent::KeyRelease)
         e.setState(SoButtonEvent::UP);

     if(mQtInventorKeyMap.find((Qt::Key)event->key()) != mQtInventorKeyMap.end())
     {
         e.setKey(mQtInventorKeyMap[(Qt::Key)event->key()]);
     }
     if(mQtInventorKeyMap.find((Qt::Key)event->nativeVirtualKey()) != mQtInventorKeyMap.end())
     {
         e.setKey(mQtInventorKeyMap[(Qt::Key)event->nativeVirtualKey()]);
     }

     event->accept();

     if (mSceneManager->processEvent(&e))
     {
		repaint();
     }
 }

 void QCtkXipSGWidget::wheelEvent(QWheelEvent *event)
 {
 // TODO: Find a solution for Linux and Mac
 #if WIN32
     SbTime time;
     time.setToTimeOfDay();
     SoMouseWheelEvent e;
     e.setTime(time);
     if (event->modifiers() & Qt::ShiftModifier)
         e.setShiftDown(TRUE);
     if (event->modifiers() & Qt::ControlModifier)
         e.setCtrlDown(TRUE);
     e.setDelta(event->delta() / 120);
	 e.setPosition( SbVec2s(event->pos().x(), this->height() - event->pos().y() ) );
     if (mSceneManager->processEvent(&e))
     {
         //processDelayQueue();
         repaint();
         updateCursor(true);
     }
     else
     {
         updateCursor(false);
     }
 #endif
 }

 void QCtkXipSGWidget::closeEvent(QCloseEvent * event)
 {
 }

 void QCtkXipSGWidget::renderSceneCBFunc(void *user, class SoSceneManager *)
 {
	 QCtkXipSGWidget *thisPtr = (QCtkXipSGWidget*) user;
	thisPtr->repaint();
 }

 void QCtkXipSGWidget::resizeGL(int width, int height)
 {
	 mWidth = width;
	 mHeight = height;

	 glViewport(0,0,width, height);

	 if(mSceneManager)
	 {
		 mSceneManager->setWindowSize(SbVec2s(mWidth, mHeight));
		 mSceneManager->setSize(SbVec2s(mWidth, mHeight));
		 mSceneManager->setViewportRegion(SbViewportRegion(mWidth, mHeight));
	 }
 }

 void QCtkXipSGWidget::renderGL()
 {
	 renderScene(mSceneManager);
 }

 void QCtkXipSGWidget::setRoot(SoNode *node)
 {
	 if (mRoot)
	 {
		 mRoot->removeAllChildren();

		 if (node)
		 {
			 mRoot->addChild(node);
		 }
	 }
 }

 void QCtkXipSGWidget::processDelayQueue(bool idle)
 {
	 try
	 {
		 SoDB::getSensorManager()->processDelayQueue(idle ? TRUE : FALSE);
	 } 
	 catch (...) 
	 {
	 }
 }


 bool QCtkXipSGWidget::loadIVExtensions(const QStringList& extensionList)
 {


	 for(int i = 0; i < extensionList.size(); ++i)
	 {
		 QString extensionFilename = extensionList.at(i);
		 if(QLibrary::isLibrary(extensionFilename))
		 {
			 QLibrary lib(extensionFilename);
			 lib.load();



			 if(!lib.isLoaded())
			 {
				 QMessageBox msgBox;
				 msgBox.setText(QString("Failed to load iv extension ") + extensionFilename);
				 msgBox.setStandardButtons(QMessageBox::Ok );
				 msgBox.setDefaultButton(QMessageBox::Ok);
				 int ret = msgBox.exec();

				 return false;
			 }
			 else
			 {
				 typedef int (*dllInitFunc)(void);
				 dllInitFunc initFunc = (dllInitFunc) lib.resolve("xipivcoregl_init");

				 if(initFunc) 
					 initFunc();
			 }
		 }
	 }

	 return true;
	
 }

 bool QCtkXipSGWidget::loadIvFile(const QString& ivFileName)
 {
	 if(!QFile::exists(ivFileName))
	 {
		 QString pwd = QDir::currentPath();
		return false;
	 }

	 std::string ivFileNameStr(ivFileName.toStdString());
	 const char *ivStr = (const char*) ivFileNameStr.c_str();

	 SoInput in;
	 in.openFile(ivStr); // file
	 SoSeparator *topNode = SoDB::readAll(&in);
	 if (topNode)
	 {
		 if(mRoot)
		 {
			 mRoot->removeAllChildren();
			 mRoot->addChild(topNode);

			 return true;
			 
		 }
	 }
	 

	 return false;

 }




 void QCtkXipSGWidget::resizeEvent(QResizeEvent * event)
 {
	 QWidget::resizeEvent(event);
	 resizeGL(width(), height());
 }


 SoNode	  * QCtkXipSGWidget::getNode(const char * nodeName)
 {
	 SoNode * ret = 0;
	 if(nodeName)
		 ret = (SoNode*) SoNode::getByName(nodeName);
	 return ret;

 }

 SoEngine  * QCtkXipSGWidget::getEngine(const char * engineName)
 {
	 SoEngine * ret = 0;
	 if(engineName)
		 ret = (SoEngine*) SoEngine::getByName(engineName);
	 return ret;

 }

 SoField * QCtkXipSGWidget::getSoField(const char * fieldContainerName, const char * fieldName)
 {
	 SoFieldContainer * fc = SoNode::getByName(fieldContainerName);
	 if(!fc)
		 fc = SoEngine::getByName(fieldContainerName);
	 if(!fc)
		 return 0;

	 SoField *field = fc->getField(fieldName);
	 return field;
 }

 SoSField * QCtkXipSGWidget::getSField(const char * fieldContainerName, const char * fieldName)
 {
	SoField * field = getSoField(fieldContainerName, fieldName);
	if(field->isOfType(SoSField::getClassTypeId()))
		return (SoSField *) field;
	return 0;
 }

 SoMField * QCtkXipSGWidget::getMField(const char * fieldContainerName, const char * fieldName)
 {
	 SoField * field = getSoField(fieldContainerName, fieldName);
	 if(field->isOfType(SoMField::getClassTypeId()))
		 return (SoMField *) field;
	 return 0;
 }


 void QCtkXipSGWidget::setSField(const char * fieldContainerName, const char * fieldName, const char * value)
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

 void QCtkXipSGWidget::setMField(const char * fieldContainerName, const char * fieldName, const char * value, int index)
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


 void QCtkXipSGWidget::setMFieldNumElements(const char * fieldContainerName, const char * fieldName, int num)
 {
	 if( !fieldContainerName || !fieldName)
		 return;

	 SoMField *mfield = getMField(fieldContainerName, fieldName);
	 if (mfield)
			 mfield->setNum(num);
 }




