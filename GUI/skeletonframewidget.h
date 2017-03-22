#ifndef SKELETONFRAMEWIDGET_H
#define SKELETONFRAMEWIDGET_H

#include "framepainter.h"
#include "imagepainter.h"

#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>


class QTimer;
class TransferSocketThread;

class SkeletonFrameWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    SkeletonFrameWidget(unsigned int port = 0, QString title = "Kinect Show", QWidget *parent = 0);
    ~SkeletonFrameWidget();

private:
    unsigned int m_uPort;
    QString m_strTitle;
    TransferSocketThread *m_pTransferSocketThread;

/*******************************************************************************************/
/*openGLœ‡πÿ*/
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int  w, int h) override;

private:
    FramePainter *m_pFramePainter;
    ImagePainter *m_pImagePainter;
    QTimer* m_pTimer;

/********************************************************************************************/

public slots:
    void slot_update();
};

#endif // SKELETONFRAMEWIDGET_H
