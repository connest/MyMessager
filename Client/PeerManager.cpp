#include "PeerManager.h"
#include <qrsaencryption.h>

PeerManager::PeerManager(QObject *parent)
    : QObject{parent}
    , m_server{nullptr}
    , m_peer{nullptr}
{

}

PeerManager::~PeerManager()
{
    if(m_server)
    {
        m_server->close();
        m_server->deleteLater();
    }
    deleteLater();
}

bool PeerManager::connectToPeer(QString IP, quint16 port, QByteArray publicKey)
{

    if(!m_peer)
        m_peer = new Peer(publicKey);
    m_peer->init();

    m_peer->connectToHost(IP, port);

    connect(m_peer, &Peer::newMessageSignal,
            this, &PeerManager::onNewMessage);

   /* if(m_peer->getState() == ConnectionManager::States::OK) {
        emit onConnect();
        return true;
    }
    else {
        return false;
    }
    */
    emit onConnect();
    return true;
}

std::pair<quint16, QByteArray> PeerManager::listenToPeer( bool& result)
{
    if(!m_server)
        m_server = new QTcpServer(this);

    if(m_server->isListening())
    {
        qWarning() << "Server is already listen on"
                   << m_server->serverPort() << "port";
        result = false;
        return {};
    }

    m_server->listen();

    connect(m_server, &QTcpServer::newConnection,
            this, &PeerManager::onNewConnection);

    createKeys();


    qInfo() << "Listen for" << m_server->serverPort() << "port";


    result = true;
    return {
        m_server->serverPort(),
        m_publicKey
    };
}

void PeerManager::send(QString message)
{
    if(m_peer)
        m_peer->sendMessage(message);
    else
        qWarning() << "Cannot send message."
                      "Peer was not created";
}

void PeerManager::init()
{
}

void PeerManager::createKeys()
{
    QRSAEncryption::generatePairKey(m_publicKey,
                                    m_privateKey,
                                    QRSAEncryption::RSA_2048);
}

void PeerManager::onNewConnection()
{

    QTcpSocket* socket = m_server->nextPendingConnection();
    qDebug() << "New peer connection on:" << socket->localPort() << "port";

    m_peer = new Peer(socket, m_publicKey, m_privateKey);
    m_peer->init();
    m_publicKey.clear();
    m_privateKey.clear();


    connect(m_peer, &Peer::newMessageSignal,
            this, &PeerManager::onNewMessage);

    emit onConnect();
}
