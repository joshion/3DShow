#include "showwidget.h"

#include "utilities.h"

#include <random>

ShowWidget::ShowWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    resize(600, 400);
    triangleVertices = new QVector4D[6];
    color = new QVector4D[6];
    m_pTimer = new QTimer;
    connect(m_pTimer, &QTimer::timeout, this, &ShowWidget::slot_update);
    m_pTimer->start(50);
}

ShowWidget::~ShowWidget()
{

}

void ShowWidget::initializeGL()
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

void ShowWidget::paintGL()
{
    int vertexLocation = program.attributeLocation("vertex");
    int colorLocation = program.attributeLocation("color");

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);
    for(int i = 0; i < 2; ++i)
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

void ShowWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, width(), height());
}

void ShowWidget::slot_update()
{
    this->update();
}
