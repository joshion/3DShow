/*
* �ײ������ ���ϲ� GUI��ָ��
* ע�� GUI����������һ��Ҫ�� �ײ��������������Ҫ��
* ��Ȼ����ᷢ��δ֪����
* �������²���� �ͷ� �ϲ�GUI���ڴ�, �粻�� delete GUIָ��
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
