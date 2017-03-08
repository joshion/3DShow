#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QObject>

#include "thread.h"

#include <condition_variable>
#include <string>

class OrderInterface;
class FrameBuffer;

class QString;
class QTcpSocket;

/*
该类应用于与服务器的网络交互
a.请求链接
b.控制与kinect设备的链接
c.断开链接
该类所对应的接口类为 OrderInterface
*/
class OrderSocket : public QObject, public Thread
{
    Q_OBJECT
public:
    explicit OrderSocket(QString adress="127.0.0.1", int port= 7892, QObject *parent = 0);
    ~OrderSocket();

private:
    QString m_strIPAdress;
    int m_uPort;
    QTcpSocket *m_pTcpSocket;

private:
    bool m_bConnected = false;
private slots:
    inline void slot_setConnected();
    inline void slot_setDisConnected();
/***********************************************************************************/
/*与GUI通信所用接口*/
public:
    void registerGUIClass(OrderInterface*  gui)
    {
        this->m_pGUI = gui;
    };
private:
    OrderInterface *m_pGUI;
/***********************************************************************************/

/***********************************************************************************/
/*发送指令到服务器的相关函数*/
private:
    FrameBuffer *m_pSendFrameBuffer;

private:
    bool writeBufferToServer() const;
    bool writeBufferToServer(const FrameBuffer & buffer) const;

public slots:
    bool requireConnect();
    bool exitConnect();
    bool requireDevices();
    bool startRequire(std::string deviceName, unsigned int dataType);
    bool endConnect();
/***********************************************************************************/

/***********************************************************************************/
/*与线程相关的变量以及函数*/
protected:
    virtual void WorkingFunc();

private:
    std::mutex m_bufferMutex;
    QByteArray m_receiveBuffer;

    std::mutex m_ReadyReadMutex;
    std::condition_variable m_ReadyReadCV;
    bool m_bReadyRead = false;

    bool m_bNotHasHead = true;
    FrameBuffer *m_pReceiveFrameBuffer;
private:
    void analysisReceiveByteArrayBuffer();
    void analysisReceiveFrameBuffer(const FrameBuffer &buffer);
private slots:
    void readDataFromServer();
/***********************************************************************************/
};

#endif // TCPSOCKET_H
