#ifndef FRAMEPAINTER_H
#define FRAMEPAINTER_H

#include "painter.h"

class FramePainter : public Painter
{
public:
    FramePainter();
    ~FramePainter();

public:
    void paint() override;

private:
    
};

#endif // FRAMEPAINTER_H
