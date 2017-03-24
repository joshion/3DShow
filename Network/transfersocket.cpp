#include "transfersocket.h"
#include "transferframebuffer.h"
#include <QByteArray>

#include <qDebug>
#include <QHostAddress>

namespace
{
    static const unsigned int DATATYPE_RGB = 1;
    static const unsigned int DATATYPE_DEPTH = 2;
    static const unsigned int DATATYPE_SKELETON = 3;
}

TransferSocket::TransferSocket(QString strIPAdress, unsigned int port)
    : m_strIPAdress(strIPAdress),
    m_uPort(port),
    m_bConnected(false),
    m_pGUI(nullptr),
    m_bNotHasHead(true), 
    m_pReceiveFrameBuffer(nullptr)
{
    m_receiveBuffer.clear();
    m_pReceiveFrameBuffer = new TransferFrameBuffer;

    this->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    this->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    this->bind();
    qDebug() << this->localPort();
    this->connectToHost(m_strIPAdress, m_uPort, QIODevice::ReadWrite);
    connect(this, &TransferSocket::connected, this, &TransferSocket::slot_setConnected, Qt::QueuedConnection);
    connect(this, &TransferSocket::disconnected, this, &TransferSocket::slot_setDisConnected, Qt::QueuedConnection);
    connect(this, &TransferSocket::readyRead, this, &TransferSocket::slot_readDataFromServer, Qt::QueuedConnection);

    this->start();  // 启动解析线程
    this->write(QByteArray(1, 'c'));
}

TransferSocket::~TransferSocket()
{
    this->close();
    this->stop();   // 关闭解析线程

    delete m_pReceiveFrameBuffer;
}

void TransferSocket::slot_setConnected()
{
    qDebug() << this->localAddress();
    qDebug() << this->localPort();
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

#ifdef COMMENT
    {
        std::lock_guard<std::mutex> lock_buffer(m_mutexReceiveBuffer);
        m_pDecoder->pushBytes((unsigned char *) m_receiveBuffer.data(), m_receiveBuffer.length());
        m_receiveBuffer.remove(0, m_receiveBuffer.length());
    }
#endif // COMMENT

    bool bIsACompleteFrameBuffer = false;
    int length = 0;
    {
        std::lock_guard<std::mutex> lock_buffer(m_mutexReceiveBuffer);
        length = m_receiveBuffer.length();
    }
    while (length > 0)
    {
        {
            std::lock_guard<std::mutex> lock_buffer(m_mutexReceiveBuffer);

            /*
            若服务器发来的缓存少于12或者已经读取到了头部时跳过此部分,等待下次读取到更多缓存再
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

        {
            std::lock_guard<std::mutex> lock_buffer(m_mutexReceiveBuffer);
            length = m_receiveBuffer.length();
        }
    }
}

void TransferSocket::analysisReceiveFrameBuffer(const TransferFrameBuffer& buffer)
{
    switch (buffer.dataType())
    {
    case DATATYPE_RGB:
    {
    
    }
    break;
    case DATATYPE_DEPTH:
    {

    }
    break;
    case DATATYPE_SKELETON:
    {

    }
    default:
        break;
    }
}

void TransferSocket::slot_readDataFromServer()
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexReceiveBuffer);
    m_receiveBuffer.append(this->readAll());
    /* 通知解析线程解析数据 */
    notifyThreadToContinue();
}
