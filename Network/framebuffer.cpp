#include "framebuffer.h"
#include <memory>
#include <qbytearray.h>

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::FrameBuffer(unsigned int len, unsigned char *data = NULL)
	: m_u32length(len)
	, m_data(NULL)
{
    if (m_u32length > 0)
    {
        m_data = new char[m_u32length] {0};
        memcpy(m_data, data, m_u32length);
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
    bytes.append(buffer.m_data, buffer.m_u32length);
    return bytes;
}

FrameBuffer FrameBuffer::fromByte(const QByteArray & bytes)
{
    return FrameBuffer();
}
