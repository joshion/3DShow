#pragma once
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class QByteArray;

class FrameBuffer
{
public:
    FrameBuffer();
	FrameBuffer(unsigned int len, unsigned char *data);
	~FrameBuffer();

public:
    static QByteArray toByte(const FrameBuffer &buffer);
    static FrameBuffer fromByte(const QByteArray &bytes);

public:
	union
	{
		char m_length[4];
		int m_u32length;
	};  //���峤��,Ҳ����m_data�ĳ���,������ͷ�����ȡ�ͷ�����ȹ̶�Ϊ11Bytes
    char m_cmdType;
    char m_cmdNum;
    union
    {
        char m_sequence[4];
        int m_u32Sequence;
    };
	char m_version;
	char *m_data;
};

#endif FRAMEBUFFER_H
