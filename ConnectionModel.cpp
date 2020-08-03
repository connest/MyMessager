#include "ConnectionModel.h"

QDebug operator<<(QDebug debug, const ConnectionEntry &entry)
{
    QDebugStateSaver saver(debug);
    debug.nospace()
            << "ConnectionEntry("   << &entry               << ")\n"
            << "name: "             << entry.getName()      << "\n"
            << "client: "           << entry.getClient()    << "\n"
            << "key: "              << entry.getPublicKey() << "\n"
            << "IP: "               << entry.getIP()        << "\n"
            << "port: "             << entry.getPort()      << "\n\n"
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

void ConnectionEntry::setIP(const QString &value)
{
    IP.setAddress(value);
}

void ConnectionEntry::setPort(quint16 value)
{
    port = value;
}

ConnectionEntry::ConnectionEntry(const QByteArray &name, const QByteArray &client, const QByteArray &publicKey, const QHostAddress &IP, const quint16 &port, QObject *parent)
    : QObject(parent)
    , client(client)
    , name(name)
    , publicKey(publicKey)
    , IP(IP)
    , port(port)
{

}

ConnectionEntry::ConnectionEntry(const ConnectionEntry &entry, QObject *parent):
    QObject(parent)
{
    this->client    =entry.client;
    this->publicKey =entry.publicKey;
    this->name      =entry.name;
    this->IP        =entry.IP;
    this->port      =entry.port;
}

ConnectionEntry &ConnectionEntry::operator=(const ConnectionEntry &entry)
{
    client      =entry.client;
    publicKey   =entry.publicKey;
    name        =entry.name;
    IP          =entry.IP;
    port        =entry.port;
    return *this;
}

void ConnectionEntry::show()
{
    qDebug() << (*this);
}

void ConnectionEntry::setPublicKey(const QByteArray &value)
{
    publicKey = value;
}
