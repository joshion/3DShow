#ifndef TRANSFERSOCKETTHREAD_H
#define TRANSFERSOCKETTHREAD_H

#include <QThread>

class TransferSocket;
class TransferInterface;

class TransferSocketThread : public QThread
{
    Q_OBJECT

public:
    TransferSocketThread(TransferInterface *pInterface = nullptr, QObject *parent = nullptr);
    ~TransferSocketThread();

protected:
    void run() override;

private:
    TransferSocket *m_pTransferSocket;
    TransferInterface *m_pTransferInterface;    // �ϲ�GUI��������, �����䵽�²�C++����,�������²��ͨ��
};

#endif // TRANSFERSOCKETTHREAD_H
