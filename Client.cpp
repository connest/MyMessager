#include "Client.h"



QByteArray Peer::getPrivKey() const
{
    return privKey;
}

void Peer::setPrivKey(const QByteArray &value)
{
    privKey = value;
}

void Peer::setClientPubKey(const QByteArray &value)
{
    clientPubKey = value;
}

QByteArray Peer::getClientPubKey() const
{
    return clientPubKey;
}

QByteArray Peer::getPubKey() const
{
    return pubKey;
}

void Peer::setPubKey(const QByteArray &value)
{
    pubKey = value;
}


quint16 Client::setupClientSocket()
{
    serv = new QTcpServer(this);
    serv->listen();

    connect(serv, &QTcpServer::newConnection,
            this, &Client::slotNewConnection);

    return serv->serverPort();
}

void Client::setupSocket()
{
    if (socket)
        return;

    socket = new QSslSocket();

    connect(socket, &QSslSocket::stateChanged,
            this, &Client::slotStateChanged);

    connect(socket, &QSslSocket::errorOccurred,
            this, &Client::slotError);

    connect(socket,
            QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
            this, &Client::sslErrors);

    connect(socket, &QSslSocket::readyRead,
            this, &Client::slotReadyRead);
}

void Client::processData(QJsonDocument &request)
{
    QJsonObject obj=request.object();

    switch(obj["operation"].toInt())
    {
    case Operations::WAIT:
        wait();
        socket->close();
        break;
    case Operations::CONNECTION:
        connect2Peer(obj);
        socket->close();
        break;
    }
}

void Client::send(const QByteArray &data)
{
    socket->write(data);
}

void Client::send(const QJsonObject &obj)
{
    send(QJsonDocument(obj).toJson());
}


void Client::slotNewConnection()
{
    QTcpServer* serv=(QTcpServer*)sender();
    QTcpSocket* s=serv->nextPendingConnection();
    serv->close();

    Peer* p=new Peer(s, this->privKey, this);

    this->privKey.clear();

    emit peerCreated(p);
}

void Client::slotStateChanged(QAbstractSocket::SocketState state)
{
    qDebug()<< "Client: " << state;
}

void Client::slotReadyRead()
{
    QJsonDocument request=QJsonDocument::fromJson(socket->readAll());
    processData(request);
}

void Client::slotError(QAbstractSocket::SocketError error)
{
    qDebug() << "Client error: " << socket->errorString()<<error;
}

void Client::sslErrors(const QList<QSslError> &errors)
{
    for(const auto& err: errors)
        qDebug() << "Client ssl error: "<< err;
}

Client::Client(const QString &host, int port, QObject *parent)
    : QObject(parent)
    , host(host)
    , port(port)
{
    setupSocket();
}

Client::Client(QObject *parent)
    : QObject(parent)
{
    setupSocket();
}

Client::~Client()
{
    if(socket)
        socket->deleteLater();


    if(serv)
        serv->deleteLater();

    deleteLater();
}

void Client::secureConnect()
{
    QSslConfiguration conf;

#ifdef LOCALHOST_CERTIFICATE
    conf.addCaCertificates(
                "C:/Users/Constatine/Desktop/projects/MyMessager/Certifications/key_c.pem"
                );
#elif
    conf = QSslConfiguration::defaultConfiguration();
#endif

    socket->setSslConfiguration(conf);
    socket->connectToHostEncrypted(host, port);
}

void Client::setNamePassword(const QString &name, const QString &password)
{
    this->name=name;
    this->password=password;
}

void Client::setHostPort(const QString &hostname, quint16 port)
{
    this->host = hostname;
    this->port=port;
}

void Client::connect2Peer(const QJsonObject &obj)
{
    Peer* p=new Peer(
            obj["IP"].toString(),
            obj["port"].toInt(),
            jsonValue2ByteArray(obj["publicKey"]),
            this
    );

    emit peerCreated(p);
}

void Client::whoIs(const QString &clientName, const QString &clientPassword)
{

    QByteArray hashClient=QCryptographicHash::hash(
                (clientName+clientPassword).toLatin1(),
                QCryptographicHash::Algorithm::Sha256
                );


    QByteArray hashName=QCryptographicHash::hash(
                (name+password).toLatin1(),
                QCryptographicHash::Algorithm::Sha256
                );


    QJsonObject request = {
        {"operation",   Operations::WHOIS},
        {"client",      byteArray2string(hashClient)},
        {"name",        byteArray2string(hashName)}
    };

    send(request);

}

void Client::wait()
{
    QByteArray pubKey;
    QRSAEncryption::generatePairKey(pubKey, privKey, QRSAEncryption::Rsa::RSA_2048);


    quint16 port=setupClientSocket();


    QJsonObject request={
        {"operation",   Operations::WAIT},
        {"port",        port},
        {"IP",          socket->localAddress().toString()},
        {"publicKey",   byteArray2string(pubKey)}
    };

    send(request);
}
