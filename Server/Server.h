#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QJsonObject>

#include "MyMessagerGlobal.h"

class SSLServerConnection : public QObject
{
    Q_OBJECT

public:
    SSLServerConnection(quint16 socket, QObject *parent = 0);
    ~SSLServerConnection();
private slots:
    void acceptedClient();
    void readData();
    void connectionClosed();
    void slotError(QAbstractSocket::SocketError err);

private:
    QSslSocket *socket;
    void send(const QByteArray& arr);

    void processData(QJsonObject obj);

    void whoIs(QJsonObject obj);

    void send(const QJsonObject &data);
    void registration(QJsonObject obj);

    void declareQAbstractSocketSocketError();
};


class SSLServer : public QTcpServer
{
    Q_OBJECT

public:
    SSLServer(quint16 port, QObject *parent = 0);

    void incomingConnection(qintptr socket);
};



#endif // SERVER_H
