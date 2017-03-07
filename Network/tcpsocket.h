#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <string>

class FrameBuffer;
class QString;
class QTcpSocket;

class TcpSocket : public QObject
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

public:
    bool writeDataToServer();
    bool writeBufferToServer() const;
    bool writeBufferToServer(const FrameBuffer & buffer);
    bool requireConnect();
    bool exitConnect();
    bool requireDevices();
    bool startConnect(std::string deviceName, unsigned int dataType);
    bool endConnect();

private:
    void analysisReceiveBuffer(const FrameBuffer &buffer);

public slots:
    void readDataFromServer();
};

#endif // TCPSOCKET_H
