#pragma once

#include "thread.h"
#include "utilities.h"
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
    explicit TransferSocket(QString strIPAdress = "127.0.0.1",
        Utilities::SocketType type = Utilities::SocketType::Color);
    TransferSocket(TransferSocket &) = delete;
    TransferSocket& operator= (const TransferSocket &other) = delete;
    ~TransferSocket();

public slots:
    void connectToServer(unsigned int uPort);

protected slots:
    inline virtual void slot_connected();
    inline virtual void slot_disConnected();

private:
    QString m_strIPAdress;
    Utilities::SocketType m_eSocketType;
    unsigned int m_uPort;
    bool m_bConnected;
/***********************************************************************************/
/*与GUI通信所用接口*/
private:
    TransferInterfaceManager m_InterfaceManager;
public:
    /*
    创建类的实例后,应立刻注册上层GUI的指针,
    且类的构造函数中不能有信号的发出
    不然该实例发出的信号,上层GUI无法接收到
    */
    void registerGUIClass(TransferInterface*  gui)
    {
        m_InterfaceManager.registerInterface(gui);
    };
/***********************************************************************************/


/***********************************************************************************/
/*数据解析相关的线程*/
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
