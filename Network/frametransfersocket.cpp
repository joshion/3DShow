#include "frametransfersocket.h"

FrameTransferSocket::FrameTransferSocket(QString strIPAdress)
    : TransferSocket(strIPAdress)
{

}

FrameTransferSocket::~FrameTransferSocket()
{

}

void FrameTransferSocket::analysisReceiveFrameBuffer(const TransferFrameBuffer & buffer)
{
}
