#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include <QMessageBox>
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

    void sendData(const QByteArray& data)
    {

        socket->write(data);
        //socket->flush();
    }
public:
    Peer(QString host, quint16 port, QObject* parent = 0)
        : QObject(parent)
    {
        socket = new QTcpSocket(this);
        socket->connectToHost(host, port);
        connect(socket, &QTcpSocket::readyRead, this, &Peer::recv);
        connect(socket, &QTcpSocket::connected, this, &Peer::connectionAccept);
        connect(socket, &QTcpSocket::errorOccurred, this, &Peer::slotError);


    }

    Peer(QTcpSocket* socket, QObject* parent = 0)
        : QObject(parent)
        , socket(socket)
    {
        connect(socket, &QTcpSocket::readyRead, this, &Peer::recv);
        connect(socket, &QTcpSocket::connected, this, &Peer::connectionAccept);
        connect(socket, &QTcpSocket::errorOccurred, this, &Peer::slotError);

    }
    ~Peer()
    {
        qDebug() << "~Peer()";
        if(socket)
            socket->deleteLater();
    }


    void InitMessage()
    {
        QRSAEncryption::generatePairKey(pubKey, privKey, QRSAEncryption::Rsa::RSA_2048);

        QJsonObject initRequest;

        initRequest["operation"] = Operations::INIT_MESSAGE;
        initRequest["publicKey"] = byteArray2string(pubKey);
        initRequest["message"] = "Init message";

        send(QJsonDocument(initRequest).toJson());

    }

    void send(const QByteArray& data)
    {
        sendData(QRSAEncryption::encode(data, clientPubKey, QRSAEncryption::Rsa::RSA_2048));
    }
    QByteArray getPubKey() const;
    void setPubKey(const QByteArray &value);

    QByteArray getPrivKey() const;
    void setPrivKey(const QByteArray &value);

    void setClientPubKey(const QByteArray &value);

    QByteArray getClientPubKey() const;

public slots:

    void slotError(QAbstractSocket::SocketError error)
    {
        qDebug() << this<< " error:" << socket->errorString()<<error;
    }
    void slotSend(const QString& message)
    {
        QRSAEncryption::generatePairKey(pubKey, privKey, QRSAEncryption::Rsa::RSA_2048);

        QJsonObject initRequest;

        initRequest["operation"] = Operations::MESSAGE;
        initRequest["publicKey"] = byteArray2string(pubKey);
        initRequest["message"] = message;

        send(QJsonDocument(initRequest).toJson());
    }
    void recv()
    {
        QByteArray data = QRSAEncryption::decode(socket->readAll(),
                                                 privKey,
                                                 QRSAEncryption::Rsa::RSA_2048);

        QJsonObject request = QJsonDocument::fromJson(data).object();
       // qDebug() << request;

        clientPubKey = jsonValue2ByteArray(request["publicKey"]);

        if(request["operation"] == Operations::INIT_MESSAGE)
        {
            return ;
        }


        //


        emit readSignal(request["message"].toString());
    }
    void connectionClosed()
    {
        if (socket->state() == QAbstractSocket::ClosingState) {
            connect(socket, &QTcpSocket::disconnected, this, &Peer::deleteLater);
        } else {
            deleteLater();
            return;
        }
    }
    void connectionAccept()
    {
        qDebug() << "========== CLIENT ACCEPT ===============";
        emit connectSignal();
    }
signals:
    void connectSignal();
    void readSignal(const QString& message);

};












class Client : public QObject
{
    Q_OBJECT

    QSslSocket* socket = nullptr;

    QString hostSocket;
    int portSocket;

    QString name = "";
    QString password = "";
    QByteArray privKey;
public:
    explicit Client(const QString& host, int port, QObject *parent = nullptr)
        : QObject(parent)
        , hostSocket(host)
        , portSocket(port)
    {
        setupSocket();

    }

    ~Client()
    {
        if(socket)
            socket->deleteLater();
    }
    void setupSocket()
    {
        if (socket)
            return;

        socket = new QSslSocket(this);

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



    void processData(QJsonDocument& request)
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


    void send(const QByteArray& data)
    {
        //qDebug() << "Client send: " << QString(data);
        socket->write(data);
    }
private:

    quint16 setupClientSocket()
    {
        QTcpServer* serv = new QTcpServer(this);
        serv->listen();

        connect(serv, &QTcpServer::newConnection,
                this, &Client::slotNewConnection);

        return serv->serverPort();
    }

public slots:
    void slotNewConnection()
    {
        qDebug() << "========= CONNECT ===========";
        QTcpServer* serv=(QTcpServer*)sender();
        QTcpSocket* s=serv->nextPendingConnection();
        serv->close();
        // serv->deleteLater();

        Peer* p=new Peer(s, this);
        p->setPrivKey(this->privKey);
        this->privKey.clear();

        emit peerCreated(p);

    }











    void setNamePassword(const QString& name, const QString& password)
    {
        this->name=name;
        this->password=password;
    }

    void connect2Peer(const QJsonObject& obj)
    {
        Peer* p=new Peer(obj["IP"].toString(), obj["port"].toInt(), this);
        p->setClientPubKey(jsonValue2ByteArray(obj["publicKey"]));

        p->InitMessage();

        emit peerCreated(p);
    }
    void wait()
    {
        QByteArray pubKey;
        QRSAEncryption::generatePairKey(pubKey, privKey, QRSAEncryption::Rsa::RSA_2048);


        quint16 port=setupClientSocket();


        QJsonObject request;

        request["operation"] = Operations::WAIT;
        request["port"] = port;
        request["IP"] = socket->localAddress().toString();
        request["publicKey"]=byteArray2string(pubKey);

        send(QJsonDocument(request).toJson());
    }
    void whoIs(const QString& clientName, const QString& clientPassword)
    {

        QByteArray hashClient=QCryptographicHash::hash(
                    (clientName+clientPassword).toLatin1(),
                    QCryptographicHash::Algorithm::Sha256
                    );


        QByteArray hashName=QCryptographicHash::hash(
                    (name+password).toLatin1(),
                    QCryptographicHash::Algorithm::Sha256
                    );




        QJsonObject request;
        request["operation"]=Operations::WHOIS;
        request["client"]=byteArray2string(hashClient);
        request["name"]=byteArray2string(hashName);
        //request["port"]=socketClient->serverPort();





        send(QJsonDocument(request).toJson());

    }

    void secureConnect()
    {

#ifdef LOCALHOST_CERTIFICATE
        QSslConfiguration conf;
        conf.addCaCertificates(
                    "C:/Users/Constatine/Desktop/projects/MyMessager/Certifications/key_c.pem"
                    );
        socket->setSslConfiguration(conf);
#endif


        socket->connectToHostEncrypted(hostSocket, portSocket);

    }
    void slotStateChanged(QAbstractSocket::SocketState state)
    {
        //qDebug()<< "Client: " << state;
        Q_UNUSED(state);
    }
    void slotReadyRead()
    {
        QJsonDocument request=QJsonDocument::fromJson(socket->readAll());
        processData(request);
    }
    void slotError(QAbstractSocket::SocketError error)
    {
        qDebug() << "Client error: " << socket->errorString()<<error;
    }
    void sslErrors(const QList<QSslError> &errors)
    {
        for(const auto& err: errors)
            qDebug() << "Client ssl error: "<< err;
    }
signals:
    void peerCreated(Peer* p);
};

#endif // CLIENT_H
