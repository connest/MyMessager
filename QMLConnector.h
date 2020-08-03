#ifndef QMLCONNECTOR_H
#define QMLCONNECTOR_H

#include <QObject>
#include <memory>
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



public:
    explicit QMLConnector(QObject *parent = nullptr);
    ~QMLConnector();

    QString getName() const;
    QString getPassword() const;
    QString getHost() const;

public slots:

    void setName(QString Name);
    void setPassword(QString Password);
    void setHost(QString Host);

    Q_INVOKABLE void onLogin();

signals:
    void onLogined();
    void onChangeName(QString Name);
    void onChangePassword(QString Password);
    void onChangeHost(QString Host);

private:
    class QMLConnectorPrivate;

    std::unique_ptr<QMLConnectorPrivate> m_d;

    QString m_Name;
    QString m_Password;
    QString m_Host;
};

#endif // QMLCONNECTOR_H
