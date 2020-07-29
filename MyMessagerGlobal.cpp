#include "MyMessagerGlobal.h"

QByteArray jsonValue2ByteArray(const QJsonValue& obj)
{
    return QByteArray::fromBase64(obj.toString().toLatin1());
}
QString byteArray2string(const QByteArray& arr)
{
    return QString(arr.toBase64()).toLatin1();
}
