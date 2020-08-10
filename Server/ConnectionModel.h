#ifndef CONNECTIONMODEL_H
#define CONNECTIONMODEL_H

#include <QObject>
#include <QtNetwork>
#include <map>
#include <optional>

class Connections {
public:
    class ConnectionsEntry
    {
    public:
        ConnectionsEntry() = default;
        ConnectionsEntry(QHostAddress info, quint16 port,  QByteArray publicKey);
        ~ConnectionsEntry() = default;

        QHostAddress info() const;
        void setInfo(const QHostAddress &info);

        QByteArray publicKey() const;
        void setPublicKey(const QByteArray &publicKey);

        quint16 port() const;
        void setPort(const quint16 &port);


    private:
        QHostAddress m_info;
        quint16 m_port;
        QByteArray m_publicKey;
    };

    static Connections& instance();

    bool add(QByteArray client, QString IP, quint16 port, QByteArray publicKey);
    std::pair<ConnectionsEntry, bool> pop(QByteArray client);

    size_t count() const;
private:
    Connections();
    Connections(Connections& conn);
    Connections& operator=(const Connections& conn);
    ~Connections();

    // key: client name + password hash
    static std::map<QByteArray, ConnectionsEntry> m_entries;
};






#endif // CONNECTIONMODEL_H
