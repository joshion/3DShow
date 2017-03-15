#pragma once

class QByteArray;

class TransferFrameBuffer
{
public:
    TransferFrameBuffer();
    virtual ~TransferFrameBuffer();

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

private:
    union
    {
        char m_TimeStamp[4];
        unsigned int m_u32TimeStamp;    // ʱ���
    };
    union
    {
        char m_DataType;
        unsigned int m_u32DataType;     // ��������
    };
    union
    {
        char m_BodyLength[4];
        unsigned int m_u32BodyLength;   // ���峤��
    };

    unsigned char *m_Data;
};

