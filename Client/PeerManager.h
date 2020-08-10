#ifndef PEERMANAGER_H
#define PEERMANAGER_H

#include <QObject>
#include <memory>
#include <QtNetwork>
#include "Peer.h"

class PeerManager : public QObject
{
    Q_OBJECT
public:
    PeerManager(QObject* parent = nullptr);
    ~PeerManager();
    bool connectToPeer(QString IP, quint16 port, QByteArray publicKey);

    // returns port & publicKey
    std::pair<quint16, QByteArray> listenToPeer(bool& result);
    void send(QString message);
signals:

    void onConnect();
    void onNewMessage(QString message, QDateTime time);

private:
    QTcpServer m_server;
    std::unique_ptr<Peer> m_peer;

    QByteArray m_publicKey;
    QByteArray m_privateKey;

    void createKeys();

private slots:
    void onNewConnection();
};

#endif // PEERMANAGER_H
