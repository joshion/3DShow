#include "decodevideostream.h"

#include <QByteArray>

#include <Qdebug>

namespace
{
    static const unsigned int BYTES_BUFFER_RESERVED_SIZE = 40960000;
    static const unsigned int MATS_BUFFER_RESERVED_SIZE = 24 * 2;
    static const unsigned int DECODE_BUFFER_SIZE = 4096;
    static const unsigned int DECODE_BUFFER_TOTAL_SIZE = DECODE_BUFFER_SIZE + AV_INPUT_BUFFER_PADDING_SIZE;
}

DecodeVideoStream::DecodeVideoStream()
    : m_pCodecCtx(nullptr),
    m_pCodecParserCtx(nullptr),
    m_pCodec(nullptr),
    m_pFrame(nullptr),
    m_bFirstTime(true),
    m_DecodeBytesBuffer(DECODE_BUFFER_TOTAL_SIZE, '0')
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
    // this->start();
}

DecodeVideoStream::~DecodeVideoStream()
{
    // this->stop();
    this->releaseDecodec();
}

void DecodeVideoStream::run()
{
    std::lock_guard<std::mutex> lock_decode(m_mutexDeocder);
    decodeH264();
}

cv::Mat DecodeVideoStream::popMat()
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexMatsBuffer);
    if (m_MatsBuffer.isEmpty())
    {
        return cv::Mat();
    }
    else
    {
        // qDebug() << __FILE__ << __LINE__ << m_MatsBuffer.size();
        cv::Mat mat = m_MatsBuffer.first();
        m_MatsBuffer.pop_front();
        return mat;
    }
}

int DecodeVideoStream::matsSize()
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexMatsBuffer);
    return m_MatsBuffer.size();
}

void DecodeVideoStream::pushBytes(const QByteArray & bytes)
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
    m_BytesBuffer.append(bytes);
    notifyThreadToContinue();
}

void DecodeVideoStream::pushBytes(const unsigned char * data, unsigned int length)
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
    if (data != nullptr && length > 0)
    {
        m_BytesBuffer.append((char *) data, length);
    }
    notifyThreadToContinue();
}

void DecodeVideoStream::pushMats(const cv::Mat & mat)
{
    std::lock_guard<std::mutex> lock_buffer(m_mutexMatsBuffer);
    m_MatsBuffer.push_back(mat);
}

void DecodeVideoStream::initDecodec()
{
    std::lock_guard<std::mutex> lock_decode(m_mutexDeocder);
    av_register_all();
    avcodec_register_all();

    /* 初始化AVCodec */
    m_pCodec = avcodec_find_decoder(AV_CODEC_ID_H264);

    /* 初始化AVCodecContext,只是分配，还没打开 */
    m_pCodecCtx = avcodec_alloc_context3(m_pCodec);

    /* 初始化AVCodecParserContext */
    m_pCodecParserCtx = av_parser_init(AV_CODEC_ID_H264);
    if (!m_pCodecParserCtx)
    {
        qDebug() << "AVCodecParseContext error";
        ::exit(0);
    }

    /*
    * 使得视频解码得到的图片完整
    * 但需要有下一帧的数据才可以解码完上一帧的图片
    */
    if (m_pCodec->capabilities & CODEC_CAP_TRUNCATED)
    {
        m_pCodecCtx->flags |= CODEC_FLAG_TRUNCATED;
    }

    /* 打开解码器 */
    if (avcodec_open2(m_pCodecCtx, m_pCodec, nullptr) < 0)
    {
        qDebug() << "avocodec_open2 error";
        ::exit(0);
    }

    m_pFrame = av_frame_alloc();

    av_init_packet(&m_Packet);
    m_Packet.size = 0;
    m_Packet.data = nullptr;
}

void DecodeVideoStream::releaseDecodec()
{
    std::lock_guard<std::mutex> lock_decode(m_mutexDeocder);

    av_packet_unref(&m_Packet);
    av_frame_free(&m_pFrame);
    avcodec_free_context(&m_pCodecCtx);
    av_parser_close(m_pCodecParserCtx);
}

