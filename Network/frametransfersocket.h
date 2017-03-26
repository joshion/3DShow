#pragma once

#include "transfersocket.h"

class FrameTransferSocket : public TransferSocket
{
    Q_OBJECT

public:
    FrameTransferSocket(QString strIPAdress, Utilities::SocketType type);
    ~FrameTransferSocket();
protected:
    void analysisReceiveFrameBuffer(const TransferFrameBuffer& buffer) override;
private:
    
};
