#include "framebuffer.h"
#include <memory>
#include <qbytearray.h>

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

FrameBuffer::FrameBuffer(unsigned int len, unsigned char cmdType, unsigned char cmdNum,
    unsigned int sequence, unsigned char version, unsigned char * data)
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

QByteArray FrameBuffer::toByte(const FrameBuffer &buffer)
{
    QByteArray bytes;
    bytes.append(buffer.m_length, 4);
    bytes.append(buffer.m_cmdType);
    bytes.append(buffer.m_cmdNum);
    bytes.append(buffer.m_sequence, 4);
    bytes.append(buffer.m_version);
    bytes.append((char *)(buffer.m_data), buffer.m_u32length);
    return bytes;
}

FrameBuffer FrameBuffer::fromByte(const QByteArray & bytes)
{
    FrameBuffer buffer;
    memcpy(buffer.m_length, bytes.data(), 4);
    memcpy(&buffer.m_cmdType, bytes.data() + 4, 1);
    memcpy(&buffer.m_cmdNum, bytes.data() + 5, 1);
    memcpy(buffer.m_sequence, bytes.data() + 6, 4);
    memcpy(&buffer.m_version, bytes.data() + 10, 1);

    if (buffer.m_u32length > 0)
    {
        buffer.m_data = new unsigned char[buffer.m_u32length] {0};
        memcpy(buffer.m_data, bytes.data() + 11, buffer.m_u32length);
    }
    else
    {
        buffer.m_data = nullptr;
    }
    return buffer;
}
