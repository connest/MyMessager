#ifndef QMLCONNECTOR_H
#define QMLCONNECTOR_H

#include <QObject>
#include <memory>
#include <QQmlContext>

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


    void registerModel(QQmlContext* context);
signals:
    void error();
    void toMessageArea();
private:
    class QmlConnectorPrivate;
    std::unique_ptr<QmlConnectorPrivate> m_d;
};

#endif // QMLCONNECTOR_H
