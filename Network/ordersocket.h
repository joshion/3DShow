#pragma once

/*
����Ӧ����������������罻��
a.��������
b.������kinect�豸������
c.�Ͽ�����
��������Ӧ�Ľӿ���Ϊ OrderInterface
��Ҫ�ȵ���registerGUIClass()��ſ��Ե���start()�Կ����̶߳�ȡ����
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
    QTimer *m_pKeepAliveTimer;  // �ͻ�����������Ķ�ʱ��
    QTimer *m_pRespKeepAliveTimer;  // ���շ���������Ķ�ʱ��

private slots:
    inline void slot_setConnected();
    inline void slot_setDisConnected();
/***********************************************************************************/
/*��GUIͨ�����ýӿ�*/
public:
    void registerGUIClass(OrderInterface*  gui)
    {
        this->m_pGUI = gui;
    };
private:
    OrderInterface *m_pGUI;
/***********************************************************************************/

/***********************************************************************************/
/*����ָ�����������غ���*/
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
/*���߳���صı����Լ�����*/
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
