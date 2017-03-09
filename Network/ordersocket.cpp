#include "ordersocket.h"
#include "framebuffer.h"
#include "orderinterface.hpp"

#include "ConnectProto.pb.h"
#include "KinectDataProto.pb.h"

#include <QtNetwork/QTcpSocket>
#include <QString>

namespace
{
    const static unsigned int CONNECT_PROTOCOL = 1;

    const static unsigned int REQUIRE_CONNECT = 1;
    const static unsigned int EXIT_CONNECT = 2;
    const static unsigned int REQUIRE_DEVICES = 3;
    const static unsigned int RESP_REQUIRE_CONNECT = 100;
    const static unsigned int RESP_DEVICES = 101;

    const static unsigned int KINECT_PROTOCOL = 2;

    const static unsigned int START_REQUIRE = 1;
    const static unsigned int END_REQUIRE = 2;
    const static unsigned int RESP_START_REQUIRE = 100;
    const static unsigned int SERVER_REQUIRE_END_CONNECT = 102;
}

OrderSocket::OrderSocket(QString adress, int port, QObject *parent)
    : QObject(parent), m_strIPAdress(adress), m_uPort(port)
{
    m_pSendFrameBuffer = new FrameBuffer {};
    m_pReceiveFrameBuffer = new FrameBuffer {};
    m_pTcpSocket = new QTcpSocket(this);
    m_pTcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    m_pTcpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    m_pTcpSocket->connectToHost(m_strIPAdress, m_uPort);
    connect(m_pTcpSocket, &QTcpSocket::connected, this, &OrderSocket::slot_setConnected, Qt::QueuedConnection);
    connect(m_pTcpSocket, &QTcpSocket::disconnected, this, &OrderSocket::slot_setDisConnected, Qt::QueuedConnection);
    connect(m_pTcpSocket, &QTcpSocket::readyRead, this, &OrderSocket::readDataFromServer, Qt::QueuedConnection);
}

OrderSocket::~OrderSocket()
{
    if (m_pTcpSocket)
    {
        m_pTcpSocket->close();
        delete m_pTcpSocket;
    }
    this->stop();
    if (m_pSendFrameBuffer)
    {
        delete m_pSendFrameBuffer;
    }
}

void OrderSocket::slot_setConnected()
{
    m_bConnected = true;
}

void OrderSocket::slot_setDisConnected()
{
    m_bConnected = false;
}

bool OrderSocket::writeBufferToServer() const
{
    QByteArray bytes = FrameBuffer::toByte(*m_pSendFrameBuffer);
    
    /*
    若服务器已经链接上,则发送数据,并返回true
    没有链接上则重新请求链接,并返回false
    */
    if (m_bConnected)
    {
        int writeLength = m_pTcpSocket->write(bytes);
        return writeLength == bytes.length();
    }
    else
    {
        m_pTcpSocket->connectToHost(m_strIPAdress, m_uPort);
        return false;
    }
}

bool OrderSocket::writeBufferToServer(const FrameBuffer & buffer) const
{
    QByteArray bytes = FrameBuffer::toByte(buffer);
    int writeLength = m_pTcpSocket->write(bytes);
    return writeLength == bytes.length();
}

bool OrderSocket::slot_requireConnect()
{
    m_pSendFrameBuffer->setCmdType(1);
    m_pSendFrameBuffer->setCmdNum(1);
    m_pSendFrameBuffer->setData(nullptr, 0);
    qDebug() << "enter require connect";
    return this->writeBufferToServer();
}

bool OrderSocket::slot_exitConnect()
{
    m_pSendFrameBuffer->setCmdType(1);
    m_pSendFrameBuffer->setCmdNum(2);
    m_pSendFrameBuffer->setData(nullptr, 0);
    qDebug() << "enter exit connect";
    return this->writeBufferToServer();
}

bool OrderSocket::slot_requireDevices()
{
    m_pSendFrameBuffer->setCmdType(1);
    m_pSendFrameBuffer->setCmdNum(3);
    m_pSendFrameBuffer->setData(nullptr, 0);
    qDebug() << "enter require devices";
    return this->writeBufferToServer();
}

bool OrderSocket::slot_startRequire(QString deviceName, unsigned int dataType)
{
    m_pSendFrameBuffer->setCmdType(2);
    m_pSendFrameBuffer->setCmdNum(1);
    KinectDataProto::pbReqStart req {};
    req.set_devicename(deviceName.toStdString());
    req.set_datatype(dataType);
    m_pSendFrameBuffer->setData(req);
    qDebug() << "enter start require";
    return this->writeBufferToServer();
}

