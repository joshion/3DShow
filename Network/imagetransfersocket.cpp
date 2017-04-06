#include "imagetransfersocket.h"
#include "transferframebuffer.h"
#include "decodevideostream.h"

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
    case TransferFrameBuffer::DATA_TYPE_COLOR:
    {
        m_pDecoder->pushBytes(buffer.data(), buffer.bodyLength());
    }
    break;
    case TransferFrameBuffer::DATA_TYPE_DEPTH:
    {
        m_pDecoder->pushBytes(buffer.data(), buffer.bodyLength());
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