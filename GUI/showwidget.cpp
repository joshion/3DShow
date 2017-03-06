#include "showwidget.h"

// #include <QOpenGLContext>
#include <QFile>

ShowWidget::ShowWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    resize(600, 400);
    triangleVertices = new QVector4D[3];
    color = new QVector4D[4];
}

ShowWidget::~ShowWidget()
{

}

void ShowWidget::initializeGL()
{
    /* 0. 初始化函数，使得函数可以使用 */
    initializeOpenGLFunctions();
    glViewport(0, 0, width(), height());
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    QString vertStr = readStringFromFile("triangles.vert");
    QString fragStr = readStringFromFile("triangles.frag");
    program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertStr);
    program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragStr);
    program.link();
    program.bind();
}

void ShowWidget::paintGL()
{
    int vertexLocation = program.attributeLocation("vertex");
    int colorLocation = program.attributeLocation("color");

    {
        triangleVertices[0] = QVector4D(0, 1, -0.5, 1);
        triangleVertices[1] = QVector4D(1, -1, -0.5, 1);
        triangleVertices[2] = QVector4D(-1, -1, -0.5, 1);
    }


    {
        color[0] = QVector4D(0.0f, 0.8, 0.0f, 0.0f);
        color[1] = QVector4D(0.0f, 0.0f, 0.8f, 0.0f);
        color[2] = QVector4D(0.8f, 0.0f, 0.0f, 0.0f);
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
    glViewport(0, 0, this->width()/2, this->height()/2);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glFlush();

    program.disableAttributeArray(vertexLocation);
    program.disableAttributeArray(colorLocation);
}

void ShowWidget::resizeGL(int w, int h)
{
}

QString ShowWidget::readStringFromFile(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly))
    {
        return file.readAll();
    }
    else
    {
        return "";
    }
}