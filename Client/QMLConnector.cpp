#include "QMLConnector.h"
#include "Client.h"
#include <QDebug>

#include <QGuiApplication>


class QMLConnector::QMLConnectorPrivate
{
public:
    QMLConnectorPrivate(const QGuiApplication& app, QMLConnector& parent);
    ~QMLConnectorPrivate();
    void initClient(const QString& hostname,
                    const QString& name,
                    const QString& password
                    );
    void connect();
    void WHOIS(QString clientName, QString clientPassword);
private:
    Client m_client;
    Peer* m_peer;

    QThread t;
    QMLConnector& parent;
};
QMLConnector::QMLConnectorPrivate::QMLConnectorPrivate(const QGuiApplication& app, QMLConnector& parent)
    : m_client {}
    , parent {parent}
{
//    QObject::connect(&m_client, &Client::errorSignal, &parent, &QMLConnector::error);

    QObject::connect(&app, &QGuiApplication::aboutToQuit, &t, &QThread::quit);

    m_client.moveToThread(&t);
    t.start();
}

QMLConnector::QMLConnectorPrivate::~QMLConnectorPrivate()
{
    m_client.deleteLater();
}

void QMLConnector::QMLConnectorPrivate::initClient(const QString &hostname,
                                                   const QString &name,
                                                   const QString &password)
{
    quint16 m_port {4567};

    QStringList l=hostname.split(QChar(':'));
    if(l.count() == 2) {
        m_port=static_cast<quint16>(l.at(1).toInt());
    }

    m_client.setNamePassword(name, password);
    m_client.setHostPort(hostname, m_port);

}

void QMLConnector::QMLConnectorPrivate::connect()
{
    m_client.secureConnect();
}

void QMLConnector::QMLConnectorPrivate::WHOIS(QString clientName, QString clientPassword)
{
    m_client.whoIs(clientName, clientPassword);
    QObject::connect(&m_client, &Client::peerCreated, [this](Peer* peer) {
        this->m_peer = peer;
        qDebug() << "Peer enabled";
    });
}









QMLConnector::QMLConnector(const QGuiApplication &app, QObject *parent)
    : QObject(parent)
{
    m_d=std::make_unique<QMLConnectorPrivate>(app, *this);
}


QMLConnector::~QMLConnector()
{
    deleteLater();
}

QString QMLConnector::getName() const
{
    return m_Name;
}

QString QMLConnector::getPassword() const
{
    return m_Password;
}

QString QMLConnector::getHost() const
{
    return m_Host;
}

QString QMLConnector::getClientName() const
{
    return m_ClientName;
}

QString QMLConnector::getClientPassword() const
{
    return m_ClientPassword;
}


void QMLConnector::setClientName(QString ClientName)
{
    if (m_ClientName == ClientName)
        return;

    m_ClientName = ClientName;
    emit onChangeClientName(m_ClientName);
}

void QMLConnector::setClientPassword(QString ClientPassword)
{
    if (m_ClientPassword == ClientPassword)
        return;

    m_ClientPassword = ClientPassword;
    emit onChangeClientPassword(m_ClientPassword);
}

void QMLConnector::setHost(QString Host)
{
    if (m_Host == Host)
        return;

    m_Host = Host;
    emit onChangeHost(m_Host);
}

void QMLConnector::setLoginParameters(QString Name, QString Password, QString Host)
{
    //TODO: checking for correct input

    setName(Name);
    setPassword(Password);
    setHost(Host);
    m_d->initClient(m_Host, m_Name, m_Password);
    m_d->connect();
    emit logined();
}

void QMLConnector::setClientParameters(QString clientName, QString Password)
{
    setClientName(clientName);
    setClientPassword(Password);
    m_d->WHOIS(clientName, Password);
    emit waitForConnect();
}

void QMLConnector::setName(QString Name)
{
    if (m_Name == Name)
        return;

    m_Name = Name;
    emit onChangeName(m_Name);
}

void QMLConnector::setPassword(QString Password)
{
    if (m_Password == Password)
        return;

    m_Password = Password;
    emit onChangePassword(m_Password);
}

