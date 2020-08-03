#include "Peer.h"


void Peer::send(const QByteArray &data)
{
    socket->write(QRSAEncryption::encode(data,
                                         clientPubKey,
                                         QRSAEncryption::Rsa::RSA_2048));
}

void Peer::send(const QJsonObject &obj)
{
    send(QJsonDocument(obj).toJson());
}

void Peer::InitMessage()
{
    QRSAEncryption::generatePairKey(pubKey,
                                    privKey,
                                    QRSAEncryption::Rsa::RSA_2048);

    QJsonObject initRequest = {
        {"operation", Operations::INIT_MESSAGE},
        {"publicKey", byteArray2string(pubKey)},
        {"message", "Init message"}
    };

    send(initRequest);

    emit connectSignal();

}

void Peer::setupSocket()
{
    connect(socket, &QTcpSocket::readyRead, this, &Peer::recv);
    connect(socket, &QTcpSocket::errorOccurred, this, &Peer::slotError);
}

Peer::Peer(QString host, quint16 port, QByteArray clientPubKey, QObject *parent)
    : QObject(parent)
    , clientPubKey(clientPubKey)
{
    socket = new QTcpSocket(this);
    setupSocket();

    socket->connectToHost(host, port);
    InitMessage();
}

Peer::Peer(QTcpSocket *socket, QByteArray privKey, QObject *parent)
    : QObject(parent)
    , socket(socket)
    , privKey(privKey)
{
    setupSocket();
}

Peer::~Peer()
{
    if(socket)
        socket->deleteLater();
}

void Peer::slotError(QAbstractSocket::SocketError error)
{
    qDebug() << this<< " error:" << socket->errorString()<<error;
}

void Peer::connectionClosed()
{
    if (socket->state() == QAbstractSocket::ClosingState) {
        connect(socket, &QTcpSocket::disconnected,
                this, &Peer::deleteLater);
    } else {
        deleteLater();
        return;
    }
}

void Peer::recv()
{
    QByteArray data = QRSAEncryption::decode(socket->readAll(),
                                             privKey,
                                             QRSAEncryption::Rsa::RSA_2048);

    QJsonObject request = QJsonDocument::fromJson(data).object();

    clientPubKey = jsonValue2ByteArray(request["publicKey"]);

    if(request["operation"] == Operations::INIT_MESSAGE) {
        emit connectSignal();
        return ;
    }

    emit readSignal(request["message"].toString());
}

void Peer::slotSend(const QString &message)
{
    QRSAEncryption::generatePairKey(pubKey,
                                    privKey,
                                    QRSAEncryption::Rsa::RSA_2048);

    QJsonObject initRequest = {
        {"operation",   Operations::MESSAGE},
        {"publicKey",   byteArray2string(pubKey)},
        {"message",     message}
    };

    send(initRequest);
}
