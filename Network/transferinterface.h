/*
* 底层组件中 有上层 GUI的指针
* 注意 GUI的生命周期一定要比 底层组件的生命周期要长
* 不然程序会发生未知错误
* 不允许下层组件 释放 上层GUI的内存, 如不能 delete GUI指针
*/

#pragma once
#include "utilities.h"
#include <QVector>

class TransferInterface
{
public:
    TransferInterface();
    virtual ~TransferInterface();
public:
    virtual void signal_getLocalPort(Utilities::SocketType type, unsigned int uPort) = 0;
    virtual void signal_connectedToServer() = 0;

};


class TransferInterfaceManager
{
public:
    TransferInterfaceManager();
    ~TransferInterfaceManager();

    TransferInterfaceManager(TransferInterfaceManager &) = delete;
    TransferInterfaceManager& operator= (TransferInterfaceManager &) = delete;

    void signal_getLocalPort(Utilities::SocketType type, unsigned int uPort);
    void signal_connectedToServer();

    inline void registerInterface(TransferInterface *pInterface)
    {
        if (pInterface)
        {
            m_Interfaces.push_back(pInterface);
        }
    }
private:
    QVector<TransferInterface* > m_Interfaces;
};
