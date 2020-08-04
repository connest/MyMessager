#ifndef QMLCONNECTOR_H
#define QMLCONNECTOR_H

#include <QGuiApplication>
#include <QObject>
#include <memory>

#include "MyMessagerGlobal.h"

class QMLConnector : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString Name
               READ getName
               WRITE setName
               NOTIFY onChangeName)
    Q_PROPERTY(QString Password
               READ getPassword
               WRITE setPassword
               NOTIFY onChangePassword)
    Q_PROPERTY(QString Host
               READ getHost
               WRITE setHost
               NOTIFY onChangeHost)
    Q_PROPERTY(QString ClientName
               READ getClientName
               WRITE setClientName
               NOTIFY onChangeClientName)
    Q_PROPERTY(QString ClientPassword
               READ getClientPassword
               WRITE setClientPassword
               NOTIFY onChangeClientPassword)



public:
    explicit QMLConnector(const QGuiApplication& app, QObject *parent = nullptr);
    ~QMLConnector();

    QString getName() const;
    QString getPassword() const;
    QString getHost() const;

    QString getClientName() const;

    QString getClientPassword() const;

public slots:

    void setName(QString Name);
    void setPassword(QString Password);
    void setHost(QString Host);

    void setLoginParameters(QString Name,
                            QString Password,
                            QString Host);
    void setClientParameters(QString clientName,
                             QString Password);

    void setClientName(QString ClientName);

    void setClientPassword(QString ClientPassword);

signals:
    Q_INVOKABLE void logined();
    Q_INVOKABLE void waitForConnect();
    Q_INVOKABLE void error(net::Errors err);
    void onChangeName(QString Name);
    void onChangePassword(QString Password);
    void onChangeHost(QString Host);

    void onChangeClientName(QString ClientName);

    void onChangeClientPassword(QString ClientPassword);

private:
    class QMLConnectorPrivate;

    std::unique_ptr<QMLConnectorPrivate> m_d;

    QString m_Name;
    QString m_Password;
    QString m_Host;
    QString m_ClientName;
    QString m_ClientPassword;
};

#endif // QMLCONNECTOR_H
