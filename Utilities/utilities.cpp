#include "utilities.h"

#include <QString>
#include <QFile>

QString Utilities::readStringFromFile(const QString & fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly))
    {
        return file.readAll();
    }
    else
    {
        return QString {};
    }
}
