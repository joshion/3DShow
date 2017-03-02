#include "showwidget.h"
#include <QPainter>
#include <QPen>
#include <QDebug>

ShowWidget::ShowWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    resize(600, 400);
    triangleVertices = new QVector3D[3];
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

    QString vertStr = readStringFromFile(":/GUI/triangles.vert");
    QString fragStr = readStringFromFile(":/GUI/triangles.frag");
    program.addShaderFromSourceCode(QOpenGLShader::Vertex, vertStr);
    program.addShaderFromSourceCode(QOpenGLShader::Fragment, fragStr);
    program.link();
    program.bind();
}

void ShowWidget::paintGL()
{
    int vertexLocation = program.attributeLocation("vertex");
    int colorLocation = program.attributeLocation("color");
    int matrixLocation = program.uniformLocation("matrix");

    {
        triangleVertices[0] = QVector3D(static_cast<float>(this->width()/2),
            5.0f,
            0.0f);
        triangleVertices[1] = QVector3D(static_cast<float>((this->width()-5)), 
            static_cast<float>((this->height()-5)),
            0.0f);
        triangleVertices[2] = QVector3D(5.0f,
            static_cast<float>((this->height()-5)),
            0.0f);


    }

    {
        color[0] = QVector4D(0.0f, 0.8, 0.0f, 0.0f);
        color[1] = QVector4D(0.0f, 0.0f, 0.8f, 0.0f);
        color[2] = QVector4D(0.8f, 0.0f, 0.0f, 0.0f);
    }

    QMatrix4x4 pmvMatrix;
    pmvMatrix.ortho(this->rect());

    program.enableAttributeArray(vertexLocation);
    program.setAttributeArray(vertexLocation, triangleVertices);

    program.enableAttributeArray(colorLocation);
    program.setAttributeArray(colorLocation, color);
    program.setUniformValue(matrixLocation, pmvMatrix);

    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glFlush();

    program.disableAttributeArray(vertexLocation);
    program.disableAttributeArray(colorLocation);
}

void ShowWidget::resizeGL(int ,int )
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