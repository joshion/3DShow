#pragma once

#include "transfersocket.h"

class FrameTransferSocket : public TransferSocket
{
    Q_OBJECT

public:
    FrameTransferSocket(QString deviceName, Utilities::SocketType type,
        QString strIPAdress, unsigned int port);
    ~FrameTransferSocket();
protected:
    void analysisReceiveFrameBuffer(const TransferFrameBuffer& buffer) override;
private:
};
