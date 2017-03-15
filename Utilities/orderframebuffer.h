#pragma once
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <google/protobuf/message.h>

class QByteArray;

class OrderFrameBuffer
{
public:
    OrderFrameBuffer(unsigned char cmdType = 0, unsigned char cmdNum = 0, unsigned int sequence = 0,
        unsigned char version = 0, unsigned int len = 0, unsigned char * data = nullptr);
    OrderFrameBuffer(OrderFrameBuffer &other);
    ~OrderFrameBuffer();

    OrderFrameBuffer& operator=(const OrderFrameBuffer& other);

public:
    static QByteArray toByte(const OrderFrameBuffer &buffer);
    static OrderFrameBuffer fromByte(const QByteArray &bytes);

public:
    inline void setCmdType(const unsigned char cmdType)
    {
        m_ucCmdType = cmdType;
    }
    inline void setCmdNum(const unsigned char cmdNum)
    {
        m_ucCmdNum = cmdNum;
    }
    inline void setSequence(const unsigned int sequence)
    {
        m_u32Sequence = sequence;
    }
    inline void setVersion(const unsigned char version)
    {
        m_ucVsersion = version;
    }

    /*
    头的长度为11,故bytes的长度至少要有11
    在此设置了头部,里面包含了包体长度,此时data内容清空,但是包体长度不变
    */
    bool setHead(const QByteArray &bytes);
    /*
    与m_u32length相关联,如m_data == nullptr时, m_u32length也置0
    */
    bool setData(const QByteArray &bytes, const unsigned int length);
    bool setData(const unsigned char *data, const unsigned int length);
    void setData(const ::google::protobuf::Message &data);

    inline unsigned char cmdType() const
    {
        return m_ucCmdType;
    }
    inline unsigned char cmdNum() const
    {
        return m_ucCmdNum;
    }
    inline unsigned int sequence() const
    {
        return m_u32Sequence;
    }
    inline unsigned char version() const
    {
        return m_ucVsersion;
    }

    inline const unsigned char* data() const
    {
        return m_data;
    }
    inline unsigned int length() const
    {
        return m_u32length;
    }

private:
    union
    {
        char m_cmdType;
        unsigned char m_ucCmdType;
    };
    union
    {
        char m_cmdNum;
        unsigned char m_ucCmdNum;
    };
    union
    {
        char m_sequence[4];
        unsigned int m_u32Sequence;
    };
    union
    {
        char m_version;
        unsigned char m_ucVsersion;
    };
    union
    {
        char m_length[4];
        unsigned int m_u32length;
    };  //包体长度,也即是m_data的长度,不包括头部长度。头部长度固定为11Bytes
	unsigned char *m_data; 
};

#endif FRAMEBUFFER_H
