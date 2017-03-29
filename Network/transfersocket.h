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
    explicit TransferSocket(QString deviceName, QString guid, Utilities::SocketType type,
        QString strIPAdress = "127.0.0.1");
    TransferSocket(TransferSocket &) = delete;
    TransferSocket& operator= (const TransferSocket &other) = delete;
    ~TransferSocket();

protected slots:
    inline virtual void slot_connected();
    inline virtual void slot_disConnected();

public:
    /*
    * 把与服务器建立连接独立在构造函数外
    * 是因为注册上层GUI是在构造之后,如果在构造函数内建立连接
    * 会导致信号无法传递到上层GUI
    */
    void connectToServer(unsigned int port);
private:
    void validatePort();

private:
    QString m_strDeviceName;
    QString m_strGuid;
    Utilities::SocketType m_eSocketType;
    unsigned int m_uPort;
    QString m_strIPAdress;
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
