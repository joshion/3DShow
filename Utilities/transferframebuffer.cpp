#include "transferframebuffer.h"

#include <QByteArray>
#include <QtEndian>

#include <memory>

TransferFrameBuffer::TransferFrameBuffer(unsigned int timeStamp, unsigned int dataType,
    unsigned int bodyLength, unsigned char * data)
    : m_u32TimeStamp(timeStamp),
    m_u32DataType(dataType)
{
    if (data && bodyLength > 0)
    {
        m_Data = new unsigned char [bodyLength] {0};
        memcpy(m_Data, data, bodyLength);
        m_u32BodyLength = bodyLength;
    }
    else
    {
        m_u32BodyLength = 0;
        m_Data = nullptr;
    }
}

TransferFrameBuffer::TransferFrameBuffer(const TransferFrameBuffer & other)
    : m_u32TimeStamp(other.m_u32TimeStamp),
    m_u32DataType(other.m_u32DataType)
{
    unsigned char* pOrig = this->m_Data;

    if (other.m_Data && other.m_u32BodyLength > 0)
    {
        this->m_Data = new unsigned char [other.m_u32BodyLength] {0};
        memcpy(this->m_Data, other.m_Data, other.m_u32BodyLength);
        this->m_u32BodyLength = other.m_u32BodyLength;
    }
    else
    {
        this->m_u32BodyLength = 0;
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
    m_u32TimeStamp = other.m_u32TimeStamp;
    m_u32DataType = other.m_u32DataType;
    m_u32BodyLength = other.m_u32BodyLength;

    unsigned char *pOrig = this->m_Data;
    if (other.m_u32BodyLength > 0 && other.m_Data != nullptr)
    {
        this->m_Data = new unsigned char[other.m_u32BodyLength] { 0 };
        memcpy(this->m_Data, other.m_Data, other.m_u32BodyLength);
    }
    else
    {
        this->m_u32BodyLength = 0;
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
    if (bytes.length() < 12)
    {
        return false;
    }
    else
    {
        memcpy(&this->m_TimeStamp, bytes.data(), 4);
        memcpy(&this->m_DataType, bytes.data() + 4, 4);
        memcpy(&this->m_BodyLength, bytes.data() + 8, 4);

        /*
        将网络传输的大端数据转换回小端数据
        用qToLittleEndian不会将数据顺序翻转
        但是用qToBigEndian却会
        */
        this->m_u32TimeStamp = qToBigEndian(this->m_u32TimeStamp);
        this->m_u32DataType = qToBigEndian(this->m_u32DataType);
        this->m_u32BodyLength = qToBigEndian(this->m_u32BodyLength);

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
    return setData(bytes, this->m_u32BodyLength);
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
        m_u32BodyLength = length;
        m_Data = new unsigned char[length] {0};
        memcpy(m_Data, bytes.data(), length);
        return true;
    }
}

bool TransferFrameBuffer::setData(const unsigned char * data, const unsigned int length)
{
    if (data || length <= 0)
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
        m_u32BodyLength = length;
        m_Data = new unsigned char[length] {0};
        memcpy(m_Data, data, length);
        return true;
    }
}
