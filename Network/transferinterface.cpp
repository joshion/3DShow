#include "transferinterface.h"

#include <cassert>

TransferInterface::TransferInterface()
{

}

TransferInterface::~TransferInterface()
{

}


TransferInterfaceManager::TransferInterfaceManager()
{
}

TransferInterfaceManager::~TransferInterfaceManager()
{
}

void TransferInterfaceManager::signal_getLocalPort(unsigned int uPort)
{
    for (auto a : m_Interfaces)
    {
        assert(a);
        a->signal_getLocalPort(uPort);
    }
}

void TransferInterfaceManager::signal_connectedToServer()
{
    for (auto a : m_Interfaces)
    {
        assert(a);
        a->signal_connectedToServer();
    }
}
