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
    if (m_pFrameBuffer)
    {
        delete m_pFrameBuffer;
    }
}

bool TcpSocket::writeDataToServer()
{
    //requireConnect();
    //_sleep(2000);
    ////requireDevices();
    //startConnect("hello", 1);
    //_sleep(2000);
    //endConnect();
    //_sleep(2000);
    exitConnect();
    _sleep(2000);
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
    return this->writeBufferToServer();
}

bool TcpSocket::exitConnect()
{
    m_pFrameBuffer->setCmdType(1);
    m_pFrameBuffer->setCmdNum(2);
    m_pFrameBuffer->setData(nullptr, 0);
    return this->writeBufferToServer();
}

bool TcpSocket::requireDevices()
{
    m_pFrameBuffer->setCmdType(1);
    m_pFrameBuffer->setCmdNum(3);
    m_pFrameBuffer->setData(nullptr, 0);
    return this->writeBufferToServer();
}

bool TcpSocket::startConnect(std::string deviceName, unsigned int dataType)
{
    KinectDataProto::pbReqStart req;
    req.set_devicename(deviceName);
    req.set_datatype(dataType);
    m_pFrameBuffer->setData(req);
    return this->writeBufferToServer();
}

bool TcpSocket::endConnect()
{
    m_pFrameBuffer->setCmdType(2);
    m_pFrameBuffer->setCmdNum(2);
    m_pFrameBuffer->setData(nullptr, 0);
    return this->writeBufferToServer();
}

void TcpSocket::readDataFromServer()
{
    QByteArray bytes = m_pTcpSocket->readAll();
    FrameBuffer buffer = FrameBuffer::fromByte(bytes);
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
