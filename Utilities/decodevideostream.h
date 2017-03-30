#pragma once

extern "C"
{
#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
#include <libswscale\swscale.h>
}

#include "thread.h"

#include <opencv2\opencv.hpp>

#include <QList>

class QByteArray;

class DecodeVideoStream : public Thread
{
public:
    DecodeVideoStream();
    virtual ~DecodeVideoStream();
protected:
    void run() override;

public:
    cv::Mat popMat();
    int matsSize();
    void pushBytes(const QByteArray &bytes);
    void pushBytes(const unsigned char* data, unsigned int length);

private:
    void pushMats(const cv::Mat &mat);

private:
    void initDecodec();
    void releaseDecodec();
    void decodeH264();
    void decodeBuffer(const QByteArray &buffer, const int bufferLength);
private:
    std::mutex m_mutexDeocder;

    AVCodecContext *m_pCodecCtx;
    AVCodecParserContext *m_pCodecParserCtx;
    AVCodec *m_pCodec;
    AVFrame *m_pFrame;  // 一帧yuv格式图片
    AVPacket m_Packet;  // h264数据包

    bool m_bFirstTime;  // 判断是否为解码第一帧
    /*
    * 将FFmpeg的yuv图片数据转为cv::Mat的中转缓存,
    * 该类中有2个线程,此变量只能在解码线程中使用
    */
    cv::Mat m_mYUVBuffer;

    /*
    * 将socket接收到H264数据拆分成AVPacket包的中转缓存
    * 该类中有2个线程,此变量只能在解码线程中使用
    */
    QByteArray m_DecodeBytesBuffer;

private:
    QByteArray m_BytesBuffer;
    std::mutex m_mutexBytesBuffer;

    QList<cv::Mat> m_MatsBuffer;
    std::mutex m_mutexMatsBuffer;
};
