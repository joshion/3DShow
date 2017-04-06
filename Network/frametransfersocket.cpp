#include "frametransfersocket.h"
#include "transferframebuffer.h"
#include "skeletonframe.h"

FrameTransferSocket::FrameTransferSocket(QString deviceName, Utilities::SocketType type,
    QString strIPAdress, unsigned int port)
    : TransferSocket(deviceName, type, strIPAdress, port)
{
}

FrameTransferSocket::~FrameTransferSocket()
{
}

void FrameTransferSocket::analysisReceiveFrameBuffer(const TransferFrameBuffer & buffer)
{
    switch (buffer.dataType())
    {
    case TransferFrameBuffer::DATA_TYPE_SKELE:
    {
        if (buffer.data() && buffer.bodyLength() > 0)
        {
            SkeletonFrame frame = SkeletonFrame::fromBytes(buffer.data(), buffer.bodyLength());
            pushSkeletonFrame(frame);
        }
    }
    break;
    default:
        break;
    }
}

void FrameTransferSocket::pushSkeletonFrame(const SkeletonFrame & frame)
{
    std::lock_guard<std::mutex> lock(m_mutexSkeletonFrames);
    m_SkeletonFrames.push_back(frame);
}

SkeletonFrame FrameTransferSocket::popSkeletonFrame()
{
    std::lock_guard<std::mutex> lock(m_mutexSkeletonFrames);
    if (m_SkeletonFrames.isEmpty())
    {
        return SkeletonFrame();
    }
    else
    {
        SkeletonFrame result = m_SkeletonFrames.first();
        m_SkeletonFrames.pop_front();
        return result;
    }
}

int FrameTransferSocket::SkeletonFramesSizes()
{
    std::lock_guard<std::mutex> lock(m_mutexSkeletonFrames);
    return m_SkeletonFrames.size();
}
