#include "Peer.h"
#include "MyMessagerGlobal.h"
#include <qrsaencryption.h>

Peer::Peer(const QByteArray& clientPublicKey,
           QObject *parent)
    : ConnectionManager(false, parent)
    , m_clientPublicKey{clientPublicKey}
{

    setupConnections();

    connect(this, &ConnectionManager::connectedSignal,
            this, &Peer::onConnected);

    createKeys();
    setGetRaw(true);

}

Peer::Peer(QAbstractSocket* socket,
           const QByteArray& publicKey,
           const QByteArray& privateKey,
           QObject *parent)
    : ConnectionManager(socket, false, parent)
    , m_publicKey{publicKey}
    , m_privateKey{privateKey}
{
    setupConnections();
    setGetRaw(true);
}

void Peer::sendMessage(QString message)
{

    sendEncode({
                   {"type", static_cast<int>(Peer::MessageTypes::MESSAGE)},
                   {"message", message}
               });
}

QByteArray Peer::publicKey() const
{

    return m_publicKey;
}

void Peer::setPublicKey(const QByteArray &publicKey)
{

    m_publicKey = publicKey;
}

void Peer::setPrivateKey(const QByteArray &privateKey)
{

    m_privateKey = privateKey;
}

QByteArray Peer::clientPublicKey() const
{

    return m_clientPublicKey;
}

void Peer::setClientPublicKey(const QByteArray &clientPublicKey)
{

    m_clientPublicKey = clientPublicKey;
}

void Peer::setupConnections()
{

    connect(this, &ConnectionManager::newDataSignalRaw,
            this, &Peer::decodeData);
}

void Peer::createKeys()
{

    QRSAEncryption::generatePairKey(m_publicKey,
                                    m_privateKey,
                                    QRSAEncryption::RSA_2048);
}

void Peer::processData(const QJsonDocument &data)
{


    const QJsonObject& obj{data.object()};

    setClientPublicKey(
                jsonValue2ByteArray(obj["publicKey"])
            );


    switch(static_cast<Peer::MessageTypes>(obj["type"].toInt()))
    {
    case Peer::MessageTypes::INIT_MESSAGE:
        return;
    case Peer::MessageTypes::MESSAGE:
    {
        const QDateTime& time {QDateTime::fromString(obj["time"].toString())};
        emit newMessageSignal(obj["message"].toString(), time);
    }
        break;
    default:
        qWarning() << "Bad message type:" << obj["type"].toInt();
        break;
    }
}

void Peer::sendEncode(const QJsonObject &data)
{

    createKeys();

    QJsonObject obj{data};
    obj["publicKey"] = byteArray2string(m_publicKey);
    obj["time"] = QDateTime::currentDateTimeUtc().toString();


    QByteArray encodedData {QRSAEncryption::encode(QJsonDocument(obj).toJson(),
                                                   m_clientPublicKey,
                                                   QRSAEncryption::RSA_2048)};
    send(encodedData);
}

void Peer::initMessage()
{

    sendEncode({
                   {"type", static_cast<int>(MessageTypes::INIT_MESSAGE)},
                   {"message", "Init"}
               });
}

void Peer::onConnected()
{

    initMessage();
}

void Peer::decodeData(const QByteArray &data)
{

    QByteArray decodedData {QRSAEncryption::decode(data,
                                                   m_privateKey,
                                                   QRSAEncryption::RSA_2048)};
    processData(QJsonDocument::fromJson(decodedData));
}