bool OrderSocket::slot_endConnect()
{
    m_pSendFrameBuffer->setCmdType(2);
    m_pSendFrameBuffer->setCmdNum(2);
    m_pSendFrameBuffer->setData(nullptr, 0);
    qDebug() << "enter end connect";
    return this->writeBufferToServer();
}

void OrderSocket::workingFunc()
{
    analysisReceiveByteArrayBuffer();
}

void OrderSocket::analysisReceiveByteArrayBuffer()
{
    while (true)
    {
        std::unique_lock<std::mutex> ul(m_ReadyReadMutex);
        while (!m_bReadyRead)
        {
            m_ReadyReadCV.wait(ul);
        }
        /*
        缓存内可能粘包而存在多条指令,也可能存在不完整的指令
        所以循环解析指令,直到发现不完整的指令,等待接收到更多指令的时候再进行解析
        */
        while (true)
        {
            bool bIsACompleteFrameBuffer = false;
            {
                std::lock_guard<std::mutex> lg(m_bufferMutex);
                /*
                若服务器发来的缓存少于11或者已经读取到了头部时跳过此部分,等待下次读取到更多缓存再进行判断
                */
                if (m_bNotHasHead && m_receiveBuffer.length() >= 11)
                {
                    m_pReceiveFrameBuffer->setHead(m_receiveBuffer);
                    m_receiveBuffer.remove(0, 11);
                    m_bNotHasHead = false;
                }
                /*
                若没有读取到头部或者缓存的长度比头部指定的protobuf的长度小时跳过此部分,等待下次读取到更多缓存时再进行判断
                */
                if (!m_bNotHasHead && m_receiveBuffer.length() >= m_pReceiveFrameBuffer->length())
                {
                    m_pReceiveFrameBuffer->setData(m_receiveBuffer, m_pReceiveFrameBuffer->length());
                    m_receiveBuffer.remove(0, m_pReceiveFrameBuffer->length());
                    m_bNotHasHead = true;
                    bIsACompleteFrameBuffer = true;
                }
            }
            if (bIsACompleteFrameBuffer)
            {
                analysisReceiveFrameBuffer(*m_pReceiveFrameBuffer);
            }
            else //指令不完整,等待读取更待缓存
            {
                break;
            }
        }
        m_bReadyRead = false;
    }
}

void OrderSocket::analysisReceiveFrameBuffer(const FrameBuffer & buffer)
{
    switch (buffer.cmdType())
    {
    case CONNECT_PROTOCOL:
        switch (buffer.cmdNum())
        {
        case RESP_REQUIRE_CONNECT:
        {
            ConnectProto::pbRespConnect resp;
            resp.ParseFromArray(buffer.data(), buffer.length());
            resp.PrintDebugString();
            if (m_pGUI)
            {
                m_pGUI->signal_respConnect();
            }
        }
        break;
        case RESP_DEVICES:
        {
            ConnectProto::pbRespDevices resp;
            resp.ParseFromArray(buffer.data(), buffer.length());
            resp.PrintDebugString();
            if (m_pGUI)
            {
                m_pGUI->signal_respDevices();
            }
        }
        break;
        default:
            break;
        }
        break;
    case KINECT_PROTOCOL:
        switch (buffer.cmdNum())
        {
        case RESP_START_REQUIRE:
        {
            KinectDataProto::pbReqStart req;
            req.ParseFromArray(buffer.data(), buffer.length());
            req.PrintDebugString();
            if (m_pGUI)
            {
                m_pGUI->signal_respStartRequire();
            }
        }
        break;
        case SERVER_REQUIRE_END_CONNECT:
        {
            KinectDataProto::pbRespStart resp;
            resp.ParseFromArray(buffer.data(), buffer.length());
            resp.PrintDebugString();
            if (m_pGUI)
            {
                m_pGUI->signal_reqEndConnect();
            }
        }
        break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void OrderSocket::readDataFromServer()
{
    {
        std::lock_guard<std::mutex> lg(m_bufferMutex);
        m_receiveBuffer.append(m_pTcpSocket->readAll());
    }
    {
        std::unique_lock<std::mutex> ul(m_ReadyReadMutex);
        m_bReadyRead = true;
        m_ReadyReadCV.notify_all();
    }
}
