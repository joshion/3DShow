#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QObject>

#include "thread.h"

#include <condition_variable>
#include <string>

class TcpSocketInterface;
class FrameBuffer;

class QString;
class QTcpSocket;

class TcpSocket : public QObject, public Thread
{
    Q_OBJECT
public:
    explicit TcpSocket(QString adress="127.0.0.1", int port= 7892, QObject *parent = 0);
    ~TcpSocket();

private:
    QString m_strIPAdress;
    int m_uPort;
    QTcpSocket *m_pTcpSocket;

/***********************************************************************************/
/*与GUI通信所用接口*/
public:
    void registerGUIClass(TcpSocketInterface*  gui)
    {
        this->m_pGUI = gui;
    };
private:
    TcpSocketInterface *m_pGUI;
/***********************************************************************************/

/***********************************************************************************/
/*发送指令到服务器的相关函数*/
private:
    FrameBuffer *m_pSendFrameBuffer;

private:
    bool writeBufferToServer() const;
    bool writeBufferToServer(const FrameBuffer & buffer) const;

public slots:
    bool requireConnect();
    bool exitConnect();
    bool requireDevices();
    bool startRequire(std::string deviceName, unsigned int dataType);
    bool endConnect();
/***********************************************************************************/

/***********************************************************************************/
/*与线程相关的变量以及函数*/
protected:
    virtual void WorkingFunc();

private:
    std::mutex m_bufferMutex;
    QByteArray m_receiveBuffer;

    std::mutex m_ReadyReadMutex;
    std::condition_variable m_ReadyReadCV;
    bool m_bReadyRead = false;

    bool m_bNotHasHead = true;
    FrameBuffer *m_pReceiveFrameBuffer;
private:
    void analysisReceiveByteArrayBuffer();
    void analysisReceiveFrameBuffer(const FrameBuffer &buffer);
private slots:
    void readDataFromServer();
/***********************************************************************************/
};

#endif // TCPSOCKET_H
