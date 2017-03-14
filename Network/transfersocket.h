#ifndef TRANSFERSOCKET_H
#define TRANSFERSOCKET_H

#include <QTcpSocket>
#include <QThread>

class TransferInterface;

class QByteArray;

class TransferSocket : public QTcpSocket
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
/*与GUI通信所用接口*/
private:
    TransferInterface *m_pGUI = nullptr;
public:
    void registerGUIClass(TransferInterface*  gui)
    {
        this->m_pGUI = gui;
    };
/***********************************************************************************/


/***********************************************************************************/
/*数据解析相关的线程*/

private:
    void analysisReceiveBuffer();
private:
    QByteArray m_receiveBuffer;

private slots:
    void slot_readDataFromServer();

/***********************************************************************************/

};

#endif // TRANSFERSOCKET_H
