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

void TransferInterfaceManager::signal_connectedToServer()
{
    for (auto a : m_Interfaces)
    {
        assert(a);
        a->signal_connectedToServer();
    }
}
