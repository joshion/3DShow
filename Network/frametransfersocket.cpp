#include "frametransfersocket.h"

FrameTransferSocket::FrameTransferSocket(QString deviceName, QString guid, Utilities::SocketType type, QString strIPAdress)
    : TransferSocket(deviceName, guid, type, strIPAdress)
{

}

FrameTransferSocket::~FrameTransferSocket()
{

}

void FrameTransferSocket::analysisReceiveFrameBuffer(const TransferFrameBuffer & buffer)
{
}
