#include "decodevediostream.h"

#include <QVector>
#include <QByteArray>
#include <Qdebug>

#pragma warning(disable:4996)   // 使用废除的函数


namespace
{
    static const unsigned int BYTES_BUFFER_RESERVED_SIZE = 4096;
    static const unsigned int MATS_BUFFER_RESERVED_SIZE = 24 * 2;
}

DecodeVedioStream::DecodeVedioStream()
{
    {
        std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
        m_BytesBuffer.reserve(BYTES_BUFFER_RESERVED_SIZE);
    }
    {
        std::lock_guard<std::mutex> lock_buffer(m_mutexMatsBuffer);
        m_MatsBuffer.reserve(MATS_BUFFER_RESERVED_SIZE);
    }

    this->initDecodec();
    this->start();
}


DecodeVedioStream::~DecodeVedioStream()
{
    this->stop();
    this->releaseDecodec();
}

void DecodeVedioStream::run()
{
    std::lock_guard<std::mutex> lock_decode(m_mutexDeocder);
    decodeH264();
}

cv::Mat DecodeVedioStream::popMats()
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexMatsBuffer);
    if (m_MatsBuffer.isEmpty())
    {
        return cv::Mat();
    }
    else
    {
        cv::Mat mat = m_MatsBuffer.first();
        m_MatsBuffer.pop_front();
        return mat;
    }
}

int DecodeVedioStream::matsSize()
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexMatsBuffer);
    return m_MatsBuffer.size();
}

void DecodeVedioStream::pushBytes(const QByteArray & bytes)
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
    m_BytesBuffer.append(bytes);
}

void DecodeVedioStream::pushBytes(const unsigned char * data, unsigned int length)
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
    if (data != nullptr && length > 0)
    {
        m_BytesBuffer.append((char *) data, length);
    }
}

void DecodeVedioStream::pushMats(const cv::Mat & mat)
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexMatsBuffer);
    m_MatsBuffer.push_back(mat);
}

void DecodeVedioStream::initDecodec()
{
    std::lock_guard<std::mutex> lock_decode(m_mutexDeocder);
    av_register_all();
    avcodec_register_all();

    /* 初始化AVCodec */
    m_pCodec = avcodec_find_decoder(m_Codec_Id);

    /* 初始化AVCodecContext,只是分配，还没打开 */
    m_pCodecCtx = avcodec_alloc_context3(m_pCodec);

    /* 初始化AVCodecParserContext */
    m_pCodecParserCtx = av_parser_init(AV_CODEC_ID_H264);
    if (!m_pCodecParserCtx)
    {
        qDebug() << "AVCodecParseContext error";
        ::exit(0);
    }

    /* 什么意思 */
    if (m_pCodec->capabilities & CODEC_CAP_TRUNCATED)
    {
        m_pCodecCtx->flags |= CODEC_FLAG_TRUNCATED;
    }

    /* 打开解码器 */
    if (avcodec_open2(m_pCodecCtx, m_pCodec, NULL) < 0)
    {
        qDebug() << "avocodec_open2 error";
        ::exit(0);
    }

    m_pFrame = av_frame_alloc();

    av_init_packet(&m_Packet);
    m_Packet.size = 0;
    m_Packet.data = NULL;
}

void DecodeVedioStream::releaseDecodec()
{
    std::lock_guard<std::mutex> lock_decode(m_mutexDeocder);

    av_free_packet(&m_Packet);
    av_frame_free(&m_pFrame);
    avcodec_free_context(&m_pCodecCtx);
    av_parser_close(m_pCodecParserCtx);
}

void DecodeVedioStream::decodeH264()
{
    {
        std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
        int buffer_size = m_BytesBuffer.size();   // 缓冲区数据长度
        if (buffer_size > 0)
        {
            uint8_t *buffer_ptr = (uint8_t*) m_BytesBuffer.data(); // 缓冲区的数据
            /* 返回解析了的字节数 */
            int len = av_parser_parse2(m_pCodecParserCtx, m_pCodecCtx, &m_Packet.data, &m_Packet.size,
                buffer_ptr, buffer_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);
            m_BytesBuffer.remove(0, len);
        }
    }

    if (m_Packet.size > 0)
    {
        int got = 0;
        /* 解码出错, 中断程序 */
        if (avcodec_decode_video2(m_pCodecCtx, m_pFrame, &got, &m_Packet) < 0)
        {
            qDebug() << "decodec error";
            ::exit(0);
        }

        if (got)
        {
            cv::Mat mYUV(m_pFrame->height, m_pFrame->width, CV_8UC1, m_pFrame->data);
            cv::Mat mRGB(m_pFrame->height, m_pFrame->width, CV_8UC3);
            cv::cvtColor(mYUV, mRGB, CV_YUV2BGR_I420);

            this->pushMats(mRGB);
        }
    }
}
