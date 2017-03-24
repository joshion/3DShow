#pragma once

#include "transfersocket.h"

class FrameTransferSocket : public TransferSocket
{
    Q_OBJECT

public:
    FrameTransferSocket(QString strIPAdress = "127.0.0.1", unsigned int port = 7892);
    ~FrameTransferSocket();
protected:
    void analysisReceiveFrameBuffer(const TransferFrameBuffer& buffer) override;
private:
    
};
