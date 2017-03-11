#ifndef TRANSFERSOCKET_H
#define TRANSFERSOCKET_H

#include "thread.h"

#include <QtNetwork/QTcpSocket>

#include <condition_variable>

class TransferSocket : public QTcpSocket, public Thread
{
    Q_OBJECT

public:
    TransferSocket(QString strIPAdress = "127.0.0.1", unsigned int port = 7892, QObject *parent = nullptr);
    ~TransferSocket();

private:
    QString m_strIPAdress;
    unsigned int m_uPort;
    bool m_bConnected = false;

private slots:
    inline void slot_setConnected();
    inline void slot_setDisConnected();

/***********************************************************************************/
/*数据解析相关的线程*/
protected:
    void workingFunc() override;
private:
    std::mutex m_bufferMutex;
    QByteArray m_receiveBuffer;

    std::mutex m_ReadyReadMutex;
    std::condition_variable m_ReadyReadCV;
    bool m_bReadyRead = false;

    bool m_bNotHasHead = true;
private:
    void analysisReceiveBuffer();
 
private slots:
    void slot_readDataFromServer();

};

#endif // TRANSFERSOCKET_H
