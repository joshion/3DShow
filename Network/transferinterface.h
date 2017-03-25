#pragma once
class TransferInterface
{
public:
    TransferInterface();
    virtual ~TransferInterface();
public:
    virtual void signal_getLocalPort(unsigned int uPort) = 0;
    virtual void signal_connectedToServer() = 0;

};
