#ifndef PEER_H
#define PEER_H


#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include "MyMessagerGlobal.h"

#include <QJsonObject>
#include <QJsonDocument>

#include <qrsaencryption.h>

class Peer : public QObject
{
    Q_OBJECT
    QTcpSocket* socket = nullptr;

    QByteArray clientPubKey;

    QByteArray pubKey;
    QByteArray privKey;


    void send(const QByteArray& data);

    void send(const QJsonObject& obj);

    void InitMessage();
    void setupSocket();
public:
    Peer(QString host,
         quint16 port,
         QByteArray clientPubKey,
         QObject* parent = 0);

    Peer(QTcpSocket* socket,
         QByteArray privKey,
         QObject* parent = 0);
    ~Peer();

    QByteArray getPubKey() const;
    void setPubKey(const QByteArray &value);

    QByteArray getPrivKey() const;
    void setPrivKey(const QByteArray &value);

    QByteArray getClientPubKey() const;
    void setClientPubKey(const QByteArray &value);

private slots:
    void slotError(QAbstractSocket::SocketError error);
    void connectionClosed();
    void recv();
public slots:

    void slotSend(const QString& message);
signals:
    void connectSignal();
    void readSignal(const QString& message);
};


#endif // PEER_H
