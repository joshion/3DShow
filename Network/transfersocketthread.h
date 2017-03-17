#ifndef TRANSFERSOCKETTHREAD_H
#define TRANSFERSOCKETTHREAD_H

#include <QThread>

class TransferSocket;
class TransferInterface;

class TransferSocketThread : public QThread
{
    Q_OBJECT

public:
    TransferSocketThread(QString strIPAdress = "127.0.0.1", unsigned int port = 7892, 
        TransferInterface *pInterface = nullptr, QObject *parent = nullptr);
    ~TransferSocketThread();
private:
    QString m_strIPAdress;
    unsigned int m_uPort;

    TransferInterface *m_pTransferInterface;    // �ϲ�GUI��������, �����䵽�²�C++����,�������²��ͨ��

/***********************************************************************************/
/*QThread�ڴ����Ķ���, ��ҪdeleteLater()ɾ��*/
protected:
    void run() override;
private:
    TransferSocket *m_pTransferSocket;
/***********************************************************************************/
};

#endif // TRANSFERSOCKETTHREAD_H
