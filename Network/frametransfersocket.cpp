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
}