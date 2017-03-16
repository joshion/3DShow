#include "skeletonframewidget.h"

#include "utilities.h"

#include <random>

#include <QGLWidget>
#include <QImage>
#include <QTimer>

namespace
{
    static const int VERTEX_LOCATION = 1;
    static const int COLOR_LOCATION = 2;
    static const int VIEW_LOCATION = 3;
    static const int PROJECTION_LOCATION = 4;
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


    buildRGBProgram();
    buildSkeletonProgram();
}

void SkeletonFrameWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    paintRGB();
    paintFrame();
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

void SkeletonFrameWidget::buildRGBProgram()
{
    QString vertStr = Utilities::readStringFromFile("skeletonframewidget.vert");
    QString fragStr = Utilities::readStringFromFile("skeletonframewidget.frag");
    RGBprogram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertStr);
    RGBprogram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragStr);
    RGBprogram.link();
}

void SkeletonFrameWidget::paintRGB()
{
    RGBprogram.bind();
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

    RGBprogram.enableAttributeArray(vertexLocation);
    RGBprogram.setAttributeArray(vertexLocation, triangleVertices);

    RGBprogram.enableAttributeArray(colorLocation);
    RGBprogram.setAttributeArray(colorLocation, coord);

    QMatrix4x4 view_Matrix;
    view_Matrix.lookAt(QVector3D(0.0, 0.0, 0.0), QVector3D(0, 0, -1.0), QVector3D(0, 1, 0));
    RGBprogram.setUniformValue(VIEW_LOCATION, view_Matrix);

    QMatrix4x4 projection_Matrix;
    // projection_Matrix.frustum(-1, 1, -1, 1, 0.3, 5.0);   // 透视投影
    projection_Matrix.ortho(-1, 1, -1, 1, 0.3, 5);    // 正交投影
    RGBprogram.setUniformValue(PROJECTION_LOCATION, projection_Matrix);

    glBindTexture(GL_TEXTURE_2D, m_Textures[0]);
    // glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glFlush();

    RGBprogram.disableAttributeArray(vertexLocation);
    RGBprogram.disableAttributeArray(colorLocation);
}

void SkeletonFrameWidget::buildSkeletonProgram()
{

    QString vertStr = Utilities::readStringFromFile("showwidget.vert");
    QString fragStr = Utilities::readStringFromFile("showwidget.frag");
    skeletonProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertStr);
    skeletonProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragStr);
    skeletonProgram.link();
}

void SkeletonFrameWidget::paintFrame()
{
    skeletonProgram.bind();
    int vertexLocation = 1;
    int colorLocation = 2;

    QVector4D *triangleVertices = new QVector4D[6];
    QVector4D *color = new QVector4D[6];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);
    for (int i = 0; i < 2; ++i)
    {
        triangleVertices[i * 3 + 0] = QVector4D(dis(gen), dis(gen), -0.31, 1.0);
        triangleVertices[i * 3 + 1] = QVector4D(dis(gen), dis(gen), -0.31, 1.0);
        triangleVertices[i * 3 + 2] = QVector4D(dis(gen), dis(gen), -0.31, 1.0);
    }

    for (int i = 0; i < 2; ++i)
    {
        color[i * 3 + 0] = QVector4D(0.0f, 0.8, 0.0f, 0.0f);
        color[i * 3 + 1] = QVector4D(0.0f, 0.0f, 0.8f, 0.0f);
        color[i * 3 + 2] = QVector4D(0.8f, 0.0f, 0.0f, 0.0f);
    }


    skeletonProgram.enableAttributeArray(vertexLocation);
    skeletonProgram.setAttributeArray(vertexLocation, triangleVertices);

    skeletonProgram.enableAttributeArray(colorLocation);
    skeletonProgram.setAttributeArray(colorLocation, color);

    int matrixLocation = 3;
    QMatrix4x4 pmvMatrix;
    pmvMatrix.frustum(-1, 1, -1, 1, 0.3, 5.0);
    skeletonProgram.setUniformValue(matrixLocation, pmvMatrix);

    // glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glFlush();

    skeletonProgram.disableAttributeArray(vertexLocation);
    skeletonProgram.disableAttributeArray(colorLocation);
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

