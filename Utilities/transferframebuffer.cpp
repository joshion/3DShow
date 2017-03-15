#include "transferframebuffer.h"

#include <QByteArray>
#include <QtEndian>

#include <memory>

TransferFrameBuffer::TransferFrameBuffer()
{
}


TransferFrameBuffer::~TransferFrameBuffer()
{
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
        memcpy(&this->m_BodyLength, bytes.data() + 4, 4);

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
