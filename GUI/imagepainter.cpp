#include "imagepainter.h"

namespace
{
    const int VERTEX_LOCATION = 1;
    static const int COORD_LOCATION = 2;
    static const int VIEW_LOCATION = 3;
    static const int PROJECTION_LOCATION = 4;
}

ImagePainter::ImagePainter()
{
    m_pTriangleVertices = new QVector4D[4];
    {
        m_pTriangleVertices[0] = QVector4D(-1.0, -1.0, -0.31, 1.0);
        m_pTriangleVertices[1] = QVector4D(1.0, -1.0, -0.31, 1.0);
        m_pTriangleVertices[2] = QVector4D(1.0, 1.0, -0.31, 1.0);
        m_pTriangleVertices[3] = QVector4D(-1.0, 1.0, -0.31, 1.0);
    }

    m_pCoord = new QVector2D[4];
    {
        m_pCoord[0] = QVector2D(0.0, 1.0);
        m_pCoord[1] = QVector2D(1.0, 1.0);
        m_pCoord[2] = QVector2D(1.0, 0.0);
        m_pCoord[3] = QVector2D(0.0, 0.0);
    }
}

ImagePainter::~ImagePainter()
{
    m_Program.bind();
    m_Program.disableAttributeArray(VERTEX_LOCATION);
    m_Program.disableAttributeArray(COORD_LOCATION);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &m_Texture);

    delete[] m_pTriangleVertices;
    delete[] m_pCoord;
}

bool ImagePainter::buildShaderProgram(const QString & strVertFile, const QString & strFragFile)
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

        glGenTextures(1, &m_Texture);
    }
    return flag;
}

void ImagePainter::paint()
{
    m_Program.bind();

    m_Program.enableAttributeArray(VERTEX_LOCATION);
    m_Program.setAttributeArray(VERTEX_LOCATION, m_pTriangleVertices);

    m_Program.enableAttributeArray(COORD_LOCATION);
    m_Program.setAttributeArray(COORD_LOCATION, m_pCoord);

    glBindTexture(GL_TEXTURE_2D, m_Texture);
    // glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glFlush();

    m_Program.disableAttributeArray(VERTEX_LOCATION);
    m_Program.disableAttributeArray(COORD_LOCATION);
}

void ImagePainter::loadTexture(cv::Mat &mat)
{
    m_Program.bind();

    //glBindTexture(GL_TEXTURE_2D, 0);
    //glDeleteTextures(1, m_Textures);
    //glGenTextures(1, m_Textures);
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    glTexStorage2D(GL_TEXTURE_2D, 2, GL_RGBA8, mat.cols, mat.rows);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mat.cols, mat.rows,
        GL_RGB, GL_UNSIGNED_BYTE, mat.data);

    static const GLint swizzles[] = { GL_BLUE, GL_GREEN, GL_RED, GL_ONE };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzles);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_2D);
}