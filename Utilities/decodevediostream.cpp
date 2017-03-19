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

    initDecodec();
}


DecodeVedioStream::~DecodeVedioStream()
{
    releaseDecodec();
}

void DecodeVedioStream::run()
{
    decodeH264();
}

void DecodeVedioStream::pushBytes(const QByteArray & bytes)
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
    m_BytesBuffer.append(bytes);
}

void DecodeVedioStream::pushMats(const cv::Mat & mat)
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexMatsBuffer);
    m_MatsBuffer.push_back(mat);
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

void DecodeVedioStream::initDecodec()
{
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
    av_free_packet(&m_Packet);
    av_frame_free(&m_pFrame);
    avpicture_free(&m_Picture);
    sws_freeContext(m_pSwsCtx);
    avcodec_free_context(&m_pCodecCtx);
    av_parser_close(m_pCodecParserCtx);
}

void DecodeVedioStream::decodeH264()
{
    {
        std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
        uint8_t *buffer_ptr = (uint8_t*) m_BytesBuffer.data(); // 缓冲区的数据
        int buffer_size = m_BytesBuffer.size();   // 缓冲区数据长度
        if (buffer_size > 0)
        {
            /* 返回解析了的字节数 */
            int len = av_parser_parse2(m_pCodecParserCtx, m_pCodecCtx, &m_Packet.data, &m_Packet.size,
                buffer_ptr, buffer_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);
            m_BytesBuffer.remove(0, len);
        }
    }


    if (m_Packet.size >= 0)
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
            if (m_bIsFirstTime)	// 分配格式转换存储空间
            {

                m_pSwsCtx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt,
                    m_pCodecCtx->width, m_pCodecCtx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

                avpicture_alloc(&m_Picture, AV_PIX_FMT_RGB32, m_pCodecCtx->width, m_pCodecCtx->height);

                m_bIsFirstTime = false;
            }
            /* YUV转RGB */
            sws_scale(m_pSwsCtx, m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height, m_Picture.data, m_Picture.linesize);

            cv::Mat mat = cv::Mat(m_pCodecCtx->width, m_pCodecCtx->height, CV_8SC3, m_Picture.data[0]).inv();
            this->pushMats(mat);
        }
    }
}
