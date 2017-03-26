#include "transfersocketthread.h"

#include "frametransfersocket.h"
#include "imagetransfersocket.h"
#include "transferinterface.h"

TransferSocketThread::TransferSocketThread(TransferInterface *pInterface, Utilities::SocketType type, 
    QString strIPAdress, QObject *parent)
    : QThread(parent),
    m_eSocketType(type),
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
    if (m_eSocketType == Utilities::SocketType::Color)
    {
        m_pTransferSocket = new ImageTransferSocket { m_strIPAdress };
        m_pTransferSocket->registerGUIClass(m_pTransferInterface);
        m_pTransferSocket->bindRandomPort();
        connect(this, &TransferSocketThread::finished, m_pTransferSocket, &ImageTransferSocket::deleteLater);
        exec();
    }
    else if (m_eSocketType == Utilities::SocketType::Depth)
    {
        m_pTransferSocket = new ImageTransferSocket { m_strIPAdress };
        m_pTransferSocket->registerGUIClass(m_pTransferInterface);
        m_pTransferSocket->bindRandomPort();
        connect(this, &TransferSocketThread::finished, m_pTransferSocket, &ImageTransferSocket::deleteLater);
        exec();
    }
    else if (m_eSocketType == Utilities::SocketType::Skele)
    {
        m_pTransferSocket = new FrameTransferSocket { m_strIPAdress };
        m_pTransferSocket->registerGUIClass(m_pTransferInterface);
        m_pTransferSocket->bindRandomPort();
        connect(this, &TransferSocketThread::finished, m_pTransferSocket, &ImageTransferSocket::deleteLater);
        exec();
    }
}

