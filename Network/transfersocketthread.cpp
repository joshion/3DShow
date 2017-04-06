#include "transfersocketthread.h"

#include "frametransfersocket.h"
#include "imagetransfersocket.h"
#include "transferinterface.h"

TransferSocketThread::TransferSocketThread(QString deviceName, Utilities::SocketType type,
    QString strIPAdress, unsigned int port, TransferInterface *pInterface, QObject *parent)
    : QThread(parent),
    m_strDeviceName(deviceName),
    m_eSocketType(type),
    m_strIPAdress(strIPAdress),
    m_uPort(port),
    m_pTransferInterface(pInterface),
    m_pTransferSocket(nullptr)
{
    this->start();
}

TransferSocketThread::~TransferSocketThread()
{
    this->quit();
    this->wait(1000);
}

void TransferSocketThread::run()
{
    createTransferSocket(m_eSocketType);
}

void TransferSocketThread::createTransferSocket(Utilities::SocketType type)
{
    if (type == Utilities::SocketType::Color)
    {
        m_pTransferSocket = new ImageTransferSocket { m_strDeviceName, type, m_strIPAdress, m_uPort };
    }
    else if (type == Utilities::SocketType::Depth)
    {
        m_pTransferSocket = new ImageTransferSocket { m_strDeviceName, type, m_strIPAdress, m_uPort };
    }
    else if (type == Utilities::SocketType::Skele)
    {
        m_pTransferSocket = new FrameTransferSocket { m_strDeviceName, type, m_strIPAdress, m_uPort };
    }
    m_pTransferSocket->registerGUIClass(m_pTransferInterface);
    m_pTransferSocket->connectToServer();

    connect(this, &TransferSocketThread::finished,
        m_pTransferSocket, &TransferSocket::disconnectFromHost, Qt::QueuedConnection);
    connect(this, &TransferSocketThread::finished,
        m_pTransferSocket, &TransferSocket::deleteLater, Qt::QueuedConnection);
    exec();
}