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

}

ImagePainter::~ImagePainter()
{

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
    }
    return flag;
}

void ImagePainter::paint()
{
    m_Program.bind();

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

    QVector2D *coord = new QVector2D[4];
    {
        coord[0] = QVector2D(0.0, 1.0);
        coord[1] = QVector2D(1.0, 1.0);
        coord[2] = QVector2D(1.0, 0.0);
        coord[3] = QVector2D(0.0, 0.0);
    }

    m_Program.enableAttributeArray(VERTEX_LOCATION);
    m_Program.setAttributeArray(VERTEX_LOCATION, triangleVertices);

    m_Program.enableAttributeArray(COORD_LOCATION);
    m_Program.setAttributeArray(COORD_LOCATION, coord);

    glBindTexture(GL_TEXTURE_2D, m_Textures[0]);
    // glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glFlush();

    m_Program.disableAttributeArray(VERTEX_LOCATION);
    m_Program.disableAttributeArray(COORD_LOCATION);
}

void ImagePainter::loadTexture(cv::Mat &mat)
{
    m_Program.bind();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, m_Textures);
    glGenTextures(1, m_Textures);
    glBindTexture(GL_TEXTURE_2D, m_Textures[0]);

    glTexStorage2D(GL_TEXTURE_2D, 2, GL_RGBA8, mat.cols, mat.rows);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mat.cols, mat.rows,
        GL_RGB, GL_UNSIGNED_BYTE, mat.data);

    static const GLint swizzles[] = { GL_BLUE, GL_GREEN, GL_RED, GL_ONE };
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzles);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_2D);
}
