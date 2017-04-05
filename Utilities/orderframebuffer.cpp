#include "orderframebuffer.h"

#include <QByteArray>
#include <QtEndian>
#include <memory>

/*
len即是data的长度
故只要len<=0 又或者 data==nullptr都是非法数据
此时两者都要置0
*/
OrderFrameBuffer::OrderFrameBuffer(unsigned char cmdType, unsigned char cmdNum, unsigned int sequence,
    unsigned char version, unsigned int len, unsigned char * data)
    : m_bodyLength(len),
    m_cmdType(cmdType),
    m_cmdNum(cmdNum),
    m_sequence(sequence),
    m_version(version)
{
    if (data && m_bodyLength > 0)
    {
        m_data = new unsigned char[m_bodyLength] {0};
        memcpy(m_data, data, m_bodyLength);
    }
    else
    {
        m_bodyLength = 0;
        m_data = nullptr;
    }
}

OrderFrameBuffer::OrderFrameBuffer(const OrderFrameBuffer & other)
    : m_cmdType(other.m_cmdType),
    m_cmdNum(other.m_cmdNum),
    m_sequence(other.m_sequence),
    m_version(other.m_version)
{
    unsigned char *pOrig = this->m_data;

    if (other.m_bodyLength > 0 && other.m_data != nullptr)
    {
        this->m_data = new unsigned char[other.m_bodyLength] { 0 };
        memcpy(this->m_data, other.m_data, other.m_bodyLength);
        this->m_bodyLength = other.m_bodyLength;
    }
    else
    {
        this->m_bodyLength = 0;
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
    this->m_bodyLength = other.m_bodyLength;
    this->m_cmdType = other.m_cmdType;
    this->m_cmdNum = other.m_cmdNum;
    this->m_sequence = other.m_sequence;
    this->m_version = other.m_version;

    unsigned char *pOrig = this->m_data;

    if (other.m_bodyLength > 0 && other.m_data != nullptr)
    {
        this->m_data = new unsigned char[other.m_bodyLength] { 0 };
        memcpy(this->m_data, other.m_data, other.m_bodyLength);
    }
    else
    {
        this->m_bodyLength = 0;
        this->m_data = nullptr;
    }

    if (pOrig != nullptr)
    {
        delete[] pOrig;
        pOrig = nullptr;
    }

    return *this;
}

QByteArray OrderFrameBuffer::toBytes(const OrderFrameBuffer &buffer)
{
    /*
    * 将uint32_t小端数据转换为大端数据发送
    * 即将uint32_t的[0][1][2][3]转换为
    * [3][2][1][0]
    */
    union DataUnion
    {
        char cData[4];
        unsigned int u32Data;
    };
    DataUnion length, sequence;
    sequence.u32Data = qToBigEndian(buffer.m_sequence);
    length.u32Data = qToBigEndian(buffer.m_bodyLength);

    QByteArray bytes;
    bytes.append(buffer.m_cmdType);
    bytes.append(buffer.m_cmdNum);
    bytes.append(sequence.cData, 4);
    bytes.append(buffer.m_version);
    bytes.append(length.cData, 4);
    bytes.append((char *) (buffer.m_data), buffer.m_bodyLength);
    return bytes;
}

OrderFrameBuffer OrderFrameBuffer::fromBytes(const QByteArray & bytes)
{
    OrderFrameBuffer buffer;
    memcpy(&buffer.m_cmdType, bytes.data(), 1);
    memcpy(&buffer.m_cmdNum, bytes.data() + 1, 1);
    memcpy(&buffer.m_sequence, bytes.data() + 2, 4);
    memcpy(&buffer.m_version, bytes.data() + 6, 1);
    memcpy(&buffer.m_bodyLength, bytes.data() + 7, 4);

    /*
    将网络传输的大端数据转换回小端数据
    用qToLittleEndian不会将数据顺序翻转
    但是用qToBigEndian却会
    */
    buffer.m_sequence = qToBigEndian(buffer.m_sequence);
    buffer.m_bodyLength = qToBigEndian(buffer.m_bodyLength);

    if (buffer.m_bodyLength > 0)
    {
        buffer.m_data = new unsigned char[buffer.m_bodyLength] { 0 };
        memcpy(buffer.m_data, bytes.data() + 11, buffer.m_bodyLength);
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
    memcpy(&this->m_sequence, bytes.data() + 2, 4);
    memcpy(&this->m_version, bytes.data() + 6, 1);
    memcpy(&this->m_bodyLength, bytes.data() + 7, 4);

    /*
    将网络传输的大端数据转换回小端数据
    用qToLittleEndian不会将数据顺序翻转
    但是用qToBigEndian却会
    */
    this->m_sequence = qToBigEndian(this->m_sequence);
    this->m_bodyLength = qToBigEndian(this->m_bodyLength);

    // 清空数据,保留包体长度,之后的设置数据需要用到包体长度
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
        m_bodyLength = length;
        m_data = new unsigned char[length] {0};
        memcpy(m_data, bytes.data(), length);
        return true;
    }
}

bool OrderFrameBuffer::setData(const unsigned char * data, const unsigned int length)
{
    if (nullptr == data || length <= 0)
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
        m_bodyLength = length;
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