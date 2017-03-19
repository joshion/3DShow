#include "skeletonframewidget.h"

#include "utilities.h"

#include <random>

#include <QImage>
#include <QTimer>


namespace
{
    const int VERTEX_LOCATION = 1;
    static const int COLOR_LOCATION = 2;
    static const int VIEW_LOCATION = 3;
    static const int PROJECTION_LOCATION = 4;
}

SkeletonFrameWidget::SkeletonFrameWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_pFramePainter = new FramePainter;
    m_pImagePainter = new ImagePainter;

    m_Capture = new cv::VideoCapture(0);
    m_Timer = new QTimer(this);
    connect(m_Timer, &QTimer::timeout, this, &SkeletonFrameWidget::slot_update);
    m_Timer->start(50);


    pDecoder = new DecodeVedioStream;
    file.setFileName("temp.h264");
    file.open(QFile::ReadOnly);
}

SkeletonFrameWidget::~SkeletonFrameWidget()
{
    m_Timer->stop();
    delete m_Timer;
    m_Timer = nullptr;

    if (m_Capture && m_Capture->isOpened())
    {
        m_Capture->release();
    }
    delete m_Capture;
    m_Capture = nullptr;

    delete m_pImagePainter;
    delete m_pFramePainter;
}

void SkeletonFrameWidget::initializeGL()
{
    /* 0. 初始化函数，使得函数可以使用 */
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);

    m_pFramePainter->buildShaderProgram("showwidget.vert", "showwidget.frag");
    m_pImagePainter->buildShaderProgram("skeletonframewidget.vert", "skeletonframewidget.frag");
}

void SkeletonFrameWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    *m_Capture >> m_Mat;

    if (pDecoder->matsSize() > 0)
    {
        m_pImagePainter->loadTexture(pDecoder->popMats());
    }
    //m_pImagePainter->loadTexture(m_Mat);
    m_pImagePainter->paint();
    m_pFramePainter->paint();
}

void SkeletonFrameWidget::resizeGL(int w, int h)
{
    if ((float) width() / height() > 640.0 / 480)
    {
        glViewport(0, 0, height() * 640 / 480, height());
    }
    else
    {
        glViewport(0, 0, width(), width() * 480.0 / 640);
    }
}

void SkeletonFrameWidget::slot_update()
{
    this->update();
    QByteArray temp = file.read(4096);
    pDecoder->pushBytes(temp);
}

