#pragma once

#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/qglframebufferobject.h>
#include <Qt/qnamespace.h>
#include <Inventor/events/SoKeyboardEvent.h>

class SoNode;
class SoEngine;
class SoMField;
class SoSField;
class SoSeparator;
class SoSceneManager;
class SoField;
class SoSFInt32;



class SbXipPerformanceTimer
{
public:
  SbXipPerformanceTimer()
  {
    reset();

#ifdef WIN32
    QueryPerformanceFrequency((LARGE_INTEGER*) &mFreq);
#endif
  }

  void reset()
  {
#ifdef WIN32
    QueryPerformanceCounter((LARGE_INTEGER*) &mStartTime);
#endif
  }

  // returns the number of microseconds elapsed
  unsigned int elapsed()
  {
    unsigned int r = 0;
    quint64 stop;

#ifdef WIN32
    QueryPerformanceCounter((LARGE_INTEGER*) &stop);
    r = ((stop - mStartTime) * 1000000) / mFreq;
#endif

    return r;
  };

protected:
  quint64 mStartTime;
  quint64 mFreq;
};


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



  SoNode      * getNode(const char * nodeName);
  SoEngine  * getEngine(const char * engineName);
  SoField * getSoField(const char * fieldContainerName, const char * fieldName);
  SoSField * getSField(const char * fieldContainerName, const char * fieldName);
  SoMField * getMField(const char * fieldContainerName, const char * fieldName);


  void setSField(const char * nodeName, const char * fieldName, const char * value);
  void setMField(const char * nodeName, const char * fieldName, const char * value, int index);
  void setMFieldNumElements(const char * nodeName, const char * fieldName, int num);


  void paintGL();
  void initializeGL();


  bool loadIVExtensions(const QStringList& extensionList);
  bool loadIvFile(const QString& ivFileName);

  public slots:
    void doIdleProcessing();

protected:
  virtual void resizeEvent(QResizeEvent * event);

  virtual void renderGL();
  virtual void resizeGL(int width, int height);

  virtual void closeEvent(QCloseEvent * event);
  virtual void processDelayQueue(bool idle = false);

  virtual void updateCursor(bool fromScene);


  // inventor
  SoNode *mSceneObject;
  SoSceneManager *mSceneManager;
  SoSeparator *mRoot;
  bool mIsSceneManagerActive;
  int mWidth;
  int mHeight;
  SoField *mCursorField;
  SoSFInt32* mInteractingField;

  QPoint mMouseDownPos;
  SbVec2s mOldPos;

private:
  static void renderSceneCBFunc(void *user, class SoSceneManager *);
  std::map<Qt::Key, SoKeyboardEvent::Key> mQtInventorKeyMap;

  SbXipPerformanceTimer mTimeSinceLastRenderPass;

  //! Idle Timer, the Open Inventor Scene Graph needs from time to time little update for itself. The Timer calls an idle function for it.
  QTimer *mIdleTimer;
};
