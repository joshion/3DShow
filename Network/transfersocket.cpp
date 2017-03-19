#include "transfersocket.h"
#include "transferframebuffer.h"
#include <QByteArray>

TransferSocket::TransferSocket(QString strIPAdress, unsigned int port, QObject *parent)
    : m_strIPAdress(strIPAdress), m_uPort(port), QTcpSocket(parent),
    m_bConnected(false), m_pGUI(nullptr), m_bNotHasHead(true), m_pReceiveFrameBuffer(nullptr)
{
    m_receiveBuffer.clear();
    m_pReceiveFrameBuffer = new TransferFrameBuffer;

    this->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    this->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    this->connectToHost(m_strIPAdress, m_uPort, QIODevice::ReadOnly);
    connect(this, &TransferSocket::connected, this, &TransferSocket::slot_setConnected, Qt::QueuedConnection);
    connect(this, &TransferSocket::disconnected, this, &TransferSocket::slot_setDisConnected, Qt::QueuedConnection);
    connect(this, &TransferSocket::readyRead, this, &TransferSocket::slot_readDataFromServer, Qt::QueuedConnection);

    this->start();  // ���������߳�
}

TransferSocket::~TransferSocket()
{
    this->close();
    this->stop();   // �رս����߳�

    delete m_pReceiveFrameBuffer;
}

void TransferSocket::slot_setConnected()
{
    m_bConnected = true;
}

void TransferSocket::slot_setDisConnected()
{
    m_bConnected = false;
}

void TransferSocket::run()
{
    analysisReceiveBytesBuffer();
}

void TransferSocket::analysisReceiveBytesBuffer()
{
    bool bIsACompleteFrameBuffer = false;

    {
        std::lock_guard<std::mutex> lock_buffer(m_mutexReceiveBuffer);

        /*
        �������������Ļ�������11�����Ѿ���ȡ����ͷ��ʱ�����˲���,�ȴ��´ζ�ȡ�����໺����
        �����ж�
        */
        if (m_bNotHasHead && m_receiveBuffer.length() >= m_pReceiveFrameBuffer->headLength())
        {
            m_pReceiveFrameBuffer->setHead(m_receiveBuffer);
            m_receiveBuffer.remove(0, m_pReceiveFrameBuffer->headLength());
            m_bNotHasHead = false;
        }
        /*
        ��û�ж�ȡ��ͷ�����߻���ĳ��ȱ�ͷ��ָ����protobuf�ĳ���Сʱ�����˲���,�ȴ��´ζ�ȡ�����໺��ʱ�ٽ����ж�
        */
        if (!m_bNotHasHead && m_receiveBuffer.length() >= m_pReceiveFrameBuffer->bodyLength())
        {
            m_pReceiveFrameBuffer->setData(m_receiveBuffer, m_pReceiveFrameBuffer->bodyLength());
            m_receiveBuffer.remove(0, m_pReceiveFrameBuffer->bodyLength());
            m_bNotHasHead = true;
            bIsACompleteFrameBuffer = true;
        }
    }
    if (bIsACompleteFrameBuffer)
    {
        analysisReceiveFrameBuffer(*m_pReceiveFrameBuffer);
    }
}

void TransferSocket::analysisReceiveFrameBuffer(const TransferFrameBuffer& m_pReceiveFrameBuffer)
{
}

void TransferSocket::slot_readDataFromServer()
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexReceiveBuffer);
    m_receiveBuffer.append(this->readAll());
}