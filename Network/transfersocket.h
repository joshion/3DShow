#pragma once

#include "thread.h"

#include <opencv2\opencv.hpp>

#include <QTcpSocket>

class TransferInterface;
class TransferFrameBuffer;
class DecodeVideoStream;

class QByteArray;

class TransferSocket : public QTcpSocket, public Thread
{
    Q_OBJECT

public:
    explicit TransferSocket(QString strIPAdress = "127.0.0.1", unsigned int port = 7892, QObject *parent = nullptr);
    TransferSocket(TransferSocket &) = delete;
    TransferSocket& operator= (const TransferSocket &other) = delete;
    ~TransferSocket();

private:
    QString m_strIPAdress;
    unsigned int m_uPort;
    bool m_bConnected;

private slots:
    inline void slot_setConnected();
    inline void slot_setDisConnected();

/***********************************************************************************/
/*与GUI通信所用接口*/
private:
    TransferInterface *m_pGUI;
public:
    void registerGUIClass(TransferInterface*  gui)
    {
        this->m_pGUI = gui;
    };
/***********************************************************************************/


/***********************************************************************************/
/*数据解析相关的线程*/
public:

protected:
    void run() override;

private:
    void analysisReceiveBytesBuffer();
    void analysisReceiveFrameBuffer(const TransferFrameBuffer& buffer);
private:
    QByteArray m_receiveBuffer;
    std::mutex m_mutexReceiveBuffer;

    bool m_bNotHasHead;
    TransferFrameBuffer *m_pReceiveFrameBuffer;

private slots:
    void slot_readDataFromServer();

/***********************************************************************************/

public:
    cv::Mat popMat();
    int matsSize();
private:
    DecodeVideoStream *m_pDecoder;
};
