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
    explicit TransferSocket(QString strIPAdress = "127.0.0.1", unsigned int port = 7892);
    TransferSocket(TransferSocket &) = delete;
    TransferSocket& operator= (const TransferSocket &other) = delete;
    ~TransferSocket();
public:
    unsigned int BindRandomPort();
    void connectToServer(unsigned int uPort);

private:
    QString m_strIPAdress;
    unsigned int m_uPort;
    bool m_bConnected;

protected slots:
    inline virtual void slot_connected();
    inline virtual void slot_disConnected();

/***********************************************************************************/
/*��GUIͨ�����ýӿ�*/
private:
    TransferInterface *m_pGUI;
public:
    void registerGUIClass(TransferInterface*  gui)
    {
        this->m_pGUI = gui;
    };
/***********************************************************************************/


/***********************************************************************************/
/*���ݽ�����ص��߳�*/
public:

protected:
    void run() override;
    virtual void analysisReceiveFrameBuffer(const TransferFrameBuffer& buffer) = 0;

private:
    void analysisReceiveBytesBuffer();
private:
    QByteArray m_receiveBuffer;
    std::mutex m_mutexReceiveBuffer;

    bool m_bNotHasHead;
    TransferFrameBuffer *m_pReceiveFrameBuffer;

private slots:
    void slot_readDataFromServer();

/***********************************************************************************/
};
