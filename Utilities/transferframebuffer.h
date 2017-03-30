#pragma once

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
        this->m_u32TimeStamp = timeStamp;
    }
    inline void setDataType(unsigned int dataType)
    {
        this->m_u32DataType = dataType;
    }
    inline void setBodyLength(unsigned int bodyLength)
    {
        this->m_u32BodyLength = bodyLength;
    }

    bool setHead(const QByteArray &bytes);
    bool setData(const QByteArray &bytes);
    bool setData(const QByteArray &bytes, const unsigned int length);
    bool setData(const unsigned char *data, const unsigned int length);

    inline unsigned int timeStamp() const
    {
        return m_u32TimeStamp;
    }
    inline unsigned int dataType() const
    {
        return m_DataType;
    }
    inline unsigned int bodyLength() const
    {
        return m_u32BodyLength;
    }

    inline unsigned char* data() const
    {
        return m_Data;
    }

    inline unsigned int headLength() const
    {
        return s_u32HeadLength;
    }

private:
    union
    {
        char m_TimeStamp[4];
        unsigned int m_u32TimeStamp;    // 时间戳
    };
    union
    {
        char m_DataType;
        unsigned int m_u32DataType;     // 数据类型
    };
    union
    {
        char m_BodyLength[4];
        unsigned int m_u32BodyLength;   // 包体长度
    };
    unsigned char *m_Data;

    static const unsigned int s_u32HeadLength = 12;
};
