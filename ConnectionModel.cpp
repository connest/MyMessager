#include "ConnectionModel.h"

QDebug operator<<(QDebug debug, const ConnectionEntry &entry)
{
    QDebugStateSaver saver(debug);
    debug.nospace()
            << "ConnectionEntry(" << &entry << ")\n"
            << "name: "<<  entry.getName() << "\n"
            << "client: "<<  entry.getClient() << "\n"
            << "key: "<<  entry.getPublicKey() << "\n"
            << "IP: "<<  entry.getIP() << "\n"
            << "port: "<<  entry.getPort() << "\n\n"
            ;
    return debug;
}

void ConnectionEntry::setName(const QByteArray &value)
{
    name = value;
}

void ConnectionEntry::setClient(const QByteArray &value)
{
    client = value;
}

void ConnectionEntry::setIP(const QHostAddress &value)
{
    IP = value;
}

void ConnectionEntry::setPort(int value)
{
    port = value;
}

void ConnectionEntry::setPublicKey(const QByteArray &value)
{
    publicKey = value;
}
