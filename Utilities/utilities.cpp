#include "utilities.h"

#include <QString>
#include <QStringList>
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

IP_PORT Utilities::getAdressFromString(QString str)
{
    QStringList strs = str.split(":");

    do
    {
        if (strs.size() != 2)
        {
            break;
        }

        bool ok = false;
        unsigned int port = strs[1].toInt(&ok, 10);
        if (false == ok)
        {
            break;
        }
        return QPair<QString, unsigned int>{strs[0], port};
    } while (false);
    return QPair<QString, unsigned int>{"", 0};
}
