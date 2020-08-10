#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QObject>
#include <QJsonDocument>

#include "ConnectionManager.h"

class ClientManager : public ConnectionManager
{
    Q_OBJECT

public:
    explicit ClientManager(QObject *parent = nullptr);
    ~ClientManager();

    void whoIs(QString clientName,
               QString clientPassword);
    void registration(QString name,
                      QString password,
                      QByteArray publicKey,
                      quint16 port);
signals:
    void onNoClient();
    void onFoundClient(QString IP, quint16 port, QByteArray publicKey);
private slots:

    void processData(const QJsonDocument& data);

};

#endif // CLIENTMANAGER_H
