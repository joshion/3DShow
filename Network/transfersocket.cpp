#include "transfersocket.h"

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

}

void TransferSocket::slot_setConnected()
{
}

void TransferSocket::slot_setDisConnected()
{
}

void TransferSocket::workingFunc()
{
    this->analysisReceiveBuffer();
}

void TransferSocket::analysisReceiveBuffer()
{
    while (true)
    {
        std::unique_lock<std::mutex> ul(m_ReadyReadMutex);
        while (!m_bReadyRead)
        {
            m_ReadyReadCV.wait(ul);
        }
        while (true)
        {
            std::lock_guard<std::mutex> lg(m_bufferMutex);
            m_receiveBuffer.clear();
            
            // 在此处加处理数据以及信号转发的代码
        }
        m_bReadyRead = false;
    }
}

void TransferSocket::slot_readDataFromServer()
{
    {
        std::lock_guard<std::mutex> lg(m_bufferMutex);
        m_receiveBuffer.append(this->readAll());
    }
    {
        std::unique_lock<std::mutex> ul(m_ReadyReadMutex);
        m_bReadyRead = true;
        m_ReadyReadCV.notify_all();
    }
}