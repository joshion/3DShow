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
    bool slot_requireConnect();
    bool slot_exitConnect();
    bool slot_requireDevices();
    bool slot_startRequire(KinectDataProto::pbReqStart protoReqStart);
    bool slot_endRequire(KinectDataProto::pbReqEnd reqEnd);
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
};
