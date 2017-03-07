#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QObject>

#include "thread.h"
#include <condition_variable>
#include <string>

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
    FrameBuffer *m_pFrameBuffer;

public :
    bool writeDataToServer();
    bool writeBufferToServer() const;
    bool writeBufferToServer(const FrameBuffer & buffer);

public slots:
    bool requireConnect();
    bool exitConnect();
    bool requireDevices();
    bool startRequire(std::string deviceName, unsigned int dataType);
    bool endConnect();

protected:
    virtual void WorkingFunc();

private:
    std::mutex m_bufferMutex;
    QByteArray m_receiveBuffer;

    std::mutex m_ReadyReadMutex;
    std::condition_variable m_ReadyReadCV;
    bool m_bReadyRead = false;


private:
    void analysisReceiveByteArrayBuffer();
    void analysisReceiveFrameBuffer(const FrameBuffer &buffer);

public slots:
    void readDataFromServer();

};

#endif // TCPSOCKET_H
