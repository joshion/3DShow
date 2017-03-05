#ifndef NETWORK_H
#define NETWORK_H
#include <QtNetwork\qtcpsocket.h>

class Network
{
public:
    Network();
    ~Network();

private:
    QTcpSocket *m_pSocket;
};

#endif // NETWORK_H
