#pragma once

#include "thread.h"
#include "transferinterface.h"

#include <opencv2\opencv.hpp>

#include <QTcpSocket>

class TransferFrameBuffer;
class DecodeVideoStream;

class QByteArray;

class TransferSocket : public QTcpSocket, public Thread
{
    Q_OBJECT

public:
    explicit TransferSocket(QString strIPAdress = "127.0.0.1");
    TransferSocket(TransferSocket &) = delete;
    TransferSocket& operator= (const TransferSocket &other) = delete;
    ~TransferSocket();
public slots:
    unsigned int bindRandomPort();
    void connectToServer(unsigned int uPort);

protected slots:
    inline virtual void slot_connected();
    inline virtual void slot_disConnected();

private:
    QString m_strIPAdress;
    unsigned int m_uPort;
    bool m_bConnected;
/***********************************************************************************/
/*��GUIͨ�����ýӿ�*/
private:
    TransferInterfaceManager m_InterfaceManager;
public:
    void registerGUIClass(TransferInterface*  gui)
    {
        m_InterfaceManager.registerInterface(gui);
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
