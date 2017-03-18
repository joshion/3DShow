#include "validatedeive.h"

#include <memory>

#include <QByteArray>
#include <QString>


ValidateDeive::ValidateDeive()
{
}


ValidateDeive::~ValidateDeive()
{
}

QByteArray ValidateDeive::toByte(const ValidateDeive & other)
{
    QByteArray bytes;
    bytes.append(other.m_Length);
    bytes.append((char *) (other.m_data), other.m_ucLength);
    return bytes;
}

bool ValidateDeive::setData(const unsigned char * data, unsigned char length)
{
    if (data || length <= 0)
    {
        return false;
    }
    else
    {
        if (m_data != nullptr)
        {
            delete[] m_data;
            m_data = nullptr;
        }
        m_ucLength = length;
        m_data = new unsigned char[length] {0};
        memcpy(m_data, data, length);
        return true;
    }
}

bool ValidateDeive::setData(const QByteArray & bytes, unsigned char length)
{
    if (bytes.length() <= 0 || bytes.length() < length)
    {
        return false;
    }
    else
    {
        if (m_data != nullptr)
        {
            delete[] m_data;
            m_data = nullptr;
        }
        m_ucLength = length;
        m_data = new unsigned char[length] {0};
        memcpy(m_data, bytes.data(), length);
        return true;
    }
}

void ValidateDeive::setData(const QString & str)
{
    if (m_data != nullptr)
    {
        delete[] m_data;
        m_data = nullptr;
    }

    if (str.isEmpty())
    {
        m_ucLength = 0;
    }
    else
    {
        m_ucLength = str.length();
        m_data = new unsigned char[m_ucLength] {0};
        memcpy(m_data, str.data(), m_ucLength);
    }
}
