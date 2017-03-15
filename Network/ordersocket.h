#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QObject>

class OrderInterface;
class OrderFrameBuffer;

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
class OrderSocket : public QObject
{
    Q_OBJECT
public:
    explicit OrderSocket(QString adress="127.0.0.1", unsigned int port= 7892, QObject *parent = nullptr);
    ~OrderSocket();

private:
    QString m_strIPAdress;
    unsigned int m_uPort;
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
    OrderFrameBuffer *m_pSendFrameBuffer = nullptr;

private:
    bool writeBufferToServer() const;
    bool writeBufferToServer(const OrderFrameBuffer & buffer) const;

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
    QByteArray m_receiveBuffer;

    bool m_bNotHasHead = true;
    OrderFrameBuffer *m_pReceiveFrameBuffer = nullptr;
private:
    void analysisReceiveByteArrayBuffer();
    void analysisReceiveFrameBuffer(const OrderFrameBuffer &buffer);

private slots:
    void slot_readDataFromServer();
/***********************************************************************************/
};

#endif // TCPSOCKET_H
