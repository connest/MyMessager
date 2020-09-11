#ifndef QMLCONNECTOR_H
#define QMLCONNECTOR_H

#include <QObject>
#include <memory>
#include <QThread>

#include "MessageModel.h"

class QmlConnector : public QObject
{
    Q_OBJECT
public:
    explicit QmlConnector(QObject *parent = nullptr);
    ~QmlConnector();

    Q_INVOKABLE void setConnectionData(QString hostname,
                                       QString login,
                                       QString password,
                                       QString clientLogin,
                                       QString clientPassword);
    Q_INVOKABLE void send(QString message);

    MessageModel* getModel();

public slots:
    void onNewMessage(MessageModel::Message message);


signals:
    void error();
    void toMessageArea();

    void setConnectionDataSignal(QString hostname,
                                 QString login,
                                 QString password,
                                 QString clientLogin,
                                 QString clientPassword);
    void sendSignal(QString message);

private:
    class QmlConnectorPrivate;

    QmlConnectorPrivate* m_d;
    QThread* m_t;

    MessageModel m_model;
};

#endif // QMLCONNECTOR_H
