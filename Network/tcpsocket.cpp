#include "tcpsocket.h"
#include "framebuffer.h"

#include <ConnectProto.pb.h>
#include <KinectDataProto.pb.h>

#include <QtNetwork/QTcpSocket>
#include <QString>

#include <string>

TcpSocket::TcpSocket(QString adress, int port, QObject *parent)
    : QObject(parent), m_strIPAdress(adress), m_uPort(port)
{
    m_pTcpSocket = new QTcpSocket(this);

    m_pTcpSocket->connectToHost(m_strIPAdress, m_uPort);

    connect(m_pTcpSocket, &QTcpSocket::readyRead, this, &TcpSocket::readDataFromServer);
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

    FrameBuffer buffer(1, 1, 0, 0, 0, nullptr);
    buffer.setData(resp);
    QByteArray bytes = FrameBuffer::toByte(buffer);
    int writeLength = m_pTcpSocket->write(bytes);
    return writeLength == bytes.length();
}

void TcpSocket::readDataFromServer()
{
    QByteArray bytes = m_pTcpSocket->readAll();
    FrameBuffer buffer = FrameBuffer::fromByte(bytes);
    ConnectProto::pbRespConnect resp;
    resp.ParseFromArray(buffer.data(), buffer.length());
    resp.PrintDebugString();
}
