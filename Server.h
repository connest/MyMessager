#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include <QMessageBox>

#include <QJsonObject>
#include <QJsonDocument>

#include "ConnectionModel.h"
#include "MyMessagerGlobal.h"

class SSLServerConnection : public QObject
{
    Q_OBJECT

public:
    SSLServerConnection(quint16 socket, QObject *parent = 0);
    ~SSLServerConnection();
    void sendData(const QByteArray& arr);
public slots:
    void acceptedClient();
    void readData();
    void connectionClosed();
    void slotError(QAbstractSocket::SocketError err);

private:
    void processData(QJsonDocument& doc);
    QJsonObject WHOIS(QJsonObject& obj);
    void WAIT(QJsonObject& obj);
    ConnectionEntry ConnectionEntryFromResponse(const QJsonObject& obj);

    QSslSocket *socket;
    QVector<ConnectionEntry>::iterator currentEnrty ;
};


class SSLServer : public QTcpServer
{
    Q_OBJECT

public:
    SSLServer(quint16 port, QObject *parent = 0);

    void incomingConnection(qintptr socket);
};



#endif // SERVER_H
