#ifndef TRANSFERINTERFACE_H
#define TRANSFERINTERFACE_H

#pragma once
class TransferInterface
{
public:
    TransferInterface();
    virtual ~TransferInterface();
public:
    virtual void signal_receiveData() = 0;

};

#endif // TRANSFERINTERFACE_H
