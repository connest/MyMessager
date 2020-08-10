#include "ConnectionModel.h"
#include <QHostInfo>
#include <QDebug>





std::map<QByteArray, Connections::ConnectionsEntry> Connections::m_entries;

Connections &Connections::instance()
{
    static Connections c{};
    return c;
}

bool Connections::add(QByteArray client, QString IP, quint16 port, QByteArray publicKey)
{
    ConnectionsEntry entry{QHostAddress{IP}, port, publicKey};
    auto insertResult = m_entries.insert({client, entry});

    /*if(insertResult.second)
    {
        qDebug() << "Add element: {\n"
                 << "\tkey:" << client
                 << "\n\tvalue: {\n"
                 << "\t\tIP:" << IP
                 << "\n\t\tport:" << port
                 << "\n\t\tpublicKey:" << publicKey
                 << "\n\t}\n}";
    }*/

    return insertResult.second;
}

std::pair<Connections::ConnectionsEntry, bool> Connections::pop(QByteArray client)
{
    auto entry = m_entries.find(client);

    if(entry != m_entries.end())
    {
        Connections::ConnectionsEntry e = entry->second;

        m_entries.erase(entry);

        return {
            e, true
        };
    }

    return {
        {}, false
    };
}

size_t Connections::count() const
{
    return m_entries.size();
}

Connections::Connections()
{

}

Connections::Connections(Connections &conn)
{
    Q_UNUSED(conn);
}

Connections &Connections::operator=(const Connections &conn)
{
    Q_UNUSED(conn)
    return *this;
}

Connections::~Connections()
{

}











QHostAddress Connections::ConnectionsEntry::info() const
{
    return m_info;
}

void Connections::ConnectionsEntry::setInfo(const QHostAddress &info)
{
    m_info = info;
}

QByteArray Connections::ConnectionsEntry::publicKey() const
{
    return m_publicKey;
}

void Connections::ConnectionsEntry::setPublicKey(const QByteArray &publicKey)
{
    m_publicKey = publicKey;
}

quint16 Connections::ConnectionsEntry::port() const
{
    return m_port;
}

void Connections::ConnectionsEntry::setPort(const quint16 &port)
{
    m_port = port;
}

Connections::ConnectionsEntry::ConnectionsEntry(QHostAddress info,
                                                quint16 port,
                                                QByteArray publicKey)
    : m_info{info}
    , m_port{port}
    , m_publicKey{publicKey}
{

}
