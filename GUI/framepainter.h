#ifndef FRAMEPAINTER_H
#define FRAMEPAINTER_H

#include "painter.h"

class FramePainter : public Painter
{
public:
    FramePainter();
    ~FramePainter();

public:
    bool buildShaderProgram(const QString &strVertFile, const QString& strFragFile) override;
    void paint() override;

private:
    
};

#endif // FRAMEPAINTER_H
