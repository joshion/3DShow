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
        glBindVertexArray(m_VertexArraysObject[0]);
        glGenBuffers(1, m_PointsBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_PointsBuffer[0]);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }
    return flag;
}

void FramePainter::paint()
{
    m_Program.bind();
    loadFrame();

    glEnableVertexAttribArray(VERTEX_LOCATION);
    glEnableVertexAttribArray(COLOR_LOCATION);

    glLineWidth(6.0);
    glDrawArrays(GL_LINE_LOOP, 0, 3);
    glFlush();

    glDisableVertexAttribArray(VERTEX_LOCATION);
    glDisableVertexAttribArray(COLOR_LOCATION);
}

void FramePainter::loadFrame()
{
    // m_Program.enableAttributeArray(VERTEX_LOCATION);
    // m_Program.enableAttributeArray(COLOR_LOCATION);

    glBindVertexArray(m_VertexArraysObject[0]);
    glBindBuffer(GL_ARRAY_BUFFER, m_PointsBuffer[0]);

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
        m_pColor[i * 3 + 2] = QVector4D(0.8, 0.0, 0.0, 1.0);
    }

    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(QVector4D), nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(QVector4D), m_pTriangleVertices);
    glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(QVector4D), 6 * sizeof(QVector4D), m_pColor);

    glVertexAttribPointer(VERTEX_LOCATION, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glVertexAttribPointer(COLOR_LOCATION, 4, GL_FLOAT, GL_FALSE,
        sizeof(m_pTriangleVertices), (const GLvoid *) (6 * sizeof(QVector4D)));

    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);
    //m_Program.disableAttributeArray(VERTEX_LOCATION);
    //m_Program.disableAttributeArray(COLOR_LOCATION);
}
