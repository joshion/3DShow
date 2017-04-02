#pragma once

#include "transfersocket.h"
#include "SkeletonFrame.h"

#include <QList>

#include <mutex>

class FrameTransferSocket : public TransferSocket
{
    Q_OBJECT

public:
    FrameTransferSocket(QString deviceName, Utilities::SocketType type,
        QString strIPAdress, unsigned int port);
    ~FrameTransferSocket();
protected:
    void analysisReceiveFrameBuffer(const TransferFrameBuffer& buffer) override;

public:
    void pushSkeletonFrame(const SkeletonFrame & frame);
    SkeletonFrame popSkeletonFrame();
    int SkeletonFramesSizes();
private:
    QList<SkeletonFrame> m_SkeletonFrames;
    std::mutex m_mutexSkeletonFrames;
};
