#pragma once

#include "singleton.h"

#include <QString>
#include <QMap>
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
        m_strIPAdress = ip;
    }

    void setGuid(QString guid)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_strGuid = guid;
    }

    void setServerPort(unsigned int port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_uServerPort = port;
    }

    void setColorPort(unsigned int port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_uColorPort = port;
    }

    void setDepthPort(unsigned int port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_uDepthPort = port;
    }

    void setSkelePort(unsigned int port)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_uSkelePort = port;
    }

    QString IPAdress()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strIPAdress;
    }

    QString guid()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_strGuid;
    }

    unsigned int serverPort()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_uServerPort;
    }

    unsigned int colorPort()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_uColorPort;
    }

    unsigned int depthPort()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_uDepthPort;
    }

    unsigned int skelePort()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_uSkelePort;
    }

    void setDeviceOnline(QString device)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_Device_Online.insert(device, true);
    }

    void setDeviceOffline(QString device)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_Device_Online.insert(device, false);
    }

    bool getDeviceStatus(QString device)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_Device_Online.contains(device))
        {
            return m_Device_Online[device];
        }
        else
        {
            return false;
        }
    }

private:
    /* 本类的所有变量存取共用一个锁 */
    std::mutex m_mutex;

    QString m_strIPAdress;
    QString m_strGuid;
    unsigned int  m_uServerPort;
    unsigned int  m_uColorPort;
    unsigned int  m_uDepthPort;
    unsigned int  m_uSkelePort;

    QMap<QString, bool> m_Device_Online;
};
