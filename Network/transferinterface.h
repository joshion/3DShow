#pragma once
class TransferInterface
{
public:
    TransferInterface();
    virtual ~TransferInterface();
public:
    virtual void signal_receiveData() = 0;

};
