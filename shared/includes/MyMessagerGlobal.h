#ifndef MYMESSAGERGLOBAL_H
#define MYMESSAGERGLOBAL_H

enum Operations {
    WHOIS,
    REGISTRATION,
    NO_SUCH_CLIENT,
    CONNECT_TO_CLIENT
};


#include <QByteArray>
#include <QJsonValue>
QByteArray jsonValue2ByteArray(const QJsonValue& obj);
QString byteArray2string(const QByteArray& arr);
#endif // MYMESSAGERGLOBAL_H
