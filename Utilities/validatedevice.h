#pragma once


class QString;
class QByteArray;

class ValidateDevice
{
public:
    ValidateDevice(const unsigned char length = 0, const unsigned char* data = nullptr);
    virtual ~ValidateDevice();
    ValidateDevice(const ValidateDevice &other);
    ValidateDevice& operator=(const ValidateDevice & other);

public:
    static QByteArray toByte(const ValidateDevice &other);

public:
    inline void setLength(unsigned char length)
    {
        m_ucLength = length;
    }

    bool setData(const unsigned char *data = nullptr, unsigned char length = 0);
    bool setData(const QByteArray& bytes, unsigned char length = 0);
    void setData(const QString &data);

    inline unsigned char length() const
    {
        return m_ucLength;
    }
    inline const unsigned char* data() const
    {
        return m_data;
    }

    inline unsigned int headLength() const
    {
        return s_u32HeadLength;
    }
private:
    union
    {
        char m_Length;
        unsigned char m_ucLength;
    };
    unsigned char *m_data;
    
    static const unsigned int s_u32HeadLength = 1;
};

