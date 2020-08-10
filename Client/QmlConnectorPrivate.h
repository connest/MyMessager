#ifndef QMLCONNECTORPRIVATE_H
#define QMLCONNECTORPRIVATE_H

#include <QObject>
#include <memory>

#include "QmlConnector.h"
#include "ClientManager.h"
#include "PeerManager.h"
#include "MessageModel.h"

class QmlConnector::QmlConnectorPrivate : public QObject
{
    Q_OBJECT
public:
    explicit QmlConnectorPrivate(QObject *parent = nullptr);
    ~QmlConnectorPrivate();
    void connectToServer(QString hostname);
    void whoIs();
    void setLoginParameters(QString name, QString password);
    void setClientParameters(QString clientName, QString clientPassword);
    void send(QString message);

    MessageModel* getModel();
signals:
    void listenSignal(quint16 port);
    void connectSignal();

private slots:
    void onNoClient();
    void onFoundClient(QString IP, quint16 port, QByteArray publicKey);
    void onNewMessage(QString message, QDateTime time);
private:
    ClientManager m_client;
    PeerManager m_peer;

    QThread t;

    QString m_name;
    QString m_password;

    QString m_clientName;
    QString m_clientPassword;


    MessageModel m_model;
};

#endif // QMLCONNECTORPRIVATE_H
