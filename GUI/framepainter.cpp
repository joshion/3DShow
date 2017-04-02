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

        glGenVertexArrays(1, &m_VertexArraysObject);
        glGenBuffers(1, &m_PointsBuffer);
        glGenBuffers(1, &m_ElementBuffer);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    return flag;
}

void FramePainter::paint()
{
    m_Program.bind();

    glBindVertexArray(m_VertexArraysObject);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);

    m_Program.enableAttributeArray(VERTEX_LOCATION);
    m_Program.enableAttributeArray(COLOR_LOCATION);

    glDrawArrays(GL_POINTS, 0, 6);
    glLineWidth(3.0);
    glDrawElements(GL_LINE_STRIP, 8, GL_UNSIGNED_SHORT, nullptr);
    glFlush();

    m_Program.disableAttributeArray(VERTEX_LOCATION);
    m_Program.disableAttributeArray(COLOR_LOCATION);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void FramePainter::loadFrame(const SkeletonFrame& frame)
{
    QVector4D *pVertices = new QVector4D[frame.pointSize()];
    QVector4D *pColors = new QVector4D[frame.pointSize()];

    float x = 0.0;
    float y = 0.0;
    for (int i = 0; i < frame.pointSize(); ++i)
    {
        x = *(frame.pointData() + 2 * i) / frame.width();
        y = *(frame.pointData() + 2 * i + i) / frame.height();
        *(pVertices + i) = QVector4D(x, y, -3.0, 1.0);
        *(pColors + i) = QVector4D(1.0, 1.0, 0, 0);
    }

    loadFrame(pVertices, pColors, frame.pointSize() , frame.element(), frame.elementLength());
    delete[] pVertices;
    delete[] pColors;
}

void FramePainter::loadFrame(QVector4D* pVertices, QVector4D* pColors, unsigned int verticesSize,
    unsigned short* pElement, unsigned int elementSize)
{
    m_Program.bind();

    /*
    传输绘制索引到 opengl 服务器上的缓存上
    */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize * sizeof(GLushort), pElement, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /*
    传输顶点和颜色到 opengl 服务器的缓存上
    */
    glBindVertexArray(m_VertexArraysObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_PointsBuffer);

    glBufferData(GL_ARRAY_BUFFER, 2 * verticesSize * sizeof(QVector4D), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize * sizeof(QVector4D), pVertices);
    glBufferSubData(GL_ARRAY_BUFFER, verticesSize * sizeof(QVector4D), verticesSize * sizeof(QVector4D), pColors);

    m_Program.setAttributeBuffer(VERTEX_LOCATION, GL_FLOAT, 0, 4, 0);
    m_Program.setAttributeBuffer(COLOR_LOCATION, GL_FLOAT, verticesSize * sizeof(QVector4D), 4, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}