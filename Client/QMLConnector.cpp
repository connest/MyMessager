#include "QmlConnector.h"
#include "QmlConnectorPrivate.h"

QmlConnector::QmlConnector(QObject *parent)
    : QObject(parent)
{
    m_d = std::make_unique<QmlConnectorPrivate>(this);

    connect(m_d.get(), &QmlConnectorPrivate::connectSignal,
            this, &QmlConnector::toMessageArea);
}

QmlConnector::~QmlConnector()
{

}

void QmlConnector::setConnectionData(QString hostname,
                                     QString login,
                                     QString password,
                                     QString clientLogin,
                                     QString clientPassword)
{

    m_d->setLoginParameters(login, password);
    m_d->setClientParameters(clientLogin, clientPassword);
    m_d->connectToServer(hostname);

    m_d->whoIs();
}

void QmlConnector::send(QString message)
{
    m_d->send(message);
}

void QmlConnector::registerModel(QQmlContext* context)
{
    context->setContextProperty("CoreMessageModel",
                               m_d->getModel());
}
