#ifndef UTILITIES_H
#define UTILITIES_H

#pragma once

class QString;

class Utilities
{
public:
    Utilities() = delete;

public:
    static QString readStringFromFile(const QString &fileName);
};

#endif // !UTILITIES_H
