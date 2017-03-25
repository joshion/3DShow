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
    TransferSocketThread(TransferInterface *pInterface = nullptr, Utilities::ShowType type = Utilities::Color,
        QString strIPAdress = "127.0.0.1", unsigned int port = 7893, QObject *parent = nullptr);
    ~TransferSocketThread();
public:
    TransferSocket* getTransferSocketPtr()
    {
        return m_pTransferSocket;
    }
private:
    TransferInterface *m_pTransferInterface;    // �ϲ�GUI��������, �����䵽�²�C++����,�������²��ͨ��
    Utilities::ShowType m_eShowType;
    QString m_strIPAdress;
    unsigned int m_uPort;

/***********************************************************************************/
/*QThread�ڴ����Ķ���, ��ҪdeleteLater()ɾ��*/
protected:
    void run() override;
private:
    void createTransferSocket(Utilities::ShowType type);
private:
    TransferSocket *m_pTransferSocket;
/***********************************************************************************/
};
