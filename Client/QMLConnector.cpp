#include "QMLConnector.h"
#include "QmlConnectorPrivate.h"

QmlConnector::QmlConnector(QObject *parent)
    : QObject(parent)
{
    m_d = new QmlConnectorPrivate();
    m_t = new QThread();

    connect(m_d, &QmlConnectorPrivate::connectSignal,
            this, &QmlConnector::toMessageArea,
            Qt::QueuedConnection);

    connect(m_d, &QmlConnectorPrivate::newMessageSignal,
            this, &QmlConnector::onNewMessage,
            Qt::QueuedConnection);

    connect(this, &QmlConnector::setConnectionDataSignal,
            m_d, &QmlConnectorPrivate::setConnectionDataSlot,
            Qt::QueuedConnection);

    connect(this, &QmlConnector::sendSignal,
            m_d, &QmlConnectorPrivate::sendSlot,
            Qt::QueuedConnection);


    connect(m_t, &QThread::started, m_d, &QmlConnectorPrivate::init);
    connect(m_t, &QThread::finished, m_d, &QmlConnectorPrivate::deleteLater);


    m_d->moveToThread(m_t);
    m_t->start();
}

QmlConnector::~QmlConnector()
{
    if(m_t)
    {
        m_t->quit();
        m_t->wait();
        m_t->deleteLater();
    }

    deleteLater();
}

void QmlConnector::setConnectionData(QString hostname,
                                     QString login,
                                     QString password,
                                     QString clientLogin,
                                     QString clientPassword)
{

    emit setConnectionDataSignal(hostname,
                                 login,
                                 password,
                                 clientLogin,
                                 clientPassword);
   /* m_d->setLoginParameters(login, password);
    m_d->setClientParameters(clientLogin, clientPassword);
    m_d->connectToServer(hostname);

    m_d->whoIs();*/
}

void QmlConnector::send(QString message)
{
    emit sendSignal(message);
    //m_d->send(message);
}

MessageModel *QmlConnector::getModel()
{
    return &m_model;
}

void QmlConnector::onNewMessage(MessageModel::Message message)
{
    m_model.add(message);
}

