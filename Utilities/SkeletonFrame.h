#pragma once
class SkeletonFrame
{
public:
    SkeletonFrame();
    ~SkeletonFrame();

    static SkeletonFrame fromBytes(const unsigned char *pData, unsigned int uLength);
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
    unsigned char m_LinesSizes;
    unsigned char *m_pPointsSizePerLines;

    short *m_pElement;
    unsigned int m_ElementLength;

    unsigned short *m_pPointsData;
    unsigned int m_DataLength;  // 此值为m_pPointsSizePerLines里各元素值之和
};

