#include "tcpsocket.h"
#include "framebuffer.h"

#include "ConnectProto.pb.h"
#include "KinectDataProto.pb.h"

#include <QtNetwork/QTcpSocket>
#include <QString>


TcpSocket::TcpSocket(QString adress, int port, QObject *parent)
    : QObject(parent), m_strIPAdress(adress), m_uPort(port)
{
    m_pFrameBuffer = new FrameBuffer();
    m_pTcpSocket = new QTcpSocket(this);
    m_pTcpSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    m_pTcpSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    m_pTcpSocket->connectToHost(m_strIPAdress, m_uPort);
    connect(m_pTcpSocket, &QTcpSocket::readyRead, this, &TcpSocket::readDataFromServer);
}

TcpSocket::~TcpSocket()
{
    if (m_pTcpSocket)
    {
        m_pTcpSocket->close();
        delete m_pTcpSocket;
    }
    this->stop();
    if (m_pFrameBuffer)
    {
        delete m_pFrameBuffer;
    }
}

bool TcpSocket::writeDataToServer()
{
    requireConnect();
    //requireDevices();
    //startConnect("hello", 1);
    endConnect();
    exitConnect();
    return true;
}

bool TcpSocket::writeBufferToServer() const
{
    QByteArray bytes = FrameBuffer::toByte(*m_pFrameBuffer);
    int writeLength = m_pTcpSocket->write(bytes);
    return writeLength == bytes.length();
}

bool TcpSocket::writeBufferToServer(const FrameBuffer & buffer)
{
    QByteArray bytes = FrameBuffer::toByte(buffer);
    int writeLength = m_pTcpSocket->write(bytes);
    return writeLength == bytes.length();
}

bool TcpSocket::requireConnect()
{
    m_pFrameBuffer->setCmdType(1);
    m_pFrameBuffer->setCmdNum(1);
    m_pFrameBuffer->setData(nullptr, 0);
    qDebug() << "enter require connect";
    return this->writeBufferToServer();
}

bool TcpSocket::exitConnect()
{
    m_pFrameBuffer->setCmdType(1);
    m_pFrameBuffer->setCmdNum(2);
    m_pFrameBuffer->setData(nullptr, 0);
    qDebug() << "enter exit connect";
    return this->writeBufferToServer();
}

bool TcpSocket::requireDevices()
{
    m_pFrameBuffer->setCmdType(1);
    m_pFrameBuffer->setCmdNum(3);
    m_pFrameBuffer->setData(nullptr, 0);
    qDebug() << "enter require devices";
    return this->writeBufferToServer();
}

bool TcpSocket::startRequire(std::string deviceName, unsigned int dataType)
{
    KinectDataProto::pbReqStart req;
    req.set_devicename(deviceName);
    req.set_datatype(dataType);
    m_pFrameBuffer->setData(req);
    qDebug() << "enter start require";
    return this->writeBufferToServer();
}

bool TcpSocket::endConnect()
{
    m_pFrameBuffer->setCmdType(2);
    m_pFrameBuffer->setCmdNum(2);
    m_pFrameBuffer->setData(nullptr, 0);
    qDebug() << "enter end connect";
    return this->writeBufferToServer();
}

void TcpSocket::WorkingFunc()
{
    analysisReceiveByteArrayBuffer();
}

void TcpSocket::analysisReceiveByteArrayBuffer()
{
    while (true)
    {
        std::unique_lock<std::mutex> ul(m_ReadyReadMutex);
        while (!m_bReadyRead)
        {
            m_ReadyReadCV.wait(ul);
        }
        FrameBuffer buffer;
        {
            std::lock_guard<std::mutex> lg(m_bufferMutex);
            buffer = FrameBuffer::fromByte(m_receiveBuffer);
            m_receiveBuffer.clear();
        }
        analysisReceiveFrameBuffer(buffer);
        m_bReadyRead = false;
    }
}

void TcpSocket::analysisReceiveFrameBuffer(const FrameBuffer & buffer)
{
    switch (buffer.cmdType())
    {
    case 1:
        switch (buffer.cmdNum())
        {
        case 100:
        {
            ConnectProto::pbRespConnect resp;
            resp.ParseFromArray(buffer.data(), buffer.length());
            resp.PrintDebugString();
        }
        break;
        case 101:
        {
            ConnectProto::pbRespDevices resp;
            resp.ParseFromArray(buffer.data(), buffer.length());
            resp.PrintDebugString();
        }
        break;
        default:
            break;
        }
        break;
    case 2:
        switch (buffer.cmdNum())
        {
        case 100:
        {
            KinectDataProto::pbReqStart req;
            req.ParseFromArray(buffer.data(), buffer.length());
            req.PrintDebugString();
        }
        break;
        case 101:
        {
            KinectDataProto::pbRespStart resp;
            resp.ParseFromArray(buffer.data(), buffer.length());
            resp.PrintDebugString();
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

void TcpSocket::readDataFromServer()
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
