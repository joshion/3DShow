#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>

class QString;
class QTcpSocket;
class QTimer;

using namespace std;

const static QString SERVER_IPADRESS = "127.0.0.1";
const static int SERVER_PORT = 7892;

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
    QTimer *m_pTimer;

public:
    bool writeDataToServer();

public slots:
    void readDataFromServer();
    void receiveMessage();
};

#endif // TCPSOCKET_H
