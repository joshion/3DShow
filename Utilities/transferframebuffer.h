#pragma once

#include <stdint.h>
class QByteArray;

class TransferFrameBuffer
{
public:
    TransferFrameBuffer(unsigned int timeStamp = 0, unsigned int dataType = 0,
        unsigned int bodyLength = 0, unsigned char* data = nullptr);
    TransferFrameBuffer(const TransferFrameBuffer &other);
    virtual ~TransferFrameBuffer();
    TransferFrameBuffer& operator= (const TransferFrameBuffer& other);

public:
    inline void setTimeStamp(unsigned int timeStamp)
    {
        this->m_timeStamp = timeStamp;
    }
    inline void setDataType(unsigned int dataType)
    {
        this->m_dataType = dataType;
    }
    inline void setBodyLength(unsigned int bodyLength)
    {
        this->m_bodyLength = bodyLength;
    }

    bool setHead(const QByteArray &bytes);
    bool setData(const QByteArray &bytes);
    bool setData(const QByteArray &bytes, const unsigned int length);
    bool setData(const unsigned char *data, const unsigned int length);

    inline unsigned int timeStamp() const
    {
        return m_timeStamp;
    }
    inline unsigned int dataType() const
    {
        return m_dataType;
    }
    inline unsigned int bodyLength() const
    {
        return m_bodyLength;
    }

    inline unsigned char* data() const
    {
        return m_Data;
    }

    inline unsigned int headLength() const
    {
        return s_headLength;
    }

private:
    uint32_t m_timeStamp;    // 时间戳
    uint32_t m_dataType;     // 数据类型
    uint32_t m_bodyLength;   // 包体长度
    unsigned char *m_Data;

    static const unsigned int s_headLength = 12;
};
