#pragma once

#include <google/protobuf/message.h>

class QByteArray;

class OrderFrameBuffer
{
public:
    OrderFrameBuffer(unsigned char cmdType = 0, unsigned char cmdNum = 0, unsigned int sequence = 0,
        unsigned char version = 0, unsigned int len = 0, unsigned char * data = nullptr);
    OrderFrameBuffer(const OrderFrameBuffer &other);
    ~OrderFrameBuffer();

    OrderFrameBuffer& operator=(const OrderFrameBuffer& other);

public:
    static QByteArray toBytes(const OrderFrameBuffer &buffer);
    static OrderFrameBuffer fromBytes(const QByteArray &bytes);

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
    ͷ�ĳ���Ϊ11,��bytes�ĳ�������Ҫ��11
    �ڴ�������ͷ��,��������˰��峤��,��ʱdata�������,���ǰ��峤�Ȳ���
    */
    bool setHead(const QByteArray &bytes);
    /*
    ��m_u32length�����,��m_data == nullptrʱ, m_u32lengthҲ��0
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
        return m_u32Length;
    }

    inline unsigned int headLength() const
    {
        return s_u32HeadLength;
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
        unsigned int m_u32Length;
    };  //���峤��,Ҳ����m_data�ĳ���,������ͷ�����ȡ�ͷ�����ȹ̶�Ϊ11Bytes
    unsigned char *m_data;

    static const unsigned int s_u32HeadLength = 11;

public:
    const static unsigned int TYPE_CONNECT_PROTOCOL = 1;

    const static unsigned int NUM_REQUIRE_CONNECT = 1;
    const static unsigned int NUM_EXIT_CONNECT = 2;
    const static unsigned int NUM_REQUIRE_DEVICES = 3;
    const static unsigned int NUM_RESP_REQUIRE_CONNECT = 100;
    const static unsigned int NUM_RESP_DEVICES = 101;

    const static unsigned int TYPE_KINECT_PROTOCOL = 2;

    const static unsigned int NUM_START_REQUIRE = 1;
    const static unsigned int NUM_END_REQUIRE = 2;
    const static unsigned int NUM_RESP_START_REQUIRE = 100;
    const static unsigned int NUM_SERVER_END_TRANSFER = 101;

    const static unsigned int TYPE_DATA_CHANNEL_PROTOCOL = 3;
    const static unsigned int NUM_VALIDATE_PORT = 1;
};
