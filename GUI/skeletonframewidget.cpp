#include "skeletonframewidget.h"

#include "utilities.h"

#include <random>

#include <QGLWidget>
#include <QImage>
#include <QTimer>

namespace
{
    static const unsigned int VERTEX_LOCATION = 1;
    static const unsigned int COLOR_LOCATION = 2;
    static const unsigned int MATRIX_LOCATION = 3;
}


SkeletonFrameWidget::SkeletonFrameWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_Capture = nullptr;
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
    if (m_Capture && m_Capture->isOpened())
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
    QString vertStr = Utilities::readStringFromFile("skeletonframewidget.vert");
    QString fragStr = Utilities::readStringFromFile("skeletonframewidget.frag");
    program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertStr);
    program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragStr);
    program.link();
    program.bind();
}

void SkeletonFrameWidget::paintGL()
{
    int vertexLocation = VERTEX_LOCATION;
    int colorLocation = COLOR_LOCATION;


    loadTextures();

    QVector4D *triangleVertices;
    QVector4D *color;
    triangleVertices = new QVector4D[4];
    color = new QVector4D[4];

    {
        triangleVertices[0] = QVector4D(-1.0, -1.0, -0.31, 1.0);
        triangleVertices[1] = QVector4D(1.0, -1.0, -0.31, 1.0);
        triangleVertices[2] = QVector4D(1.0, 1.0, -0.31, 1.0);
        triangleVertices[3] = QVector4D(-1.0, 1.0, -0.31, 1.0);
    }
    
    /*
    {
        color[0] = QVector4D(0.0f, 0.8, 0.0f, 0.0f);
        color[1] = QVector4D(0.0f, 0.0f, 0.8f, 0.0f);
        color[2] = QVector4D(0.8f, 0.0f, 0.0f, 0.0f);
        color[3] = QVector4D(0.8f, 0.0f, 0.0f, 0.0f);
    }
    */

    QVector2D *coord = new QVector2D[4];

    {
        coord[0] = QVector2D(0.0, 1.0);
        coord[1] = QVector2D(1.0, 1.0);
        coord[2] = QVector2D(1.0, 0.0);
        coord[3] = QVector2D(0.0, 0.0);
    }

    program.enableAttributeArray(vertexLocation);
    program.setAttributeArray(vertexLocation, triangleVertices);

    program.enableAttributeArray(colorLocation);
    program.setAttributeArray(colorLocation, coord);

    int matrixLocation = MATRIX_LOCATION;
    QMatrix4x4 pmvMatrix;
    pmvMatrix.frustum(-1, 1, -1, 1, 0.3, 5.0);
    program.setUniformValue(matrixLocation, pmvMatrix);

    glBindTexture(GL_TEXTURE_2D, m_Textures[0]);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, m_Textures);
    glGenTextures(1, m_Textures);
    glBindTexture(GL_TEXTURE_2D, m_Textures[0]);

    glTexStorage2D(GL_TEXTURE_2D, 2, GL_RGBA8, m_Mat.cols, m_Mat.rows);

    *m_Capture >> m_Mat;

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Mat.cols, m_Mat.rows,
        GL_RGB, GL_UNSIGNED_BYTE, m_Mat.data);

    static const GLint swizzles[] = { GL_BLUE, GL_GREEN, GL_RED, GL_ONE };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzles);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_2D);
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

