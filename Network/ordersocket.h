#pragma once

/*
该类应用于与服务器的网络交互
a.请求链接
b.控制与kinect设备的链接
c.断开链接
该类所对应的接口类为 OrderInterface
需要先调用registerGUIClass()后才可以调用start()以开启线程读取缓存
*/

#include "KinectDataProto.pb.h"
#include <QTcpSocket>
class OrderInterface;
class OrderFrameBuffer;

class QString;

class OrderSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit OrderSocket(QString adress, unsigned int port, QObject *parent = nullptr);
    OrderSocket(const OrderSocket& other) = delete;
    OrderSocket& operator= (const OrderSocket& other) = delete;
    ~OrderSocket();

private:
    QString m_strIPAdress;
    unsigned int m_uPort;

private:
    bool m_bConnected;
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
    OrderFrameBuffer *m_pSendFrameBuffer;

private:
    bool writeBufferToServer();
    bool writeBufferToServer(const OrderFrameBuffer & buffer);

public slots:
    bool slot_requireConnect();
    bool slot_exitConnect();
    bool slot_requireDevices();
    bool slot_startRequire(KinectDataProto::pbReqStart protoReqStart);
    bool slot_endRequire(KinectDataProto::pbReqEnd reqEnd);
/***********************************************************************************/

/***********************************************************************************/
/*与线程相关的变量以及函数*/
protected:
    QByteArray m_receiveBuffer;

    bool m_bNotHasHead;
    OrderFrameBuffer *m_pReceiveFrameBuffer;
private:
    void analysisReceiveByteArrayBuffer();
    void analysisReceiveFrameBuffer(const OrderFrameBuffer &buffer);

private slots:
    void slot_readDataFromServer();
/***********************************************************************************/
};
