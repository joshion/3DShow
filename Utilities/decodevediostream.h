#pragma once

extern "C"
{
#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
#include <libswscale\swscale.h>
}

#include <opencv2\opencv.hpp>

#include <QList>

#include "thread.h"

class QByteArray;

class DecodeVedioStream : public Thread
{
public:
    DecodeVedioStream();
    virtual ~DecodeVedioStream();
protected:
    void run() override;

public:
    cv::Mat popMats();
    int matsSize();
    void pushBytes(const QByteArray &bytes);
    void pushBytes(const unsigned char* data, unsigned int length);

private:
    void pushMats(const cv::Mat &mat);

private:
    void initDecodec();
    void releaseDecodec();
    void decodeH264();
private:
    std::mutex m_mutexDeocder;

    AVCodecContext *m_pCodecCtx;
    AVCodecParserContext *m_pCodecParserCtx;
    AVCodec *m_pCodec;
    AVFrame *m_pFrame;  // һ֡yuv��ʽͼƬ
    AVPacket m_Packet;  // h264���ݰ�

    bool m_bFirstTime;  // �ж��Ƿ�Ϊ�����һ֡
    cv::Mat m_mYUVBuffer;   // ��FFmpeg��yuvͼƬ����תΪcv::Mat����ת����

    QByteArray m_DecodeBytesBuffer; // ��socket���յ�H264���ݲ�ֳ�AVPacket������ת����
    void decodeBuffer(const QByteArray &buffer, const int bufferLength);

private:
    QByteArray m_BytesBuffer;
    std::mutex m_mutexBytesBuffer;

    QList<cv::Mat> m_MatsBuffer;
    std::mutex m_mutexMatsBuffer;
};

