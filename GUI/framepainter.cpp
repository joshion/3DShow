#include "framepainter.h"

#include <random>

namespace
{
    const int VERTEX_LOCATION = 1;
    static const int COLOR_LOCATION = 2;
    static const int VIEW_LOCATION = 3;
    static const int PROJECTION_LOCATION = 4;
}

FramePainter::FramePainter()
{
}

FramePainter::~FramePainter()
{
    m_Program.bind();
    m_Program.disableAttributeArray(VERTEX_LOCATION);
    m_Program.disableAttributeArray(COLOR_LOCATION);
}

bool FramePainter::buildShaderProgram(const QString & strVertFile, const QString & strFragFile)
{
    bool flag = Painter::buildShaderProgram(strVertFile, strFragFile);
    if (flag)
    {
        m_Program.bind();
        QMatrix4x4 view_Matrix;
        view_Matrix.lookAt(QVector3D(0.0, 0.0, 0.0), QVector3D(0, 0, -1.0), QVector3D(0, 1, 0));
        m_Program.setUniformValue(VIEW_LOCATION, view_Matrix);

        QMatrix4x4 projection_Matrix;
        // projection_Matrix.frustum(-1, 1, -1, 1, 0.3, 5.0);   // 透视投影
        projection_Matrix.ortho(-1, 1, -1, 1, 0.30, 5);    // 正交投影
        m_Program.setUniformValue(PROJECTION_LOCATION, projection_Matrix);
    }
    return flag;
}

void FramePainter::paint()
{
    m_Program.bind();

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


    m_Program.enableAttributeArray(VERTEX_LOCATION);
    m_Program.setAttributeArray(VERTEX_LOCATION, triangleVertices);

    m_Program.enableAttributeArray(COLOR_LOCATION);
    m_Program.setAttributeArray(COLOR_LOCATION, color);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glFlush();

    m_Program.disableAttributeArray(VERTEX_LOCATION);
    m_Program.disableAttributeArray(COLOR_LOCATION);
}
