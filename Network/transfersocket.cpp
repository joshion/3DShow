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

    this->start();  // 启动解析线程
}

TransferSocket::~TransferSocket()
{
    this->close();
    this->stop();   // 关闭解析线程

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
        若服务器发来的缓存少于11或者已经读取到了头部时跳过此部分,等待下次读取到更多缓存再
        进行判断
        */
        if (m_bNotHasHead && m_receiveBuffer.length() >= m_pReceiveFrameBuffer->headLength())
        {
            m_pReceiveFrameBuffer->setHead(m_receiveBuffer);
            m_receiveBuffer.remove(0, m_pReceiveFrameBuffer->headLength());
            m_bNotHasHead = false;
        }
        /*
        若没有读取到头部或者缓存的长度比头部指定的protobuf的长度小时跳过此部分,等待下次读取到更多缓存时再进行判断
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