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
    m_pTriangleVertices = new QVector4D[6];
    m_pColor = new QVector4D[6];
    m_pElement = new GLushort[8];
}

FramePainter::~FramePainter()
{
    m_Program.bind();
    m_Program.disableAttributeArray(VERTEX_LOCATION);
    m_Program.disableAttributeArray(COLOR_LOCATION);

    delete[] m_pTriangleVertices;
    delete[] m_pColor;
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

        glGenVertexArrays(1, m_VertexArraysObject);
        glGenBuffers(1, m_PointsBuffer);
        glGenBuffers(1, m_ElementBuffer);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    return flag;
}

void FramePainter::paint()
{
    m_Program.bind();
    loadFrame();

    glBindVertexArray(m_VertexArraysObject[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer[0]);
    glEnableVertexAttribArray(VERTEX_LOCATION);
    glEnableVertexAttribArray(COLOR_LOCATION);


    glDrawArrays(GL_POINTS, 0, 6);
    glLineWidth(3.0);
    glDrawElements(GL_LINE_STRIP, 8, GL_UNSIGNED_SHORT, nullptr);
    glFlush();

    glDisableVertexAttribArray(VERTEX_LOCATION);
    glDisableVertexAttribArray(COLOR_LOCATION);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void FramePainter::loadFrame()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);
    for (int i = 0; i < 2; ++i)
    {
        m_pTriangleVertices[i * 3 + 0] = QVector4D(dis(gen), dis(gen), -0.31, 1.0);
        m_pTriangleVertices[i * 3 + 1] = QVector4D(dis(gen), dis(gen), -0.31, 1.0);
        m_pTriangleVertices[i * 3 + 2] = QVector4D(dis(gen), dis(gen), -0.31, 1.0);
    }

    for (int i = 0; i < 2; ++i)
    {
        m_pColor[i * 3 + 0] = QVector4D(0.8, 0.0, 0.0, 1.0);
        m_pColor[i * 3 + 1] = QVector4D(0.8, 0.0, 0.0, 1.0);
        m_pColor[i * 3 + 2] = QVector4D(0.0, 0.0, 0.8, 1.0);
    }

    {
        m_pElement[0] = 0;
        m_pElement[1] = 1;
        m_pElement[2] = -1;
        m_pElement[3] = 2;
        m_pElement[4] = 3;
        m_pElement[5] = -1;
        m_pElement[6] = 4;
        m_pElement[7] = 5;
    }

    loadFrame(m_pTriangleVertices, m_pColor, 6, m_pElement, 8);
}

void FramePainter::loadFrame(QVector4D* pVertices, QVector4D* pColors, unsigned int verticesSize,
    unsigned short* pElement, unsigned int elementSize)
{
    /*
    传输绘制索引到 opengl 服务器上的缓存上
    */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBuffer[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementSize * sizeof(GLushort), pElement, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /*
    传输顶点和颜色到 opengl 服务器的缓存上
    */
    glBindVertexArray(m_VertexArraysObject[0]);
    glBindBuffer(GL_ARRAY_BUFFER, m_PointsBuffer[0]);

    glBufferData(GL_ARRAY_BUFFER, 2 * verticesSize * sizeof(QVector4D), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize * sizeof(QVector4D), pVertices);
    glBufferSubData(GL_ARRAY_BUFFER, verticesSize * sizeof(QVector4D), verticesSize * sizeof(QVector4D), pColors);

    glVertexAttribPointer(VERTEX_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glVertexAttribPointer(COLOR_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *) (verticesSize * sizeof(QVector4D)));

    glDisableVertexAttribArray(VERTEX_LOCATION);
    glDisableVertexAttribArray(COLOR_LOCATION);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
