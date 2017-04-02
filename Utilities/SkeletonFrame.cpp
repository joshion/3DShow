#include "SkeletonFrame.h"

#include <QtEndian>
#include <qDebug>
#include <memory>


SkeletonFrame::SkeletonFrame() 
    :  m_Width(0),
    m_Height(0),
    m_LinesSizes(0),
    m_pPointsSizePerLines(nullptr),
    m_pElement(nullptr),
    m_ElementLength(0),
    m_pPointsData(nullptr),
    m_DataLength(0)
{
}


SkeletonFrame::~SkeletonFrame()
{
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

    memcpy(&result.m_LinesSizes, pData + 4, 1);
    result.m_pPointsSizePerLines = new unsigned char[result.m_LinesSizes];
    memcpy(result.m_pPointsSizePerLines, pData + 5, result.m_LinesSizes);

    result.m_DataLength = 0;
    for (int i = 0; i < result.m_LinesSizes; ++i)
    {
        result.m_DataLength += *(result.m_pPointsSizePerLines + i);
        qDebug() << *(result.m_pPointsSizePerLines + i);
    }

    result.m_ElementLength = result.m_LinesSizes + result.m_DataLength;
    result.m_pElement = new short[result.m_ElementLength];

    {
        int k = 0;
        int l = 0;
        for (int i = 0; i < result.m_LinesSizes; ++i)
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

    for (int i = 0; i < result.m_ElementLength; ++i)
    {
        qDebug() << *(result.m_pElement + i);
    }

    result.m_DataLength *= 2; // 每个点有x,y两个值,所有需要翻倍
    result.m_pPointsData = new unsigned short[result.m_DataLength];
    memcpy(result.m_pPointsData, pData + 5 + result.m_LinesSizes, result.m_DataLength * sizeof(unsigned short));

    for (int i = 0; i < result.m_DataLength; ++i)
    {
        *(result.m_pPointsData + i) = qToBigEndian(*(result.m_pPointsData + i));
        qDebug() << *(result.m_pPointsData + i);
    }

    return result;
}
