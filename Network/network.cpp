#include "network.h"

Network::Network()
{
    m_pSocket = new QTcpSocket;
}

Network::~Network()
{

}
