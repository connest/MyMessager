#ifndef CONNECTIONMODEL_H
#define CONNECTIONMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QTextStream>
#include <QDebug>
#include <QList>
#include <QtNetwork>

class ConnectionEntry;
QDebug operator<<(QDebug debug, const ConnectionEntry &entry);

class ConnectionEntry : public QObject {
    Q_GADGET

    QByteArray client;
    QByteArray name;
    QByteArray publicKey;
    QHostAddress IP;
    int port;
public:
    ConnectionEntry(const QByteArray& name = QByteArray(),
                    const QByteArray& client = QByteArray(),
                    const QByteArray& publicKey = QByteArray(),
                    const QHostAddress& IP = QHostAddress(),
                    const int& port = 0,
                    QObject* parent=nullptr)
        : QObject(parent)
        , client(client)
        , name(name)
        , publicKey(publicKey)
        , IP(IP)
        , port(port)
    {

    }
    ConnectionEntry(const ConnectionEntry& entry, QObject* parent=nullptr):
        QObject(parent)
    {
        this->client=entry.client;
        this->publicKey=entry.publicKey;
        this->name=entry.name;
        this->IP=entry.IP;
        this->port=entry.port;
    }

    QByteArray getName() const {return name;}
    QByteArray getPublicKey() const {return publicKey;}

    QByteArray getClient() const {return client;}
    QHostAddress getIP() const {return IP;}
    int getPort() const {return port;}

    ConnectionEntry& operator=(const ConnectionEntry& entry)
    {
        client      =entry.client;
        publicKey   =entry.publicKey;
        name        =entry.name;
        IP          =entry.IP;
        port        =entry.port;
        return *this;
    }

    void show()
    {
        qDebug() << (*this);
    }
    void setPublicKey(const QByteArray &value);
    void setName(const QByteArray &value);
    void setClient(const QByteArray &value);
    void setIP(const QHostAddress &value);
    void setIP(const QString &value)
    {
        IP.setAddress(value);
    }
    void setPort(int value);
};

Q_DECLARE_METATYPE(ConnectionEntry);




#endif // CONNECTIONMODEL_H
