#pragma once

#include <Inventor/events/SoKeyboardEvent.h>

class SoNode;
class SoSeparator;
class SoSceneManager;
class SoField;
class SoSFInt32;
class SoMouseButtonEvent;
class SoMouseWheelEvent;
class 



class IvSgWidget 
{

public:
	IvSgWidget();

	void initialize();
	bool isInitialized() const;
	void renderIv();
	void resize(int w, int h);

	// events
	void mouseButtonEvent(SoMouseButtonEvent * e);
	void mouseWheelEvent(SoMouseWheelEvent* e);

	// field getters/setters
	void setSfField(const char * nodeName, const char * fieldName, const char * value);
	void setMfField(const char * nodeName, const char * fieldName, const char * value, int index);
	void setMfFieldNumElements(const char * nodeName, const char * fieldName, int num);

	



private: 
	IvSgWidget(const IvSgWidget&);
	IvSgWidget& operator=(const IvSgWidget&);

	bool mInitialized;
	SoSceneManager *mSceneManager;
	SoNode *mSceneObject;
	SoSceneManager *mSceneManager;
	SoSeparator *mRoot;
	bool mIsSceneManagerActive;
	int mWidth;
	int mHeight;
	SoSFInt32* mInteractingField;


};


//! Open Inventor Widget which draws the Scene Graph at a given Node
/*! 
It is used in the QXipBuilderRenderViewWidget.
@todo do the documentation of this class  
*/
class QCtkXipSGWidget : public QGLWidget
{
	Q_OBJECT
public:
	QCtkXipSGWidget(QWidget *parent = 0, QGLWidget * shareWidget = 0);
	virtual ~QCtkXipSGWidget(void);

	virtual void setRoot(SoNode *node);

	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent * event);
	virtual void keyEvent(QKeyEvent *event);
	virtual void wheelEvent(QWheelEvent *event);

	virtual QGLWidget *getGLWidget() const;

	void initializeGL();



protected:
	virtual void resizeEvent(QResizeEvent * event);

	virtual void renderGL();
	virtual void resizeGL(int width, int height);

	virtual void closeEvent(QCloseEvent * event);
	virtual void processDelayQueue(bool idle = false);

	virtual void updateCursor(bool fromScene);


	// inventor
	SoField *mCursorField;

	QGLWidget *mGLWidget;
	QPoint mMouseDownPos;

	SbVec2s mOldPos;

private:
	static void renderSceneCBFunc(void *user, class SoSceneManager *);
	std::map<Qt::Key, SoKeyboardEvent::Key> mQtInventorKeyMap;
};
