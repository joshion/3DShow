#include "skeletonframewidget.h"

#include "utilities.h"

#include <QGLWidget>
#include <QImage>
#include <QTimer>


SkeletonFrameWidget::SkeletonFrameWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{

    m_Capture = new cv::VideoCapture(0);
    m_Timer = new QTimer(this);
    connect(m_Timer, &QTimer::timeout, this, &SkeletonFrameWidget::slot_update);
    m_Timer->start(50);
}

SkeletonFrameWidget::~SkeletonFrameWidget()
{
    m_Timer->stop();
    delete m_Timer;
    m_Timer = nullptr;
    if (m_Capture->isOpened())
    {
        m_Capture->release();
    }
    delete m_Capture;
    m_Capture = nullptr;
}

void SkeletonFrameWidget::initializeGL()
{
    /* 0. 初始化函数，使得函数可以使用 */
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    QString vertStr = Utilities::readStringFromFile("showwidget.vert");
    QString fragStr = Utilities::readStringFromFile("showwidget.frag");
    program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertStr);
    program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragStr);
    program.link();
    program.bind();
}

void SkeletonFrameWidget::paintGL()
{
}

void SkeletonFrameWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, width(), height());
}

QImage SkeletonFrameWidget::mat2QImage(cv::Mat &mat)
{
    cvtColor(mat, mat, CV_BGR2RGB);
    return QImage((const unsigned char*) mat.data, mat.cols, mat.rows, mat.cols * mat.channels(), QImage::Format_RGB888);
}

QImage SkeletonFrameWidget::mat2GLFormat(cv::Mat & mat)
{
    return QGLWidget::convertToGLFormat(mat2QImage(mat));
}

void SkeletonFrameWidget::slot_update()
{
    this->update();
}

