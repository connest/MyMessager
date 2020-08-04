
#include "Server.h"

#include <QtNetwork>

QVector<ConnectionEntry> connections;


SSLServer::SSLServer(quint16 port, QObject *parent)
    : QTcpServer(parent)
{
    listen(QHostAddress::Any, port);
    qInfo() << "Server started at" << port << "port";
}

void SSLServer::incomingConnection(qintptr socket)
{
    new SSLServerConnection(socket, this);
}

SSLServerConnection::SSLServerConnection(quint16 socketDescriptor,
                                         QObject *parent)
    : QObject(parent)
{

    socket = new QSslSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    socket->setLocalCertificate(
        "C:/Users/Constatine/Desktop/projects/MyMessager/shared/Certifications/key_c.pem");
    socket->setPrivateKey(
        "C:/Users/Constatine/Desktop/projects/MyMessager/shared/Certifications/key_c.key");

    //указываем версию протокола
    this->socket->setProtocol(QSsl::TlsV1_2);



    connect(socket, &QSslSocket::connected,
            this,  &SSLServerConnection::acceptedClient);
    connect(socket, &QSslSocket::disconnected,
            this,  &SSLServerConnection::connectionClosed);
    connect(socket, &QSslSocket::readyRead,
            this,  &SSLServerConnection::readData);
    connect(socket, &QSslSocket::errorOccurred,
            this, &SSLServerConnection::slotError);



    this->socket->startServerEncryption();
}
void SSLServerConnection::sendData(const QByteArray& arr)
{
    socket->write(arr);
}
void SSLServerConnection::slotError(QAbstractSocket::SocketError err)
{
    // The SSL socket conveniently provides human readable error
    // messages through the errorString() call. Note that sometimes
    // the errors come directly from the underlying SSL library, and
    // the quality of the text may vary.
    qDebug("Server error(%d): %s", err,  qPrintable(socket->errorString()));
}
SSLServerConnection::~SSLServerConnection()
{
    qDebug("~SSLServerConnection() Connection closed.");
}

void SSLServerConnection::acceptedClient()
{
    // Provide feedback to the user about incoming connections. This
    // slot is only called if the connection was established, so all
    // communication is now encrypted.
    qDebug("Accepted new client from %s:%d",
           qPrintable(socket->peerAddress().toString()),
           socket->peerPort());

}

void SSLServerConnection::readData()
{
    // First, read all incoming data from the client. The SSL socket
    // has already decrypted it for us. We assume that the client uses
    // a plain text protocol, so we convert the data to a QString.
    QJsonDocument request=QJsonDocument::fromJson(socket->readAll());
    processData(request);


}

void SSLServerConnection::connectionClosed()
{
    // Although the socket may be closing, we must not delete it until
    // the delayed close is done.
    if (socket->state() == QAbstractSocket::ClosingState) {
        connect(socket, &QSslSocket::disconnected, this, &SSLServerConnection::deleteLater);
    } else {
        deleteLater();
        return;
    }

    qDebug("Server: Connection closed.");
}











ConnectionEntry SSLServerConnection::ConnectionEntryFromResponse(const QJsonObject& obj)
{

    return ConnectionEntry(
            jsonValue2ByteArray(obj["name"]),
            jsonValue2ByteArray(obj["client"])
    );

}
QJsonObject SSLServerConnection::WHOIS(QJsonObject& obj)
{

    QJsonObject response;
    ConnectionEntry enrty = ConnectionEntryFromResponse(obj);
    //entry.show();

    auto i=std::find_if(connections.begin(), connections.end(),
        [&](const ConnectionEntry& conn) {
            return conn.getName() == enrty.getClient();
        }
    );

    if(i == std::end(connections))
    {
        connections.push_back(enrty);
        currentEnrty = connections.end() - 1;
        qDebug() << "no such a client";

        response["operation"] = Operations::WAIT;
    }
    else
    {
        qDebug() << " ========== SUCCESS =========";
        //i->show();
        response["operation"] = Operations::CONNECTION;
        response["IP"]=i->getIP().toString();
        response["port"]=i->getPort();
        response["publicKey"]=byteArray2string(i->getPublicKey());

        connections.erase(i);
    }

    return response;
}
void SSLServerConnection::WAIT(QJsonObject& obj)
{
    currentEnrty->setIP(obj["IP"].toString());
    currentEnrty->setPort(obj["port"].toInt());
    currentEnrty->setPublicKey(jsonValue2ByteArray(obj["publicKey"]));

   // qDebug() << "currentEnrty: " << currentEnrty->getName();
}
void SSLServerConnection::processData(QJsonDocument& request)
{
    QJsonObject obj=request.object();
    QJsonDocument response;

    switch(obj["operation"].toInt())
    {
    case Operations::WHOIS:
        response.setObject(WHOIS(obj));
        sendData(response.toJson());
        break;
     case Operations::WAIT:
        WAIT(obj);
        break;
    }

}