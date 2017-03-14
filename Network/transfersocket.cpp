#include "transfersocket.h"
#include <QByteArray>

TransferSocket::TransferSocket(QString strIPAdress, unsigned int port, QObject *parent)
    : QTcpSocket(parent), m_strIPAdress(strIPAdress), m_uPort(port)
{
    this->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    this->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    this->connectToHost(m_strIPAdress, m_uPort, QIODevice::ReadOnly);
    connect(this, &TransferSocket::connected, this, &TransferSocket::slot_setConnected, Qt::QueuedConnection);
    connect(this, &TransferSocket::disconnected, this, &TransferSocket::slot_setDisConnected, Qt::QueuedConnection);
    connect(this, &TransferSocket::readyRead, this, &TransferSocket::slot_readDataFromServer, Qt::QueuedConnection);
}

TransferSocket::~TransferSocket()
{
    this->close();
}

void TransferSocket::slot_setConnected()
{
    m_bConnected = true;
}

void TransferSocket::slot_setDisConnected()
{
    m_bConnected = false;
}

void TransferSocket::analysisReceiveBuffer()
{

}

void TransferSocket::slot_readDataFromServer()
{
    m_receiveBuffer.append(this->readAll());
 
}