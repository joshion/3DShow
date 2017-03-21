#ifndef TRANSFERSOCKET_H
#define TRANSFERSOCKET_H

#include "thread.h"
#include "decodevediostream.h"
#include <QTcpSocket>

#include "decodevediostream.h"
#include <QFile>
#include <QByteArray>
#include <QTimer>

class TransferInterface;
class TransferFrameBuffer;

class QByteArray;

class TransferSocket : public QTcpSocket, public Thread
{
    Q_OBJECT

public:
    explicit TransferSocket(QString strIPAdress = "127.0.0.1", unsigned int port = 7892, QObject *parent = nullptr);
    TransferSocket(TransferSocket &) = delete;
    TransferSocket& operator= (const TransferSocket &other) = delete;
    ~TransferSocket();

private:
    QString m_strIPAdress;
    unsigned int m_uPort;
    bool m_bConnected;

private slots:
    inline void slot_setConnected();
    inline void slot_setDisConnected();

/***********************************************************************************/
/*��GUIͨ�����ýӿ�*/
private:
    TransferInterface *m_pGUI;
public:
    void registerGUIClass(TransferInterface*  gui)
    {
        this->m_pGUI = gui;
    };
/***********************************************************************************/


/***********************************************************************************/
/*���ݽ�����ص��߳�*/
public:
private:
    DecodeVedioStream *m_pDecoder;

protected:
    void run() override;

private:
    void analysisReceiveBytesBuffer();
    void analysisReceiveFrameBuffer(const TransferFrameBuffer& buffer);
private:
    QByteArray m_receiveBuffer;
    std::mutex m_mutexReceiveBuffer;

    bool m_bNotHasHead;
    TransferFrameBuffer *m_pReceiveFrameBuffer;

private slots:
    void slot_readDataFromServer();

    void slot_getVedioData();

/***********************************************************************************/
private:

    QTimer *m_Timer;
    DecodeVedioStream *pDecoder;
    QFile file;

};

#endif // TRANSFERSOCKET_H
