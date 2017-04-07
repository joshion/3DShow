#include "utilities.h"

#include <QString>
#include <QByteArray>
#include <QFile>

#include <memory.h>

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
    str.remove(QChar('-'));
    QByteArray buffer;
    buffer.append(str);
    buffer = buffer.fromHex(buffer);

    QString ip = QString::number(static_cast<unsigned char>(buffer[0])) + "."
        + QString::number(static_cast<unsigned char>(buffer[1])) + "."
        + QString::number(static_cast<unsigned char>(buffer[2])) + "."
        + QString::number(static_cast<unsigned char>(buffer[3]));

    unsigned short port = 0;
    memcpy(&port, buffer.data() + 4, 2);

    return QPair<QString, unsigned int>{ip, port};
}
