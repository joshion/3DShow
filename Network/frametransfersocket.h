#pragma once

#include "transfersocket.h"

class FrameTransferSocket : public TransferSocket
{
    Q_OBJECT

public:
    FrameTransferSocket(QString deviceName, QString guid, Utilities::SocketType type, QString strIPAdress = "127.0.0.1");
    ~FrameTransferSocket();
protected:
    void analysisReceiveFrameBuffer(const TransferFrameBuffer& buffer) override;
private:
    
};
