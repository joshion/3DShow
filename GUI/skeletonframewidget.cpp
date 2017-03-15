#include "skeletonframewidget.h"

#include "utilities.h"

#include <random>

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
    int vertexLocation = program.attributeLocation("vertex");
    int colorLocation = program.attributeLocation("color");


    QVector4D *triangleVertices = new QVector4D[6];
    QVector4D *color = new QVector4D[6];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);
    for (int i = 0; i < 2; ++i)
    {
        triangleVertices[i * 3 + 0] = QVector4D(dis(gen), dis(gen), dis(gen), dis(gen));
        triangleVertices[i * 3 + 1] = QVector4D(dis(gen), dis(gen), dis(gen), dis(gen));
        triangleVertices[i * 3 + 2] = QVector4D(dis(gen), dis(gen), dis(gen), dis(gen));
    }

    for (int i = 0; i < 2; ++i)
    {
        color[i * 3 + 0] = QVector4D(0.0f, 0.8, 0.0f, 0.0f);
        color[i * 3 + 1] = QVector4D(0.0f, 0.0f, 0.8f, 0.0f);
        color[i * 3 + 2] = QVector4D(0.8f, 0.0f, 0.0f, 0.0f);
    }


    program.enableAttributeArray(vertexLocation);
    program.setAttributeArray(vertexLocation, triangleVertices);

    program.enableAttributeArray(colorLocation);
    program.setAttributeArray(colorLocation, color);

    int matrixLocation = program.uniformLocation("matrix");
    QMatrix4x4 pmvMatrix;
    pmvMatrix.frustum(-1, 1, -1, 1, 0.3, 5.0);
    program.setUniformValue(matrixLocation, pmvMatrix);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glFlush();

    program.disableAttributeArray(vertexLocation);
    program.disableAttributeArray(colorLocation);
}

void SkeletonFrameWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, width(), height());
}

void SkeletonFrameWidget::loadTextures()
{
    QImage tex = QGLWidget::convertToGLFormat(QImage("test.jpg"));
    glGenTextures(1, m_Textures);
    glBindTexture(GL_TEXTURE_2D, m_Textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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

