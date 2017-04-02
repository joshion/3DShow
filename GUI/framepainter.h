#pragma once
#include "painter.h"
#include "SkeletonFrame.h"

class FramePainter : public Painter
{
public:
    FramePainter();
    ~FramePainter();

public:
    bool buildShaderProgram(const QString &strVertFile, const QString& strFragFile) override;
    void paint() override;

public:
    void loadFrame(const SkeletonFrame& frame);
    void loadFrame(QVector4D* pVertices, QVector4D* pColors, unsigned int verticesSize,
        unsigned short* pElement, unsigned int elementSize);

private:
    GLuint m_VertexArraysObject;
    GLuint m_PointsBuffer;
    GLuint m_ElementBuffer;
};
