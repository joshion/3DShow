#include "TcpSocket.h"
#include "framebuffer.h"

#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/qtcpsocket.h>
#include <QString>
#include <QTimer>


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
    FrameBuffer buffer(1, 1, 1, 1, 10, nullptr);
    QByteArray bytes = FrameBuffer::toByte(buffer);
    FrameBuffer buffer2;
    buffer2 = FrameBuffer::fromByte(bytes);
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
}
