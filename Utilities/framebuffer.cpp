#include "framebuffer.h"

#include <QByteArray>
#include <QtEndian>
#include <memory>

FrameBuffer::FrameBuffer(FrameBuffer & other)
{
    this->m_u32length = other.m_u32length;
    this->m_ucCmdType = other.m_ucCmdType;
    this->m_ucCmdNum = other.m_ucCmdNum;
    this->m_u32Sequence = other.m_u32Sequence;
    this->m_ucVsersion = other.m_ucVsersion;

    if (m_u32length > 0 && other.m_data != nullptr)
    {
        m_data = new unsigned char[m_u32length] {0};
        memcpy(this->m_data, other.m_data, m_u32length);
    }
    else
    {
        m_u32length = 0;
        m_data = nullptr;
    }
}

/*
len即是data的长度
故只要len<=0 又或者 data==nullptr都是非法数据
此时两者都要置0
*/
FrameBuffer::FrameBuffer(unsigned char cmdType, unsigned char cmdNum, unsigned int sequence,
    unsigned char version, unsigned int len, unsigned char * data)
    : m_u32length(len), m_ucCmdType(cmdType), m_ucCmdNum(cmdNum),
    m_u32Sequence(sequence), m_ucVsersion(version), m_data(data)
{
    if (m_u32length > 0 && m_data != nullptr)
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


FrameBuffer::~FrameBuffer()
{
    if (m_data)
    {
        delete[] m_data;
    }
}

FrameBuffer & FrameBuffer::operator=(const FrameBuffer & other)
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
        memcpy(this->m_data, other.m_data, m_u32length);
    }
    else
    {
        this->m_u32length = 0;
        this->m_data = nullptr;
    }
    delete[] pOrig;
    return *this;
}

QByteArray FrameBuffer::toByte(const FrameBuffer &buffer)
{
    /*
    将小端数据转换为大端数据发送
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

FrameBuffer FrameBuffer::fromByte(const QByteArray & bytes)
{
    FrameBuffer buffer;
    memcpy(&buffer.m_cmdType, bytes.data(), 1);
    memcpy(&buffer.m_cmdNum, bytes.data() + 1, 1);
    memcpy(buffer.m_sequence, bytes.data() + 2, 4);
    memcpy(&buffer.m_version, bytes.data() + 6, 1);
    memcpy(buffer.m_length, bytes.data() + 7, 4);

    /*
    将网络传输的大端数据转换回小端数据
    用qToLittleEndian不会将数据顺序翻转
    但是用qToBigEndian却会
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
bool FrameBuffer::setHead(const QByteArray & bytes)
{
    if (bytes.length() < 11)
        return false;

    memcpy(&this->m_cmdType, bytes.data(), 1);
    memcpy(&this->m_cmdNum, bytes.data() + 1, 1);
    memcpy(this->m_sequence, bytes.data() + 2, 4);
    memcpy(&this->m_version, bytes.data() + 6, 1);
    memcpy(this->m_length, bytes.data() + 7, 4);

    /*
    将网络传输的大端数据转换回小端数据
    用qToLittleEndian不会将数据顺序翻转
    但是用qToBigEndian却会
    */
    this->m_u32Sequence = qToBigEndian(this->m_u32Sequence);
    this->m_u32length = qToBigEndian(this->m_u32length);

    // 清空数据,保留包体长度,之后的设置数据需要用到包体长度
    delete[] m_data;
    m_data = nullptr;

    return true;
}

bool FrameBuffer::setData(const QByteArray & bytes, const unsigned int length)
{
    if (bytes.length() < length || length <= 0)
    {
        return false;
    }
    else
    {
        if (m_data != nullptr)
        {
            delete[] m_data;
        }
        m_u32length = length;
        m_data = new unsigned char[length] {0};
        memcpy(m_data, bytes.data(), length);
        return true;
    }
}

void FrameBuffer::setData(const unsigned char * data, const unsigned int length)
{
    if (m_data != nullptr)
    {
        delete[] m_data;
    }
    if (length > 0 && data != nullptr)
    {
        m_u32length = length;
        m_data = new unsigned char[length] {0};
        memcpy(m_data, data, length);
    }
    else
    {
        m_u32length = 0;
        m_data = nullptr;
    }
}

void FrameBuffer::setData(const ::google::protobuf::Message & data)
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
