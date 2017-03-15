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
        �����紫��Ĵ������ת����С������
        ��qToLittleEndian���Ὣ����˳��ת
        ������qToBigEndianȴ��
        */
        this->m_u32TimeStamp = qToBigEndian(this->m_u32TimeStamp);
        this->m_u32DataType = qToBigEndian(this->m_u32DataType);
        this->m_u32BodyLength = qToBigEndian(this->m_u32BodyLength);

        /*
        �������,�������峤��,֮�������������Ҫ�õ����峤��
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
