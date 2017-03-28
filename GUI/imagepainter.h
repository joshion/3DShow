#pragma once

#include "painter.h"

#include <opencv2\opencv.hpp>

class ImagePainter : public Painter
{

public:
    ImagePainter();
    ~ImagePainter();

public:
    bool buildShaderProgram(const QString &strVertFile, const QString& strFragFile) override;
    void paint() override;

public:
    void loadTexture(cv::Mat &mat);

private:
    QVector4D *m_pTriangleVertices;
    QVector2D *m_pCoord;
    GLuint m_Textures[1];
};
