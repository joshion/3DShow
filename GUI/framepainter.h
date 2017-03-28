#pragma once
#include "painter.h"

class FramePainter : public Painter
{
public:
    FramePainter();
    ~FramePainter();

public:
    bool buildShaderProgram(const QString &strVertFile, const QString& strFragFile) override;
    void paint() override;

public:
    void loadFrame();

private:
    GLuint m_VertexArraysObject[1];
    GLuint m_PointsBuffer[1];

    QVector4D *m_pTriangleVertices;
    QVector4D *m_pColor;
};
