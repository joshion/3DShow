#include "imagetransfersocket.h"
#include "transferframebuffer.h"
#include "decodevideostream.h"

namespace
{
    static const unsigned int DATATYPE_RGB = 1;
    static const unsigned int DATATYPE_DEPTH = 2;
    static const unsigned int DATATYPE_SKELETON = 3;
}

ImageTransferSocket::ImageTransferSocket(QString strIPAdress, unsigned int port)
    : TransferSocket(strIPAdress, port),
    m_pDecoder(nullptr)
{
    m_pDecoder = new DecodeVideoStream;

}

ImageTransferSocket::~ImageTransferSocket()
{
    if (m_pDecoder)
    {
        delete m_pDecoder;
    }
}


void ImageTransferSocket::analysisReceiveFrameBuffer(const TransferFrameBuffer& buffer)
{
    switch (buffer.dataType())
    {
    case DATATYPE_RGB:
    {
        m_pDecoder->pushBytes(buffer.data(), buffer.bodyLength());
    }
    break;
    case DATATYPE_DEPTH:
    {

    }
    break;
    case DATATYPE_SKELETON:
    {

    }
    default:
        break;
    }
}

cv::Mat ImageTransferSocket::popMat()
{
    if (m_pDecoder)
    {
        return m_pDecoder->popMat();
    }
    else
    {
        return cv::Mat {};
    }
}

int ImageTransferSocket::matsSize()
{
    if (m_pDecoder)
    {
        return m_pDecoder->matsSize();
    }
    else
    {
        return 0;
    }
}
