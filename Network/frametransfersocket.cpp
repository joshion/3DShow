#include "frametransfersocket.h"

FrameTransferSocket::FrameTransferSocket(QString strIPAdress, Utilities::SocketType type)
    : TransferSocket(strIPAdress, type)
{

}

FrameTransferSocket::~FrameTransferSocket()
{

}

void FrameTransferSocket::analysisReceiveFrameBuffer(const TransferFrameBuffer & buffer)
{
}
