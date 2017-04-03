#include "imagetransfersocket.h"
#include "transferframebuffer.h"
#include "decodevideostream.h"

namespace
{
    static const unsigned int DATATYPE_RGB = 1;
    static const unsigned int DATATYPE_DEPTH = 2;
    static const unsigned int DATATYPE_SKELETON = 3;
}

ImageTransferSocket::ImageTransferSocket(QString deviceName, Utilities::SocketType type,
    QString strIPAdress, unsigned int port)
    : TransferSocket(deviceName, type, strIPAdress, port),
    m_pDecoder(nullptr)
{
}

ImageTransferSocket::~ImageTransferSocket()
{
    if (m_pDecoder)
    {
        m_pDecoder->stop();
        delete m_pDecoder;
        m_pDecoder = nullptr;
    }
}

void ImageTransferSocket::analysisReceiveFrameBuffer(const TransferFrameBuffer& buffer)
{
    switch (buffer.dataType())
    {
    case DATATYPE_RGB:
    case DATATYPE_DEPTH:
    {
        m_pDecoder->pushBytes(buffer.data(), buffer.bodyLength());
        // qDebug() << __FILE__ << __LINE__ << buffer.bodyLength();
    }
    break;
    default:
        break;
    }
}

inline void ImageTransferSocket::slot_connected()
{
    TransferSocket::slot_connected();
    if (nullptr == m_pDecoder)
    {
        m_pDecoder = new DecodeVideoStream;
        m_pDecoder->start();
    }
}

inline void ImageTransferSocket::slot_disConnected()
{
    TransferSocket::slot_disConnected();
    if (m_pDecoder)
    {
        m_pDecoder->stop();
        delete m_pDecoder;
        m_pDecoder = nullptr;
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