#pragma once

#include "singleton.h"

#include <QString>

#include <mutex>

class Config : public SingleTon<Config>
{
    friend SingleTon<Config>;
private:
    Config();
    ~Config();

public:
    void setIPAdress(QString ip)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strServerIPAdress = ip;
    }

private:
    /* 本类的所有变量存取共用一个锁 */
    std::mutex m_mutex;

    QString m_strServerIPAdress;
    QString m_strGuid;
    unsigned int  m_uServerPort;
    unsigned int  m_uColorPort;
    unsigned int  m_uDepthPort;
    unsigned int  m_uSkelePort;
};

