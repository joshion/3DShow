#ifndef UTILITIES_H
#define UTILITIES_H

#pragma once

class QString;

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

static bool operator&(const ShowType& lh, const ShowType & rh)
{
    return static_cast<unsigned char> (lh) &  static_cast<unsigned char> (rh);
}

enum class SocketType : unsigned char
{
    Color = 0x1,
    Depth = 0x2,
    Skele = 0x4
};

class Utilities
{
public:
    Utilities() = delete;

public:
    static QString readStringFromFile(const QString &fileName);
};

#endif // !UTILITIES_H
