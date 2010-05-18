#include "IvSGWidget.h"



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

#include <Inventor/events/SoMotion3Event.h>				 						
#include <Inventor/events/SoMouseWheelEvent.h>
#include <Inventor/errors/SoErrors.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/events/SoMotion3Event.h>


//#include <xip/system/standard.h>

IvSgWidget::IvSgWidget()
: mInitialized(false)
, mSceneManager(0)
, mSceneObject(0)
, mSceneManager(0)
, mRoot(0)
, mIsSceneManagerActive(false)
, mWidth(0)
, mHeight(0)
{}



void IvSgWidget::setSfField(const char * nodeName, const char * fieldName, const char * value)
{
	if(!value || !nodeName || !fieldName)
		return;

	SoFieldContainer *node = SoNode::getByName(nodeName);
	// maybe an engine?
	if (!node)
		node = SoEngine::getByName(nodeName);

	if (node)
	{
		SoField *field = node->getField(fieldName);
		if (field)
		{
			if (strlen(value) > 0)
				field->set(value);
			else
				field->touch();
		}
	}
}

void IvSgWidget::setMfField(const char * nodeName, const char * fieldName, const char * value, int index)
{
	if(!value || !nodeName || !fieldName)
		return;

	SoFieldContainer *node = SoNode::getByName(nodeName);

	// maybe an engine?
	if (!node)
		node = SoEngine::getByName(nodeName);

	if (node)
	{
		SoField *field = node->getField(fieldName);
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
				// If it's just a SF ignore the index
				else if (field->isOfType(SoSField::getClassTypeId()))
				{
					field->set(value);
				}					
			}
			else
			{
				field->touch();
			}
		}
	}
}


void IvSgWidget::setMfFieldNumElements(const char * nodeName, const char * fieldName, int num)
{
	if(num < 0 || !nodeName || !fieldName)
		return;

	SoFieldContainer *node = SoNode::getByName(nodeName);
	// maybe an engine?
	if (!node)
		node = SoEngine::getByName(nodeName);

	if (node)
	{
		SoField *field = node->getField(fieldName);
		if (field)
		{
			// Is this a MF?
			if (field->isOfType(SoMField::getClassTypeId()))
			{			
				SoMField *mfield = (SoMField*)field;
				mfield->setNum(num);
			}
		}

	}
}

void IvSgWidget::initialize()
{
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

	mInitialized = true;
}

void IvSgWidget::mouseButtonEvent(SoMouseButtonEvent * e)
{

	if (e->isButtonReleaseEvent && mInteractingField)
		mInteractingField->setValue(0);
	else
		mInteractingField->setValue(1);
}

void IvSgWidget::mouseWheelEvent(SoMouseWheelEvent* e)
{


}

void IvSgWidget::upCursor(bool fromScene)
{

}

// ---------------------------------------------------------------------------------------------------
//
//
//




QCtkXipSGWidget::QCtkXipSGWidget(QWidget *parent, QGLWidget * shareWidget)
: QGLWidget(QGLFormat(QGL::AlphaChannel|QGL::StencilBuffer|QGL::DepthBuffer|QGL::Rgba), parent)
{

	mCursorField = 0;
	mSceneObject = 0;
	mMouseDownPos = QPoint(-1, -1);

	mWidth = 0;
	mHeight = 0;

	mSceneManager = 0;
	mRoot = 0;
	mIsSceneManagerActive = false;

	setMouseTracking(true);

	//mGLWidget = new QCtkXipSgGLWidget(mSceneManager, this, shareWidget);
	//mGLWidget->show();
	//mGLWidget->setMouseTracking(true);

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
	//Fix-me: No idea what PRIOR and NEXT are
	//mQtInventorKeyMap[Qt::??] = SoKeyboardEvent::PRIOR;
	//mQtInventorKeyMap[Qt::??] = SoKeyboardEvent::NEXT;



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
		// mGLWidget->repaint();
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
		// mGLWidget->repaint();
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
		//         mGLWidget->repaint();
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
		// mGLWidget->repaint();
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
	// thisPtr->mGLWidget->repaint();
}

void QCtkXipSGWidget::resizeGL(int width, int height)
{
	mWidth = width;
	mHeight = height;

	// mGLWidget->resize(width, height);

	mSceneManager->setWindowSize(SbVec2s(mWidth, mHeight));
	mSceneManager->setSize(SbVec2s(mWidth, mHeight));
	mSceneManager->setViewportRegion(SbViewportRegion(mWidth, mHeight));
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



void QCtkXipSGWidget::resizeEvent(QResizeEvent * event)
{
	QWidget::resizeEvent(event);
	//	 resizeGL(width(), height());
}


QGLWidget *QCtkXipSGWidget::getGLWidget() const
{
	return mGLWidget;
}


