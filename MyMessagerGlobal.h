#ifndef MYMESSAGERGLOBAL_H
#define MYMESSAGERGLOBAL_H

enum Operations {
    WHOIS,
    WAIT,
    CONNECTION,
    INIT_MESSAGE,
    MESSAGE
};

#include <QByteArray>
#include <QJsonValue>
QByteArray jsonValue2ByteArray(const QJsonValue& obj);
QString byteArray2string(const QByteArray& arr);
#endif // MYMESSAGERGLOBAL_H
