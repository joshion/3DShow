#include "SkeletonFrame.h"

#include <QtEndian>
#include <qDebug>
#include <memory>


SkeletonFrame::SkeletonFrame() 
    :  m_Width(0),
    m_Height(0),
    m_LinesSize(0),
    m_pPointsSizePerLines(nullptr),
    m_pElement(nullptr),
    m_ElementLength(0),
    m_pPointsData(nullptr),
    m_DataLength(0)
{
}


SkeletonFrame::~SkeletonFrame()
{
    if (m_pPointsSizePerLines)
    {
        delete[] m_pPointsSizePerLines;
        m_pPointsSizePerLines = nullptr;
    }
    if (m_pElement)
    {
        delete[] m_pElement;
        m_pElement = nullptr;
    }
    if (m_pPointsData)
    {
        delete[] m_pPointsData;
        m_pPointsData = nullptr;
    }
}

SkeletonFrame::SkeletonFrame(const SkeletonFrame & other)
    : m_Width(other.m_Width),
    m_Height(other.m_Height),
    m_LinesSize(other.m_LinesSize),
    m_pPointsSizePerLines(nullptr),
    m_ElementLength(other.m_ElementLength),
    m_pElement(nullptr),
    m_DataLength(other.m_DataLength),
    m_pPointsData(nullptr)
{
    unsigned char *pOrigPointsSizePerLines = this->m_pPointsSizePerLines;
    short *pOrigElement = this->m_pElement;
    unsigned short *pOrigPointsData = this->m_pPointsData;

    if (other.m_LinesSize > 0 && other.m_pPointsSizePerLines)
    {
        this->m_pPointsSizePerLines = new unsigned char[other.m_LinesSize];
        memcpy(this->m_pPointsSizePerLines, other.m_pPointsSizePerLines,
            other.m_LinesSize * sizeof(unsigned short));
    }
    else
    {
        m_LinesSize = 0;
        this->m_pPointsSizePerLines = nullptr;
    }

    if (other.m_ElementLength > 0 && other.m_pElement)
    {
        this->m_pElement = new short[other.m_ElementLength];
        memcpy(this->m_pElement, other.m_pElement,
            other.m_ElementLength * sizeof(unsigned short));
    }
    else
    {
        this->m_ElementLength = 0;
        this->m_pElement = nullptr;
    }

    if (other.m_DataLength > 0 && other.m_pPointsData)
    {
        this->m_pPointsData = new unsigned short[other.m_DataLength];
        memcpy(this->m_pPointsData, other.m_pPointsData,
            other.m_DataLength * sizeof(unsigned short));
    }
    else
    {
        this->m_DataLength = 0;
        this->m_pPointsData = nullptr;
    }

    if (pOrigPointsSizePerLines)
    {
        delete[] pOrigPointsSizePerLines;
        pOrigPointsSizePerLines = nullptr;
    }
    if (pOrigElement)
    {
        delete[] pOrigElement;
        pOrigElement = nullptr;
    }
    if (pOrigPointsData)
    {
        delete[] pOrigPointsData;
        pOrigPointsData = nullptr;
    }
}

SkeletonFrame & SkeletonFrame::operator=(const SkeletonFrame & other)
{
    this->m_Width = other.m_Width;
    this->m_Height = other.m_Height;
    this->m_LinesSize = other.m_LinesSize;
    this->m_ElementLength = other.m_ElementLength;
    this->m_DataLength = other.m_DataLength;

    unsigned char *pOrigPointsSizePerLines = this->m_pPointsSizePerLines;
    short *pOrigElement = this->m_pElement;
    unsigned short *pOrigPointsData = this->m_pPointsData;

    if (other.m_LinesSize > 0 && other.m_pPointsSizePerLines)
    {
        this->m_pPointsSizePerLines = new unsigned char[other.m_LinesSize];
        memcpy(this->m_pPointsSizePerLines, other.m_pPointsSizePerLines,
            other.m_LinesSize * sizeof(unsigned short));
    }
    else
    {
        m_LinesSize = 0;
        this->m_pPointsSizePerLines = nullptr;
    }

    if (other.m_ElementLength > 0 && other.m_pElement)
    {
        this->m_pElement = new short[other.m_ElementLength];
        memcpy(this->m_pElement, other.m_pElement,
            other.m_ElementLength * sizeof(unsigned short));
    }
    else
    {
        this->m_ElementLength = 0;
        this->m_pElement = nullptr;
    }

    if (other.m_DataLength > 0 && other.m_pPointsData)
    {
        this->m_pPointsData = new unsigned short[other.m_DataLength];
        memcpy(this->m_pPointsData, other.m_pPointsData,
            other.m_DataLength * sizeof(unsigned short));
    }
    else
    {
        this->m_DataLength = 0;
        this->m_pPointsData = nullptr;
    }

    if (pOrigPointsSizePerLines)
    {
        delete[] pOrigPointsSizePerLines;
        pOrigPointsSizePerLines = nullptr;
    }
    if (pOrigElement)
    {
        delete[] pOrigElement;
        pOrigElement = nullptr;
    }
    if (pOrigPointsData)
    {
        delete[] pOrigPointsData;
        pOrigPointsData = nullptr;
    }

    return *this;
}

SkeletonFrame SkeletonFrame::fromBytes(const unsigned char * pData, unsigned int uLength)
{
    SkeletonFrame result;
    if (nullptr == pData || uLength <= 0)
    {
        return SkeletonFrame();
    }
    memcpy(&result.m_cWidth, pData, 2);
    memcpy(&result.m_cHeight, pData + 2, 2);

    result.m_Width = qToBigEndian(result.m_Width);
    result.m_Height = qToBigEndian(result.m_Height);

    memcpy(&result.m_LinesSize, pData + 4, 1);
    result.m_pPointsSizePerLines = new unsigned char[result.m_LinesSize];
    memcpy(result.m_pPointsSizePerLines, pData + 5, result.m_LinesSize);

    result.m_DataLength = 0;
    for (int i = 0; i < result.m_LinesSize; ++i)
    {
        result.m_DataLength += *(result.m_pPointsSizePerLines + i);
    }

    result.m_ElementLength = result.m_LinesSize + result.m_DataLength;
    result.m_pElement = new short[result.m_ElementLength];

    {
        int k = 0;
        int l = 0;
        for (int i = 0; i < result.m_LinesSize; ++i)
        {
            for (int j = 0; j < *(result.m_pPointsSizePerLines + i); ++j)
            {
                *(result.m_pElement + k) = l;
                ++k;
                ++l;
            }
            *(result.m_pElement + k) = -1;
            ++k;
        }
    }

    result.m_DataLength *= 2; // 每个点有x,y两个值,所有需要翻倍
    result.m_pPointsData = new unsigned short[result.m_DataLength];
    memcpy(result.m_pPointsData, pData + 5 + result.m_LinesSize, result.m_DataLength * sizeof(unsigned short));

    for (int i = 0; i < result.m_DataLength; ++i)
    {
        *(result.m_pPointsData + i) = qToBigEndian(*(result.m_pPointsData + i));
    }

    return result;
}
