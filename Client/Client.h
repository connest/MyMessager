#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include "MyMessagerGlobal.h"

#include <QJsonObject>
#include <QJsonDocument>

#include <qrsaencryption.h>

#include "Peer.h"





class Client : public QObject
{
    Q_OBJECT

    QSslSocket* socket = nullptr;
    QTcpServer* serv = nullptr;

    QString host;
    int port;

    QString name = "";
    QString password = "";
    QByteArray privKey;
public:
    explicit Client(const QString& host, int port, QObject *parent = nullptr);
    explicit Client(QObject *parent = 0);
    ~Client();


    void setNamePassword(const QString& name, const QString& password);
    void setHostPort(const QString& hostname, quint16 port);
public slots:
    void secureConnect();
    void connect2Peer(const QJsonObject& obj);
    void whoIs(const QString& clientName, const QString& clientPassword);

    void isHostOnline();
signals:
    void peerCreated(Peer* p);
    void errorSignal(net::Errors);


private:
    quint16 setupClientSocket();
    void setupSocket();
    void processData(QJsonDocument& request);
    void send(const QByteArray& data);
    void send(const QJsonObject& obj);
    void wait();

private slots:
    void slotNewConnection();


    void slotStateChanged(QAbstractSocket::SocketState state);
    void slotReadyRead();
    void slotError(QAbstractSocket::SocketError error);
    void sslErrors(const QList<QSslError> &errors);
};


#endif // CLIENT_H
