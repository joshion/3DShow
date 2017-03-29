#pragma once

#include "transfersocket.h"
#include "utilities.h"
#include <opencv2\opencv.hpp>

#include <QThread>

class TransferSocket;
class TransferInterface;

class TransferSocketThread : public QThread
{
    Q_OBJECT

public:
    TransferSocketThread(QString deviceName, QString guid, Utilities::SocketType type, unsigned int port,
        QString strIPAdress = "127.0.0.1", TransferInterface *pInterface = nullptr, QObject *parent = nullptr);
    ~TransferSocketThread();
public:
    TransferSocket* getTransferSocketPtr()
    {
        return m_pTransferSocket;
    }
private:
    QString m_strDeviceName;
    QString m_strGuid;
    Utilities::SocketType m_eSocketType;
    unsigned int m_uPort;
    QString m_strIPAdress;
    TransferInterface *m_pTransferInterface;    // �ϲ�GUI��������, �����䵽�²�C++����,�������²��ͨ��

/***********************************************************************************/
/*QThread�ڴ����Ķ���, ��ҪdeleteLater()ɾ��*/
protected:
    void run() override;
private:
    void createTransferSocket(Utilities::SocketType type);
private:
    TransferSocket *m_pTransferSocket;
/***********************************************************************************/
};
