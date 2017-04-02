#include "frametransfersocket.h"
#include "transferframebuffer.h"
#include "SkeletonFrame.h"

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
    SkeletonFrame frame = SkeletonFrame::fromBytes(buffer.data(), buffer.bodyLength());
    pushSkeletonFrame(frame);
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