void DecodeVideoStream::decodeH264()
{
    int buffer_size = 0;
    {
        std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
        buffer_size = m_BytesBuffer.size();   // 缓冲区数据长度
    }
    while (buffer_size > 0)
    {
        if (buffer_size > DECODE_BUFFER_SIZE)
        {
            {
                std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
                m_DecodeBytesBuffer.replace(0, DECODE_BUFFER_SIZE, m_BytesBuffer.data(), DECODE_BUFFER_SIZE);
                m_BytesBuffer.remove(0, DECODE_BUFFER_SIZE);
            }
            decodeBuffer(m_DecodeBytesBuffer, DECODE_BUFFER_SIZE);
        }
        else
        {
            {
                std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
                m_DecodeBytesBuffer.replace(0, buffer_size, m_BytesBuffer.data(), buffer_size);
                m_BytesBuffer.remove(0, buffer_size);
            }
            decodeBuffer(m_DecodeBytesBuffer, buffer_size);
        }

        {
            std::lock_guard<std::mutex> lock_buffer(m_mutexBytesBuffer);
            buffer_size = m_BytesBuffer.size();   // 缓冲区数据长度
            // qDebug() << __FILE__ << __LINE__ << buffer_size;
        }
    }
}

/*
buffer的末尾已经加了av_parser_parse2需要用到的AV_INPUT_BUFFER_PADDING_SIZE个字节数的缓存
bufferLength的值最大为buufer.length() - AV_INPUT_BUFFER_PADDING_SIZE
*/
void DecodeVideoStream::decodeBuffer(const QByteArray & buffer, const int bufferLength)
{
    uint8_t *currentPtr = (uint8_t*) buffer.data();
    int currentLen = bufferLength;

    while (currentLen > 0)
    {
        /* 返回解析了的字节数 */
        int len = av_parser_parse2(m_pCodecParserCtx, m_pCodecCtx, &m_Packet.data, &m_Packet.size,
            currentPtr, currentLen, AV_NOPTS_VALUE, AV_NOPTS_VALUE, AV_NOPTS_VALUE);

        currentPtr += len;
        currentLen -= len;
        // qDebug() << __FILE__ << __LINE__ << currentLen;
        if (m_Packet.size > 0)
        {
            if (avcodec_send_packet(m_pCodecCtx, &m_Packet) == 0    // 返回0时成功把数据放到解码器中
                && avcodec_receive_frame(m_pCodecCtx, m_pFrame) == 0)  // 返回0时成功把数据解码到m_pFrame中
            {
                /* YUV420P格式
                * Y:V:U = 4:1:1
                * 数据分别放在3个通道中
                */
                if (m_pCodecCtx->pix_fmt == AV_PIX_FMT_YUV420P)
                {
                    int height = m_pFrame->height;
                    int width = m_pFrame->width;

                    /* 转换第一帧时,需要依据当前视频每帧图片大小对m_mYUVBuffer分配内存 */
                    if (m_bFirstTime)
                    {
                        m_mYUVBuffer.create(height * 3 / 2, width, CV_8UC1);
                        m_bFirstTime = false;
                    }

                    /* 复制 Y 分量 */
                    memcpy(m_mYUVBuffer.data,
                        (unsigned char *) m_pFrame->data[0],
                        height * width * sizeof(unsigned char));

                    /* 复制 V 分量 */
                    memcpy(m_mYUVBuffer.data + height * width * sizeof(unsigned char),
                        (unsigned char *) m_pFrame->data[1],
                        height / 4 * width * sizeof(unsigned char));

                    /* 复制 U 分量 */
                    memcpy(m_mYUVBuffer.data + height * 5 / 4 * width * sizeof(unsigned char),
                        (unsigned char *) m_pFrame->data[2],
                        height / 4 * width * sizeof(unsigned char));

                    /*
                    * mRGB需要发送到GUI渲染且其copy函数是浅拷贝,
                    * 所以每次都需要一个新的mBGR变量存储新的图片
                    */
                    cv::Mat mBGR(height, width, CV_8UC3);
                    cv::cvtColor(m_mYUVBuffer, mBGR, CV_YUV2BGR_I420);
                    this->pushMats(mBGR);
                }
            }
            /* 解码出错, 退出程序 */
            else
            {
                qDebug() << "decodec error";
                ::exit(0);
            }
        }
        av_packet_unref(&m_Packet);
    }
}