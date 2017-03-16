#ifndef IMAGEPAINTER_H
#define IMAGEPAINTER_H

#include "painter.h"

#include <opencv2\opencv.hpp>

class ImagePainter : public Painter
{

public:
    ImagePainter();
    ~ImagePainter();

public:
    void paint() override;

public:
    void loadTexture(cv::Mat &mat);

private:
    GLuint m_Textures[1];
    
};

#endif // IMAGEPAINTER_H
