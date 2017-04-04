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

    m_Program.setUniformValue(COLOR_LOCATION, QVector4D { 0.0, 0.0, 1.0, 1.0 });
    glDrawArrays(GL_POINTS, 0, m_pointSize);
    glLineWidth(3.0);
    m_Program.setUniformValue(COLOR_LOCATION, QVector4D { 1.0, 0.0, 0.0, 1.0 });
    glDrawElements(GL_LINE_STRIP, m_elementSize, GL_UNSIGNED_SHORT, nullptr);
    glFlush();

    m_Program.disableAttributeArray(VERTEX_LOCATION);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void FramePainter::loadFrame(const SkeletonFrame &frame)
{
    m_pointSize = frame.pointSize();
    m_elementSize = frame.elementSize();

    loadFrame(frame.pointData(), frame.pointSize(), frame.elementData(), frame.elementSize());
}

void FramePainter::loadFrame(const QVector4D* pVertices, unsigned int verticesSize,
    const short* pElement, unsigned int elementSize)
{
    m_Program.bind();

    /* 传输顶点到 opengl 服务器的缓存上 */
    glBindVertexArray(m_VertexArraysObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_PointsBuffer);
    glBufferData(GL_ARRAY_BUFFER, verticesSize * sizeof(QVector4D), pVertices, GL_STATIC_DRAW);
    m_Program.setAttributeBuffer(VERTEX_LOCATION, GL_FLOAT, 0, 4, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* 传输绘制索引到 opengl 服务器上的缓存上 */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize * sizeof(GLshort), pElement, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}