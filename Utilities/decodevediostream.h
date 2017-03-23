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
    QByteArray m_BytesBuffer;
    std::mutex m_mutexBytesBuffer;

    QList<cv::Mat> m_MatsBuffer;
    std::mutex m_mutexMatsBuffer;

private:
    void initDecodec();
    void releaseDecodec();
    void decodeH264();
private:
    std::mutex m_mutexDeocder;
    AVCodecContext *m_pCodecCtx = NULL;
    AVCodecParserContext *m_pCodecParserCtx = NULL;
    AVCodec *m_pCodec = NULL;
    // AVCodecID m_Codec_Id = AV_CODEC_ID_H264;

    AVPacket m_Packet;                    //h264
    AVFrame *m_pFrame = NULL;             //yuv

    void decodeBuffer(const QByteArray &buffer);
};

