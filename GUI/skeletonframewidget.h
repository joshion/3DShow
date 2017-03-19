#ifndef SKELETONFRAMEWIDGET_H
#define SKELETONFRAMEWIDGET_H

#include "framepainter.h"
#include "imagepainter.h"

#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_3_Core>

#include "decodevediostream.h"
#include <QFile>
#include <QByteArray>

class QTimer;
class QImage;

class SkeletonFrameWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

public:
    SkeletonFrameWidget(QWidget *parent = 0);
    ~SkeletonFrameWidget();

/*******************************************************************************************/
/*openGL相关*/
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int  w, int h) override;

private:
    FramePainter *m_pFramePainter;
    ImagePainter *m_pImagePainter;

/********************************************************************************************/

/*******************************************************************************************/
/*openCV相关*/
private:
    cv::Mat m_Mat;
    cv::VideoCapture *m_Capture;
    QTimer *m_Timer;

    DecodeVedioStream *pDecoder;
    QFile file;

/********************************************************************************************/

public slots:
    void slot_update();
};

#endif // SKELETONFRAMEWIDGET_H
