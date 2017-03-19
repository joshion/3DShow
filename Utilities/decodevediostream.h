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
    void pushBytes(const QByteArray &bytes);
    void pushMats(const cv::Mat &mat);
    cv::Mat popMats();
    int matsSize();

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
    AVCodecContext *m_pCodecCtx = NULL;
    AVCodecParserContext *m_pCodecParserCtx = NULL;
    AVCodec *m_pCodec = NULL;
    AVFrame *m_pFrame = NULL;             //yuv  
    AVPacket m_Packet;                    //h264  
    AVPicture m_Picture;                  //¥¢¥Êrgb∏Ò ΩÕº∆¨  
    SwsContext *m_pSwsCtx = NULL;
    AVCodecID m_Codec_Id = AV_CODEC_ID_H264;

    bool m_bIsFirstTime = true;

};

