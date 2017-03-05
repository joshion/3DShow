#pragma once
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class QByteArray;

class FrameBuffer
{
public:
    FrameBuffer();
    FrameBuffer(FrameBuffer &other);
    FrameBuffer(unsigned int len, unsigned char cmdType, unsigned char cmdNum,
        unsigned int sequence, unsigned char version, unsigned char *data);
	~FrameBuffer();

public:
    static QByteArray toByte(const FrameBuffer &buffer);
    static FrameBuffer fromByte(const QByteArray &bytes);

public:
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
	unsigned char *m_data;
};

#endif FRAMEBUFFER_H
