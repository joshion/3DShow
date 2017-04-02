#pragma once

class SkeletonFrame
{
public:
    SkeletonFrame();
    ~SkeletonFrame();
    SkeletonFrame(const SkeletonFrame& other);
    SkeletonFrame& operator= (const SkeletonFrame & other);

    static SkeletonFrame fromBytes(const unsigned char *pData, unsigned int uLength);

public:
    short width() const
    {
        return m_Width;
    }

    short height() const
    {
        return m_Height;
    }

    int linesSize() const
    {
        return m_LinesSize;
    }

    unsigned char * pointsSizePerLines() const
    {
        return m_pPointsSizePerLines;
    }

    int elementLength() const
    {
        return m_ElementLength;
    }

    unsigned short* element() const
    {
        return m_pElement;
    }

    int pointSize() const
    {
        return m_DataLength / 2;
    }

    int dataLength() const
    {
        return m_DataLength;
    }

    unsigned short* pointData() const
    {
        return m_pPointsData;
    }

private:
    union
    {
        char m_cWidth[2];
        short m_Width;
    };
    union
    {
        char m_cHeight[2];
        short m_Height;
    };
    unsigned char m_LinesSize;
    unsigned char *m_pPointsSizePerLines;

    unsigned int m_ElementLength;
    unsigned short *m_pElement;

    unsigned int m_DataLength;  // 此值为m_pPointsSizePerLines里各元素值之和
    unsigned short *m_pPointsData;
};

