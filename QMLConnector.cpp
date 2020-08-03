#include "QMLConnector.h"
#include "Client.h"
#include <QDebug>


class QMLConnector::QMLConnectorPrivate
{
public:
    QMLConnectorPrivate();
    ~QMLConnectorPrivate();
    void initClient(const QString& hostname,
                    const QString& name,
                    const QString& password
                    );
    quint16 port() const;
    void setPort(const quint16 &port);

private:
    Client m_client;
};
QMLConnector::QMLConnectorPrivate::QMLConnectorPrivate()
    : m_client {}
{

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
    qDebug() << "port: " << m_port;

    m_client.setNamePassword(name, password);
    m_client.setHostPort(hostname, m_port);

}









QMLConnector::QMLConnector(QObject *parent)
    : QObject(parent)
{
    m_d=std::make_unique<QMLConnectorPrivate>();
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

void QMLConnector::onLogin()
{
    //TODO: checking for correct input

    m_d->initClient(m_Host, m_Name, m_Password);
    emit onLogined();

}

void QMLConnector::setHost(QString Host)
{
    if (m_Host == Host)
        return;

    m_Host = Host;
    emit onChangeHost(m_Host);
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

