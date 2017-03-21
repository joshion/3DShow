#ifndef SKELETONFRAMEWIDGET_H
#define SKELETONFRAMEWIDGET_H

#include "framepainter.h"
#include "imagepainter.h"

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_3_Core>


class QTimer;
class TransferSocketThread;

class SkeletonFrameWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

public:
    SkeletonFrameWidget(QWidget *parent = 0);
    ~SkeletonFrameWidget();

/*******************************************************************************************/
/*openGLœ‡πÿ*/
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int  w, int h) override;

private:
    FramePainter *m_pFramePainter;
    ImagePainter *m_pImagePainter;
    TransferSocketThread *m_pTransferSocketThread;

    QTimer* m_pTimer;

/********************************************************************************************/

public slots:
    void slot_update();
};

#endif // SKELETONFRAMEWIDGET_H
