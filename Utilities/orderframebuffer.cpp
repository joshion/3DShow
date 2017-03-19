#include "orderframebuffer.h"

#include <QByteArray>
#include <QtEndian>
#include <memory>

/*
len����data�ĳ���
��ֻҪlen<=0 �ֻ��� data==nullptr���ǷǷ�����
��ʱ���߶�Ҫ��0
*/
OrderFrameBuffer::OrderFrameBuffer(unsigned char cmdType, unsigned char cmdNum, unsigned int sequence,
    unsigned char version, unsigned int len, unsigned char * data)
    : m_u32length(len), m_ucCmdType(cmdType), m_ucCmdNum(cmdNum),
    m_u32Sequence(sequence), m_ucVsersion(version)
{
    if (data && m_u32length > 0)
    {
        m_data = new unsigned char[m_u32length] {0};
        memcpy(m_data, data, m_u32length);
    }
    else
    {
        m_u32length = 0;
        m_data = nullptr;
    }
}

OrderFrameBuffer::OrderFrameBuffer(const OrderFrameBuffer & other)
    : m_ucCmdType(other.m_ucCmdType), m_ucCmdNum(other.m_ucCmdNum),
    m_u32Sequence(other.m_u32Sequence), m_ucVsersion(other.m_ucVsersion)
{
    unsigned char *pOrig = this->m_data;

    if (other.m_u32length > 0 && other.m_data != nullptr)
    {
        this->m_data = new unsigned char[other.m_u32length] {0};
        memcpy(this->m_data, other.m_data, other.m_u32length);
        this->m_u32length = other.m_u32length;
    }
    else
    {
        this->m_u32length = 0;
        this->m_data = nullptr;
    }

    if (pOrig != nullptr)
    {
        delete[] pOrig;
        pOrig = nullptr;
    }
}

OrderFrameBuffer::~OrderFrameBuffer()
{
    if (m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
}

OrderFrameBuffer & OrderFrameBuffer::operator=(const OrderFrameBuffer & other)
{
    this->m_u32length = other.m_u32length;
    this->m_ucCmdType = other.m_ucCmdType;
    this->m_ucCmdNum = other.m_ucCmdNum;
    this->m_u32Sequence = other.m_u32Sequence;
    this->m_ucVsersion = other.m_ucVsersion;

    unsigned char *pOrig = this->m_data;

    if (other.m_u32length > 0 && other.m_data != nullptr)
    {
        this->m_data = new unsigned char[other.m_u32length] {0};
        memcpy(this->m_data, other.m_data, other.m_u32length);
    }
    else
    {
        this->m_u32length = 0;
        this->m_data = nullptr;
    }

    if (pOrig != nullptr)
    {
        delete[] pOrig;
        pOrig = nullptr;
    }

    return *this;
}

QByteArray OrderFrameBuffer::toByte(const OrderFrameBuffer &buffer)
{
    /*
    ��С������ת��Ϊ������ݷ���
    */
    union DataUnion
    {
        char cData[4];
        unsigned int u32Data;
    };
    DataUnion length, sequence;
    sequence.u32Data = qToBigEndian(buffer.m_u32Sequence);
    length.u32Data = qToBigEndian(buffer.m_u32length);

    QByteArray bytes;
    bytes.append(buffer.m_cmdType);
    bytes.append(buffer.m_cmdNum);
    bytes.append(sequence.cData, 4);
    bytes.append(buffer.m_version);
    bytes.append(length.cData, 4);
    bytes.append((char *)(buffer.m_data), buffer.m_u32length);
    return bytes;
}

OrderFrameBuffer OrderFrameBuffer::fromByte(const QByteArray & bytes)
{
    OrderFrameBuffer buffer;
    memcpy(&buffer.m_cmdType, bytes.data(), 1);
    memcpy(&buffer.m_cmdNum, bytes.data() + 1, 1);
    memcpy(buffer.m_sequence, bytes.data() + 2, 4);
    memcpy(&buffer.m_version, bytes.data() + 6, 1);
    memcpy(buffer.m_length, bytes.data() + 7, 4);

    /*
    �����紫��Ĵ������ת����С������
    ��qToLittleEndian���Ὣ����˳��ת
    ������qToBigEndianȴ��
    */
    buffer.m_u32Sequence = qToBigEndian(buffer.m_u32Sequence);
    buffer.m_u32length = qToBigEndian(buffer.m_u32length);

    if (buffer.m_u32length > 0)
    {
        buffer.m_data = new unsigned char[buffer.m_u32length] { 0 };
        memcpy(buffer.m_data, bytes.data() + 11, buffer.m_u32length);
    }
    else
    {
        buffer.m_data = nullptr;
    }
    return buffer;
}

// 
bool OrderFrameBuffer::setHead(const QByteArray & bytes)
{
    if (bytes.length() < 11)
        return false;

    memcpy(&this->m_cmdType, bytes.data(), 1);
    memcpy(&this->m_cmdNum, bytes.data() + 1, 1);
    memcpy(this->m_sequence, bytes.data() + 2, 4);
    memcpy(&this->m_version, bytes.data() + 6, 1);
    memcpy(this->m_length, bytes.data() + 7, 4);

    /*
    �����紫��Ĵ������ת����С������
    ��qToLittleEndian���Ὣ����˳��ת
    ������qToBigEndianȴ��
    */
    this->m_u32Sequence = qToBigEndian(this->m_u32Sequence);
    this->m_u32length = qToBigEndian(this->m_u32length);

    // �������,�������峤��,֮�������������Ҫ�õ����峤��
    if (m_data != nullptr)
    {
        delete[] m_data;
        m_data = nullptr;
    }

    return true;
}

bool OrderFrameBuffer::setData(const QByteArray & bytes, const unsigned int length)
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
        m_u32length = length;
        m_data = new unsigned char[length] {0};
        memcpy(m_data, bytes.data(), length);
        return true;
    }
}

bool OrderFrameBuffer::setData(const unsigned char * data, const unsigned int length)
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
        m_u32length = length;
        m_data = new unsigned char[length] {0};
        memcpy(m_data, data, length);
        return true;
    }
}

void OrderFrameBuffer::setData(const ::google::protobuf::Message & data)
{
    unsigned int length = data.ByteSize();
    unsigned char *byteArray = new unsigned char[length] { 0 };
    data.SerializePartialToArray(byteArray, length);
    this->setData(byteArray, length);
    if (byteArray != nullptr)
    {
        delete[] byteArray;
    }
}
