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
class QTimer;

class OrderSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit OrderSocket(QString adress = "", unsigned int port = 0, QObject *parent = nullptr);
    OrderSocket(const OrderSocket& other) = delete;
    OrderSocket& operator= (const OrderSocket& other) = delete;
    ~OrderSocket();

private:
    QString m_strIPAdress;
    unsigned int m_uPort;
    bool m_bConnected;
    QTimer *m_pKeepAliveTimer;  // 客户端主动保活的定时器
    QTimer *m_pRespKeepAliveTimer;  // 接收服务器保活的定时器

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
    void slot_requireConnect(const QString &ip, unsigned int port);
    void slot_exitConnect();
    void slot_requireDevices();
    void slot_startRequire(KinectDataProto::pbReqStart protoReqStart);
    void slot_endRequire(KinectDataProto::pbReqEnd reqEnd);

    void slot_keepAlive();
    void slot_AliveOverTime();
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
private:
    unsigned int m_Sequence_Main;
    unsigned int m_Sequence_RequireConnect;
    unsigned int m_Sequence_RequireDevices;
    unsigned int m_Sequence_StartRequire;
};
