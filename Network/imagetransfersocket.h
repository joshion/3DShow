#pragma once

#include "transfersocket.h"

class ImageTransferSocket : public TransferSocket
{
    Q_OBJECT

public:
    ImageTransferSocket(QString strIPAdress = "127.0.0.1", unsigned int port = 7892);
    ~ImageTransferSocket();

protected:
    void analysisReceiveFrameBuffer(const TransferFrameBuffer& buffer) override;

public:
    cv::Mat popMat();
    int matsSize();
private:
    DecodeVideoStream *m_pDecoder;
    
};
