#include "transfersocketthread.h"

#include "frametransfersocket.h"
#include "imagetransfersocket.h"
#include "transferinterface.h"

TransferSocketThread::TransferSocketThread(QString deviceName, QString guid, Utilities::SocketType type,
    unsigned int port, QString strIPAdress, TransferInterface *pInterface, QObject *parent)
    : QThread(parent),
    m_strDeviceName(deviceName),
    m_strGuid(guid),
    m_eSocketType(type),
    m_uPort(port),
    m_strIPAdress(strIPAdress),
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
        m_pTransferSocket = new ImageTransferSocket { m_strDeviceName, m_strGuid, type };
    }
    else if (type == Utilities::SocketType::Depth)
    {
        m_pTransferSocket = new ImageTransferSocket { m_strDeviceName, m_strGuid, type };
    }
    else if (type == Utilities::SocketType::Skele)
    {
        m_pTransferSocket = new FrameTransferSocket { m_strDeviceName, m_strGuid, type };
    }
    m_pTransferSocket->registerGUIClass(m_pTransferInterface);
    m_pTransferSocket->connectToServer(m_uPort);
    connect(this, &TransferSocketThread::finished, m_pTransferSocket, &ImageTransferSocket::deleteLater);
    exec();
}

