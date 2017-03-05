#pragma once
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class QByteArray;

class FrameBuffer
{
public:
    FrameBuffer(FrameBuffer &other);
    FrameBuffer(unsigned int len = 0, unsigned char cmdType = 0, unsigned char cmdNum = 0,
        unsigned int sequence = 0, unsigned char version = 0, unsigned char * data = nullptr);
	~FrameBuffer();

public:
    static QByteArray toByte(const FrameBuffer &buffer);
    static FrameBuffer fromByte(const QByteArray &bytes);

public:
    //void setLength(unsigned int length);
    //void setCmdType(unsigned char cmdType);
    //void setCmdNum(unsigned char cmdNum);
    //void setSequence(unsigned int sequence);
    //void setVersion(unsigned char version);
    //void setData(unsigned char *data, unsigned int length);
private:
	union
	{
		char m_length[4];
        unsigned int m_u32length;
	};  //包体长度,也即是m_data的长度,不包括头部长度。头部长度固定为11Bytes
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
	unsigned char *m_data;  //与m_u32length想关联,如m_data == nullptr时,m_u32length必须为0
};

#endif FRAMEBUFFER_H
