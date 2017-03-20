#include "validatedevice.h"

#include <memory>

#include <QByteArray>
#include <QString>


ValidateDevice::ValidateDevice(const unsigned char length, const unsigned char * data)
    : m_ucLength(length)
{
    this->setData(data, length);
}

ValidateDevice::~ValidateDevice()
{
    delete[] m_data;
}

ValidateDevice::ValidateDevice(const ValidateDevice & other)
{
    unsigned char* pOrig = this->m_data;

    if (other.m_ucLength > 0 && other.m_data != nullptr)
    {
        this->m_data = new unsigned char[other.m_ucLength] { 0 };
        memcpy(this->m_data, other.m_data, other.m_ucLength);
        this->m_ucLength = other.m_ucLength;
    }
    else
    {
        this->m_data = nullptr;
        this->m_ucLength = 0;
    }
    if (pOrig != nullptr)
    {
        delete[] pOrig;
        pOrig = nullptr;
    }
}

ValidateDevice & ValidateDevice::operator=(const ValidateDevice & other)
{
    unsigned char* pOrig = this->m_data;

    if (other.m_ucLength > 0 && other.m_data != nullptr)
    {
        this->m_data = new unsigned char[other.m_ucLength] { 0 };
        memcpy(this->m_data, other.m_data, other.m_ucLength);
        this->m_ucLength = other.m_ucLength;
    }
    else
    {
        this->m_data = nullptr;
        this->m_ucLength = 0;
    }

    if (pOrig != nullptr)
    {
        delete[] pOrig;
        pOrig = nullptr;
    }

    return *this;
}

QByteArray ValidateDevice::toByte(const ValidateDevice & other)
{
    QByteArray bytes;
    bytes.append(other.m_Length);
    bytes.append((char *) (other.m_data), other.m_ucLength);
    return bytes;
}

bool ValidateDevice::setData(const unsigned char * data, unsigned char length)
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

bool ValidateDevice::setData(const QByteArray & bytes, unsigned char length)
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

void ValidateDevice::setData(const QString & str)
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
