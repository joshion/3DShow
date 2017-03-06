#include "tcpsocket.h"
#include "framebuffer.h"

#include <ConnectProto.pb.h>
#include <KinectDataProto.pb.h>

//#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QTcpSocket>
#include <QString>
#include <QTimer>

#include <string>

TcpSocket::TcpSocket(QString adress, int port, QObject *parent)
    : QObject(parent), m_strIPAdress(adress), m_uPort(port)
{
    m_pTcpSocket = new QTcpSocket(this);
    //m_pTimer = new QTimer(this);
    m_pTcpSocket->connectToHost(m_strIPAdress, m_uPort);

    connect(m_pTcpSocket, &QTcpSocket::readyRead, this, &TcpSocket::readDataFromServer);
    //connect(m_pTimer, &QTimer::timeout, this, &TcpSocket::receiveMessage);
}

TcpSocket::~TcpSocket()
{
    m_pTcpSocket->close();
    delete m_pTcpSocket;
}

bool TcpSocket::writeDataToServer()
{
    ConnectProto::pbRespConnect resp;

    resp.set_failreason("failure");
    resp.set_resulttype(1);

    std::string output;
    resp.SerializeToString(&output);
    
    resp.ByteSize();
    char *data = new char[resp.ByteSize()] {0};
    resp.SerializePartialToArray(data, resp.ByteSize());
    
    resp.PrintDebugString();

    FrameBuffer buffer(1, 1, 0, 0, resp.ByteSize(), (unsigned char *)data);
    QByteArray bytes = FrameBuffer::toByte(buffer);
    int writeLength = m_pTcpSocket->write(bytes);
    return writeLength == bytes.length();
}

/**
 * 2016-07-01 10:08:47
 * @brief TcpSocket::receiveMessage 向服务器请求刷新信息
 */
void TcpSocket::receiveMessage()
{
    writeDataToServer();
}


void TcpSocket::readDataFromServer()
{
    QByteArray bytes = m_pTcpSocket->readAll();
    FrameBuffer buffer = FrameBuffer::fromByte(bytes);
}
