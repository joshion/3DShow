#include "decodevediostream.h"

#include <QByteArray>
#include <Qdebug>

namespace
{
    static const unsigned int BYTES_BUFFER_RESERVED_SIZE = 409600;
    static const unsigned int MATS_BUFFER_RESERVED_SIZE = 24 * 2;
    static const unsigned int DECODE_BUFFER_SIZE = 4096;
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
    notifyThreadToContinue();
}

void DecodeVedioStream::pushBytes(const unsigned char * data, unsigned int length)
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
    if (data != nullptr && length > 0)
    {
        m_BytesBuffer.append((char *) data, length);
    }
    notifyThreadToContinue();
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

    /* ��ʼ��AVCodec */
    m_pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);

    /* ��ʼ��AVCodecContext,ֻ�Ƿ��䣬��û�� */
    m_pCodecCtx = avcodec_alloc_context3(m_pCodec);

    /* ��ʼ��AVCodecParserContext */
    m_pCodecParserCtx = av_parser_init(AV_CODEC_ID_H264);
    if (!m_pCodecParserCtx)
    {
        qDebug() << "AVCodecParseContext error";
        ::exit(0);
    }

    /* ʲô��˼ */
    //if (m_pCodec->capabilities & CODEC_CAP_TRUNCATED)
    //{
    //    m_pCodecCtx->flags |= CODEC_FLAG_TRUNCATED;
    //}

    /* �򿪽����� */
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

    av_packet_unref(&m_Packet);
    av_frame_free(&m_pFrame);
    avcodec_free_context(&m_pCodecCtx);
    av_parser_close(m_pCodecParserCtx);
}

void DecodeVedioStream::decodeH264()
{
    int buffer_size = 0;
    {
        std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
        buffer_size = m_BytesBuffer.size();   // ���������ݳ���
    }
    while (buffer_size > 0)
    {
        QByteArray temp {};
        {
            std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
            if (buffer_size > DECODE_BUFFER_SIZE)
            {
                temp.append(m_BytesBuffer.data(), DECODE_BUFFER_SIZE);
                temp.append(AV_INPUT_BUFFER_PADDING_SIZE, '0');
                m_BytesBuffer.remove(0, DECODE_BUFFER_SIZE);
            }
            else
            {
                temp.append(m_BytesBuffer.data(), buffer_size);
                temp.append(AV_INPUT_BUFFER_PADDING_SIZE, '0');
                m_BytesBuffer.remove(0, buffer_size);
            }
        }
        decodeBuffer(temp);

        {
            std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
            buffer_size = m_BytesBuffer.size();   // ���������ݳ���
        }
    }
}

/*
����buffer�Ѿ�����av_parser_parse2��Ҫ�õ���AV_INPUT_BUFFER_PADDING_SIZE���ֽ����Ļ���
*/
void DecodeVedioStream::decodeBuffer(const QByteArray & buffer)
{
    int currentLen = buffer.size() - AV_INPUT_BUFFER_PADDING_SIZE;
    uint8_t *currentPtr = (uint8_t*) buffer.data(); // ������������

    while (currentLen > 0)
    {
        /* ���ؽ����˵��ֽ��� */
        int len = av_parser_parse2(m_pCodecParserCtx, m_pCodecCtx, &m_Packet.data, &m_Packet.size,
            currentPtr, currentLen, AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

        currentPtr += len;
        currentLen -= len;

        if (m_Packet.size > 0)
        {
            int got = 0;
            /* �������, �жϳ��� */
            avcodec_send_packet(m_pCodecCtx, &m_Packet);
            if (avcodec_receive_frame(m_pCodecCtx, m_pFrame) == 0)  // ����0ʱ�ɹ������ݽ��뵽m_pFrame��
            {
                /* YUV420P��ʽ
                * Y:V:U = 4:1:1
                * ���ݷֱ����3��ͨ����
                */
                if (m_pCodecCtx->pix_fmt == AV_PIX_FMT_YUV420P)
                {
                    int height = m_pFrame->height;
                    int width = m_pFrame->width;

                    /*
                    mYUV����һ������ת����ת����ʱ����,
                    ������ҪΪstatic����
                    ��mRGB��Ҫ���͵�GUI��Ⱦ����copy������ǳ����,
                    ����ÿ�ζ���Ҫһ���µ�mBGR�����洢�µ�ͼƬ
                    */
                    static cv::Mat mYUV(height * 3 / 2, width, CV_8UC1);
                    cv::Mat mBGR(height, width, CV_8UC3);

                    /* ���� Y ���� */
                    memcpy(mYUV.data,
                        (unsigned char *) m_pFrame->data[0],
                        height * width * sizeof(unsigned char));

                    /* ���� V ���� */
                    memcpy(mYUV.data + height * width * sizeof(unsigned char),
                        (unsigned char *) m_pFrame->data[1],
                        height / 4 * width * sizeof(unsigned char));

                    /* ���� U ���� */
                    memcpy(mYUV.data + height * 5 / 4 * width * sizeof(unsigned char),
                        (unsigned char *) m_pFrame->data[2],
                        height / 4 * width * sizeof(unsigned char));

                    cv::cvtColor(mYUV, mBGR, CV_YUV2BGR_I420);
                    this->pushMats(mBGR);
                }
            }
            else
            {
                qDebug() << "decodec error";
                ::exit(0);
            }
        }
        av_packet_unref(&m_Packet);
    }
}
