#include "QmlConnectorPrivate.h"
#include <QCoreApplication>
#include "MessageModel.h"
QmlConnector::QmlConnectorPrivate::QmlConnectorPrivate(QObject *parent)
    : QObject(parent)
{
    QmlConnector* p = qobject_cast<QmlConnector*>(parent);

    connect(p, &QmlConnector::error, &m_client, &ClientManager::errorSignal);

    connect(QCoreApplication::instance(),
            &QCoreApplication::aboutToQuit,
            &t,
            &QThread::quit);

    m_client.moveToThread(&t);
    m_peer.moveToThread(&t);
    t.start();

    connect(&m_peer, &PeerManager::onNewMessage,
            this, &QmlConnectorPrivate::onNewMessage);

    connect(&m_peer, &PeerManager::onConnect,
            this, &QmlConnectorPrivate::connectSignal);
}

QmlConnector::QmlConnectorPrivate::~QmlConnectorPrivate()
{

}

void QmlConnector::QmlConnectorPrivate::connectToServer(QString hostname)
{
    quint16 port {4567};

    QStringList l=hostname.split(QChar(':'));
    if(l.count() == 2) {
        port=static_cast<quint16>(l.at(1).toInt());
    }


    m_client.connectToHost(l.at(0), port);

}


void QmlConnector::QmlConnectorPrivate::whoIs()
{
    m_client.whoIs(m_clientName, m_clientPassword);

    connect(&m_client, &ClientManager::onNoClient,
            this, &QmlConnectorPrivate::onNoClient);
    connect(&m_client, &ClientManager::onFoundClient,
            this, &QmlConnectorPrivate::onFoundClient);

}

void QmlConnector::QmlConnectorPrivate::setLoginParameters(QString name,
                                                           QString password)
{
    m_name      = name;
    m_password  = password;
}

void QmlConnector::QmlConnectorPrivate::setClientParameters(QString clientName,
                                                            QString clientPassword)
{
    m_clientName        = clientName;
    m_clientPassword    = clientPassword;
}

void QmlConnector::QmlConnectorPrivate::send(QString message)
{
    m_peer.send(message);
    m_model.add(MessageModel::Message(QDateTime::currentDateTime(),
                                      message,
                                      true));

}

MessageModel *QmlConnector::QmlConnectorPrivate::getModel()
{
    return &m_model;
}

void QmlConnector::QmlConnectorPrivate::onNoClient()
{
    bool result;
    quint16 port;
    QByteArray publicKey;
    std::tie(port, publicKey) = m_peer.listenToPeer(result);

    if(result)
    {
        m_client.registration(m_name, m_password, publicKey, port);

        emit listenSignal(port);

        m_client.close();
    }
    else
        qWarning() << "Server is already listening";
}

void QmlConnector::QmlConnectorPrivate::onFoundClient(QString IP,
                                                      quint16 port,
                                                      QByteArray publicKey)
{
    bool resultConnection = m_peer.connectToPeer(IP, port, publicKey);

    if(resultConnection)
        m_client.close();
    else
    {
        qWarning() << "Cannot connect to peer"
                   << IP<<":"<<port;
    }
}

void QmlConnector::QmlConnectorPrivate::onNewMessage(QString message, QDateTime time)
{
    m_model.add(MessageModel::Message(time, message, false));
}



