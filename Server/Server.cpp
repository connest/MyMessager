
#include "Server.h"

#include <QtNetwork>
#include <optional>
#include <QJsonDocument>

#include "ConnectionModel.h"

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
    this->socket->setProtocol(QSsl::TlsV1_3);

    declareQAbstractSocketSocketError();

    connect(socket, &QSslSocket::connected,
            this,  &SSLServerConnection::acceptedClient);
    connect(socket, &QSslSocket::disconnected,
            this,  &SSLServerConnection::connectionClosed);
    connect(socket, &QSslSocket::readyRead,
            this,  &SSLServerConnection::readData);
    connect(socket, &QSslSocket::errorOccurred,
            this, &SSLServerConnection::slotError);



    this->socket->startServerEncryption();

    qInfo() << "New connection from " << socket->peerAddress()<<socket->peerPort();
    qInfo() << "Entry count:"<<Connections::instance().count();
}
void SSLServerConnection::send(const QByteArray& arr)
{
    //qDebug() << "Server send:" <<arr;
    socket->write(arr);
}
void SSLServerConnection::send(const QJsonObject& data)
{
    send(QJsonDocument(data).toJson());
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
    socket->deleteLater();
}

void SSLServerConnection::acceptedClient()
{
    qDebug("Accepted new client from %s:%d",
           qPrintable(socket->peerAddress().toString()),
           socket->peerPort());

}

void SSLServerConnection::readData()
{
    QJsonDocument request=QJsonDocument::fromJson(socket->readAll());
    processData(request.object());


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

}

void SSLServerConnection::declareQAbstractSocketSocketError()
{
    static std::once_flag QAbstractSocketSocketErrorDeclared;
    std::call_once(QAbstractSocketSocketErrorDeclared, [](){
        qRegisterMetaType<QAbstractSocket::SocketError>();
    });
}








void SSLServerConnection::processData(QJsonObject request)
{

    switch(request["operation"].toInt())
    {
    case Operations::WHOIS:
        whoIs(request);
        break;
    case Operations::REGISTRATION:
        registration(request);
        break;
    }

}

void SSLServerConnection::whoIs(QJsonObject obj)
{
    const QByteArray client = jsonValue2ByteArray(obj["client"]);

    Connections& c = Connections::instance();
    auto result = c.pop(client);

    if(result.second)
    {
        Connections::ConnectionsEntry value{result.first};
        send({
                 {"operation",  Operations::CONNECT_TO_CLIENT},
                 {"IP",         value.info().toString()},
                 {"port",       value.port()},
                 {"publicKey",  byteArray2string(value.publicKey())}
        });
    }
    else
    {
        send({
                 {"operation",  Operations::NO_SUCH_CLIENT}
        });
    }
}

void SSLServerConnection::registration(QJsonObject obj)
{
    Connections& c = Connections::instance();

    const QByteArray client     = jsonValue2ByteArray(obj["client"]);
    const QByteArray publicKey  = jsonValue2ByteArray(obj["publicKey"]);

    bool result =
            c.add(client,
                  socket->peerAddress().toString(),
                  obj["port"].toInt(),
                  publicKey
                  );


    send({
             {"operation", Operations::REGISTRATION},
             {"result", result}
         });
}


/*


QVector<ConnectionEntry> connections;

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
*/

