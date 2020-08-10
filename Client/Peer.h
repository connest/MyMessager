#ifndef PEER_H
#define PEER_H

#include "ConnectionManager.h"
#include <QJsonDocument>
#include <QJsonObject>

class Peer : public ConnectionManager
{
    Q_OBJECT
public:

    explicit Peer(const QByteArray& clientPublicKey,
                  QObject *parent = nullptr);

    explicit Peer(QAbstractSocket* socket,
                  const QByteArray& publicKey,
                  const QByteArray& privateKey,
                  QObject *parent = nullptr);

    void sendMessage(QString message);


    QByteArray publicKey() const;
    void setPublicKey(const QByteArray &publicKey);

    void setPrivateKey(const QByteArray &privateKey);

    QByteArray clientPublicKey() const;
    void setClientPublicKey(const QByteArray &clientPublicKey);

signals:
    void newMessageSignal(QString message, QDateTime time);
private:

    QByteArray m_publicKey;
    QByteArray m_privateKey;
    QByteArray m_clientPublicKey;

    enum class MessageTypes {
        INIT_MESSAGE = 1,
        MESSAGE,
        ECHO
    };

    void setupConnections();
    void createKeys();
    void processData(const QJsonDocument& data);
    void sendEncode(const QJsonObject& data);
    void initMessage();

private slots:
    void onConnected();
    void decodeData(const QByteArray& data);
};

#endif // PEER_H
