#include "transferframebuffer.h"

#include <QByteArray>
#include <QtEndian>

#include <memory>

TransferFrameBuffer::TransferFrameBuffer(unsigned int timeStamp, unsigned int dataType,
    unsigned int bodyLength, unsigned char * data)
    : m_timeStamp(timeStamp),
    m_dataType(dataType)
{
    if (data && bodyLength > 0)
    {
        m_Data = new unsigned char [bodyLength] {0};
        memcpy(m_Data, data, bodyLength);
        m_bodyLength = bodyLength;
    }
    else
    {
        m_bodyLength = 0;
        m_Data = nullptr;
    }
}

TransferFrameBuffer::TransferFrameBuffer(const TransferFrameBuffer & other)
    : m_timeStamp(other.m_timeStamp),
    m_dataType(other.m_dataType)
{
    unsigned char* pOrig = this->m_Data;

    if (other.m_Data && other.m_bodyLength > 0)
    {
        this->m_Data = new unsigned char[other.m_bodyLength] { 0 };
        memcpy(this->m_Data, other.m_Data, other.m_bodyLength);
        this->m_bodyLength = other.m_bodyLength;
    }
    else
    {
        this->m_bodyLength = 0;
        this->m_Data = nullptr;
    }

    if (pOrig != nullptr)
    {
        delete[] pOrig;
        pOrig = nullptr;
    }
}

TransferFrameBuffer::~TransferFrameBuffer()
{
    if (m_Data)
    {
        delete[] m_Data;
        m_Data = nullptr;
    }
}

TransferFrameBuffer & TransferFrameBuffer::operator=(const TransferFrameBuffer & other)
{
    m_timeStamp = other.m_timeStamp;
    m_dataType = other.m_dataType;
    m_bodyLength = other.m_bodyLength;

    unsigned char *pOrig = this->m_Data;
    if (other.m_bodyLength > 0 && other.m_Data != nullptr)
    {
        this->m_Data = new unsigned char[other.m_bodyLength] { 0 };
        memcpy(this->m_Data, other.m_Data, other.m_bodyLength);
    }
    else
    {
        this->m_bodyLength = 0;
        this->m_Data = nullptr;
    }

    if (pOrig != nullptr)
    {
        delete[] pOrig;
        pOrig = nullptr;
    }

    return *this;
}

bool TransferFrameBuffer::setHead(const QByteArray & bytes)
{
    if (bytes.length() < s_headLength)
    {
        return false;
    }
    else
    {
        memcpy(&this->m_timeStamp, bytes.data(), 4);
        memcpy(&this->m_dataType, bytes.data() + 4, 4);
        memcpy(&this->m_bodyLength, bytes.data() + 8, 4);

        /*
        将网络传输的大端数据转换回小端数据
        用qToLittleEndian不会将数据顺序翻转
        但是用qToBigEndian却会
        */
        this->m_timeStamp = qToBigEndian(this->m_timeStamp);
        this->m_dataType = qToBigEndian(this->m_dataType);
        this->m_bodyLength = qToBigEndian(this->m_bodyLength);

        /*
        清空数据,保留包体长度,之后的设置数据需要用到包体长度
        */
        if (m_Data != nullptr)
        {
            delete[] m_Data;
            m_Data = nullptr;
        }
        return true;
    }
}

bool TransferFrameBuffer::setData(const QByteArray & bytes)
{
    return setData(bytes, this->m_bodyLength);
}

bool TransferFrameBuffer::setData(const QByteArray & bytes, const unsigned int length)
{
    if (bytes.length() <= 0 || bytes.length() < length)
    {
        return false;
    }
    else
    {
        if (m_Data)
        {
            delete[] m_Data;
            m_Data = nullptr;
        }
        m_bodyLength = length;
        m_Data = new unsigned char[length] {0};
        memcpy(m_Data, bytes.data(), length);
        return true;
    }
}

bool TransferFrameBuffer::setData(const unsigned char * data, const unsigned int length)
{
    if (nullptr == data || length <= 0)
    {
        return false;
    }
    else
    {
        if (m_Data)
        {
            delete[] m_Data;
            m_Data = nullptr;
        }
        m_bodyLength = length;
        m_Data = new unsigned char[length] {0};
        memcpy(m_Data, data, length);
        return true;
    }
}