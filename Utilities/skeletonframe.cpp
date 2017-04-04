#include "skeletonframe.h"

#include <QVector4D>
#include <QtEndian>
#include <qDebug>
#include <memory>


SkeletonFrame::SkeletonFrame() 
    :  m_Width(0),
    m_Height(0),
    m_ElementSize(0),
    m_pElementData(nullptr),
    m_PointSize(0),
    m_pPointData(nullptr)
{
}


SkeletonFrame::~SkeletonFrame()
{

    if (m_pElementData)
    {
        delete[] m_pElementData;
        m_pElementData = nullptr;
    }
    if (m_pPointData)
    {
        delete[] m_pPointData;
        m_pPointData = nullptr;
    }
}

SkeletonFrame::SkeletonFrame(const SkeletonFrame & other)
    : m_Width(other.m_Width),
    m_Height(other.m_Height),
    m_ElementSize(other.m_ElementSize),
    m_pElementData(nullptr),
    m_PointSize(other.m_PointSize),
    m_pPointData(nullptr)
{
    short *pOrigElement = this->m_pElementData;
    QVector4D *pOrigPointData = this->m_pPointData;

    if (other.m_ElementSize > 0 && other.m_pElementData)
    {
        this->m_pElementData = new short[other.m_ElementSize];
        memcpy(this->m_pElementData, other.m_pElementData, other.m_ElementSize * sizeof(short));
    }
    else
    {
        this->m_ElementSize = 0;
        this->m_pElementData = nullptr;
    }

    if (other.m_PointSize > 0 && other.m_pPointData)
    {
        this->m_pPointData = new QVector4D[other.m_PointSize];
        memcpy(this->m_pPointData, other.m_pPointData, other.m_PointSize * sizeof(QVector4D));
    }
    else
    {
        this->m_PointSize = 0;
        this->m_pPointData = nullptr;
    }

    if (pOrigElement)
    {
        delete[] pOrigElement;
        pOrigElement = nullptr;
    }
    if (pOrigPointData)
    {
        delete[] pOrigPointData;
        pOrigPointData = nullptr;
    }
}

SkeletonFrame & SkeletonFrame::operator=(const SkeletonFrame & other)
{
    this->m_Width = other.m_Width;
    this->m_Height = other.m_Height;
    this->m_ElementSize = other.m_ElementSize;
    this->m_PointSize = other.m_PointSize;

    short *pOrigElement = this->m_pElementData;
    QVector4D *pOrigPointData = this->m_pPointData;

    if (other.m_ElementSize > 0 && other.m_pElementData)
    {
        this->m_pElementData = new short[other.m_ElementSize];
        memcpy(this->m_pElementData, other.m_pElementData, other.m_ElementSize * sizeof(short));
    }
    else
    {
        this->m_ElementSize = 0;
        this->m_pElementData = nullptr;
    }

    if (other.m_PointSize > 0 && other.m_pPointData)
    {
        this->m_pPointData = new QVector4D[other.m_PointSize];
        memcpy(this->m_pPointData, other.m_pPointData, other.m_PointSize * sizeof(QVector4D));
    }
    else
    {
        this->m_PointSize = 0;
        this->m_pPointData = nullptr;
    }

    if (pOrigElement)
    {
        delete[] pOrigElement;
        pOrigElement = nullptr;
    }
    if (pOrigPointData)
    {
        delete[] pOrigPointData;
        pOrigPointData = nullptr;
    }

    return *this;
}

/*
* 数据的完整性需要由外部保证
* 该函数默认传进来的数据是完整的一帧
* 不做数据的出错处理
*/
SkeletonFrame SkeletonFrame::fromBytes(const unsigned char * pData, unsigned int uLength)
{
    if (nullptr == pData || uLength <= 0)
    {
        return SkeletonFrame();
    }
    SkeletonFrame result;

    /* 解析出图像的 宽 高 */
    memcpy(&result.m_Width, pData, 2);
    memcpy(&result.m_Height, pData + 2, 2);
    result.m_Width = qToBigEndian(result.m_Width);
    result.m_Height = qToBigEndian(result.m_Height);

    /* 解析出线段的段数 */
    unsigned char linesSize = 0;
    memcpy(&linesSize, pData + 4, 1);

    /* 解析出各线段分别包含的点数 */
    unsigned char* pPointsSizePerLines = new unsigned char[linesSize];
    memcpy(pPointsSizePerLines, pData + 5, linesSize);

    /* 全部线段包含点的总数 */
    int pointSize = 0;
    for (int i = 0; i < linesSize; ++i)
    {
        pointSize += *(pPointsSizePerLines + i);
    }
    result.m_PointSize = pointSize;
    /* 如被解析的数据长度不够, 抛出异常*/
    if (5 + linesSize + pointSize * 2 * sizeof(unsigned short) > uLength)
    {
        throw("out of range");
    }

    /*
    * 计算出opengl绘图索引,
    * 各线段之间以-1 隔开
    */
    result.m_ElementSize = linesSize + pointSize;
    result.m_pElementData = new short[result.m_ElementSize];
    {
        int k = 0;
        int l = 0;
        for (int i = 0; i < linesSize; ++i)
        {
            for (int j = 0; j < *(pPointsSizePerLines + i); ++j)
            {
                *(result.m_pElementData + k) = l;
                ++k;
                ++l;
            }
            *(result.m_pElementData + k) = -1;
            ++k;
        }
    }

    // 每个点有x,y两个值,所有需要翻倍
    unsigned short* pPointsData = new unsigned short[pointSize * 2];

    memcpy(pPointsData, pData + 5 + linesSize, pointSize * 2 * sizeof(unsigned short));
    for (int i = 0; i < pointSize; ++i)
    {
        /* 大端字节转为小端 */
        float x = static_cast<float>(qToBigEndian(*(pPointsData + i)));
        float y = static_cast<float>(qToBigEndian(*(pPointsData + 2 * i + 1)));

        float width = result.m_Width / 2;
        float height = result.m_Height / 2;

        x = (x - width) / width;
        y = (y - height) / height;

        *(result.m_pPointData + i) = QVector4D { x, y, -3.0, 1.0 };
    }
    delete pPointsSizePerLines;
    pPointsSizePerLines = nullptr;
    delete pPointsData;
    pPointsData = nullptr;

    return result;
}
