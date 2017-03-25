#include "transfersocketthread.h"

#include "frametransfersocket.h"
#include "imagetransfersocket.h"
#include "transferinterface.h"

TransferSocketThread::TransferSocketThread(TransferInterface *pInterface, Utilities::ShowType type, 
    QString strIPAdress, unsigned int port, QObject *parent)
    : QThread(parent),
    m_eShowType(type),
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
    createTransferSocket(m_eShowType);
}

void TransferSocketThread::createTransferSocket(Utilities::ShowType type)
{
    if (m_eShowType & Utilities::Color)
    {
        m_pTransferSocket = new ImageTransferSocket { m_strIPAdress, m_uPort };
        m_pTransferSocket->registerGUIClass(m_pTransferInterface);
        connect(this, &TransferSocketThread::finished, m_pTransferSocket, &ImageTransferSocket::deleteLater);
        exec();
    }
    else if (m_eShowType & Utilities::Depth)
    {
    }
    else if (m_eShowType & Utilities::Skele)
    {
        m_pTransferSocket = new FrameTransferSocket { m_strIPAdress, m_uPort };
        m_pTransferSocket->registerGUIClass(m_pTransferInterface);
        connect(this, &TransferSocketThread::finished, m_pTransferSocket, &ImageTransferSocket::deleteLater);
        exec();
    }
}

