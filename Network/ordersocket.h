#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QObject>

#include "thread.h"

#include <condition_variable>

class OrderInterface;
class FrameBuffer;

class QString;
class QTcpSocket;

/*
����Ӧ����������������罻��
a.��������
b.������kinect�豸������
c.�Ͽ�����
��������Ӧ�Ľӿ���Ϊ OrderInterface
��Ҫ�ȵ���registerGUIClass()��ſ��Ե���start()�Կ����̶߳�ȡ����
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
/*��GUIͨ�����ýӿ�*/
public:
    void registerGUIClass(OrderInterface*  gui)
    {
        this->m_pGUI = gui;
    };
private:
    OrderInterface *m_pGUI = nullptr;
/***********************************************************************************/

/***********************************************************************************/
/*����ָ�����������غ���*/
private:
    FrameBuffer *m_pSendFrameBuffer = nullptr;

private:
    bool writeBufferToServer() const;
    bool writeBufferToServer(const FrameBuffer & buffer) const;

public slots:
    bool slot_requireConnect();
    bool slot_exitConnect();
    bool slot_requireDevices();
    bool slot_startRequire(QString deviceName, unsigned int dataType);
    bool slot_endConnect();
/***********************************************************************************/

/***********************************************************************************/
/*���߳���صı����Լ�����*/
protected:
    virtual void WorkingFunc();

private:
    std::mutex m_bufferMutex;
    QByteArray m_receiveBuffer;

    std::mutex m_ReadyReadMutex;
    std::condition_variable m_ReadyReadCV;
    bool m_bReadyRead = false;

    bool m_bNotHasHead = true;
    FrameBuffer *m_pReceiveFrameBuffer = nullptr;
private:
    void analysisReceiveByteArrayBuffer();
    void analysisReceiveFrameBuffer(const FrameBuffer &buffer);
private slots:
    void readDataFromServer();
/***********************************************************************************/
};

#endif // TCPSOCKET_H
