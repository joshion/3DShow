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
    TransferSocketThread(TransferInterface *pInterface = nullptr, Utilities::SocketType type = Utilities::SocketType::Color,
        QString strIPAdress = "127.0.0.1", QObject *parent = nullptr);
    ~TransferSocketThread();
public:
    TransferSocket* getTransferSocketPtr()
    {
        return m_pTransferSocket;
    }
private:
    TransferInterface *m_pTransferInterface;    // �ϲ�GUI��������, �����䵽�²�C++����,�������²��ͨ��
    Utilities::SocketType m_eSocketType;
    QString m_strIPAdress;

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
