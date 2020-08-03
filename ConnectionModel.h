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
    quint16 port;
public:
    ConnectionEntry(const QByteArray& name      = QByteArray(),
                    const QByteArray& client    = QByteArray(),
                    const QByteArray& publicKey = QByteArray(),
                    const QHostAddress& IP      = QHostAddress(),
                    const quint16& port             = 0,
                    QObject* parent             = nullptr);
    ConnectionEntry(const ConnectionEntry& entry, QObject* parent=nullptr);

    QByteArray getName() const {return name;}
    QByteArray getPublicKey() const {return publicKey;}

    QByteArray getClient() const {return client;}
    QHostAddress getIP() const {return IP;}
    int getPort() const {return port;}

    ConnectionEntry &operator=(const ConnectionEntry& entry);

    void show();
    void setPublicKey(const QByteArray &value);
    void setName(const QByteArray &value);
    void setClient(const QByteArray &value);
    void setIP(const QHostAddress &value);
    void setIP(const QString &value);
    void setPort(quint16 value);
};

Q_DECLARE_METATYPE(ConnectionEntry);




#endif // CONNECTIONMODEL_H
