#include "frametransfersocket.h"

FrameTransferSocket::FrameTransferSocket(QString strIPAdress, unsigned int port)
    : TransferSocket(strIPAdress, port)
{

}

FrameTransferSocket::~FrameTransferSocket()
{

}

void FrameTransferSocket::analysisReceiveFrameBuffer(const TransferFrameBuffer & buffer)
{
}
