#pragma once

#include "transfersocket.h"

class ImageTransferSocket : public TransferSocket
{
    Q_OBJECT

public:
    ImageTransferSocket(QString deviceName, Utilities::SocketType type,
        QString strIPAdress, unsigned int port);
    ~ImageTransferSocket();

protected:
    void analysisReceiveFrameBuffer(const TransferFrameBuffer& buffer) override;

protected slots:
    inline void slot_connected() override;
    inline void slot_disConnected() override;
public:
    cv::Mat popMat();
    int matsSize();
private:
    DecodeVideoStream *m_pDecoder;
    
};
