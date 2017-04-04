#pragma once

class QVector4D;

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

    unsigned int elementSize() const
    {
        return m_ElementSize;
    }

    const short *const elementData() const
    {
        return m_pElementData;
    }

    unsigned int pointSize() const
    {
        return m_PointSize;
    }

    const QVector4D *const pointData()  const
    {
        return m_pPointData;
    }

private:
    short m_Width;
    short m_Height;

    unsigned int m_ElementSize;
    short *m_pElementData;

    unsigned int m_PointSize;
    QVector4D* m_pPointData;
};

