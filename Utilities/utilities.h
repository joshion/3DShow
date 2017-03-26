#ifndef UTILITIES_H
#define UTILITIES_H

#pragma once

class QString;


class Utilities
{
public:
    Utilities() = delete;

public:
    enum class ShowType : unsigned char
    {
        Color = 0x1,
        Depth = 0x2,
        Skele = 0x4,
        Color_Depth = 0x3,
        Color_Skele = 0x5,
        Depth_Skele = 0x6,
        Color_Depth_Color = 0x7
    };

    enum class SocketType : unsigned char
    {
        Color = 0x1,
        Depth = 0x2,
        Skele = 0x4
    };
    static QString readStringFromFile(const QString &fileName);
};

static bool operator& (const Utilities::ShowType& lh, const Utilities::ShowType & rh)
{
    return static_cast<unsigned char> (lh) &  static_cast<unsigned char> (rh);
}

#endif // !UTILITIES_H
