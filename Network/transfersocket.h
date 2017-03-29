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
    * ����������������Ӷ����ڹ��캯����
    * ����Ϊע���ϲ�GUI���ڹ���֮��,����ڹ��캯���ڽ�������
    * �ᵼ���ź��޷����ݵ��ϲ�GUI
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
/*��GUIͨ�����ýӿ�*/
private:
    TransferInterfaceManager m_InterfaceManager;
public:
    /*
    �������ʵ����,Ӧ����ע���ϲ�GUI��ָ��,
    ����Ĺ��캯���в������źŵķ���
    ��Ȼ��ʵ���������ź�,�ϲ�GUI�޷����յ�
    */
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
