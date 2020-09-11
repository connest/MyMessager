#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>

#include <QtNetwork>

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    enum class States {
        NONE,
        OK,
        DISCONNECT,
        SERVER_OFFLINE,
        SSL_ERROR,
        UNKNOWN_ERROR
    };

    explicit ConnectionManager(bool secure, QObject *parent = nullptr);
    explicit ConnectionManager(QAbstractSocket* socket, bool secure, QObject *parent = nullptr);
    ~ConnectionManager();

    void connectToHost(QString host, quint16 port);
    ConnectionManager::States waitForConnect();
    void close();
    void init();

    ConnectionManager::States getState() const;

signals:
    void errorSignal();
    void connectedSignal();
    void newDataSignalRaw(const QByteArray& data);
    void newDataSignal(const QJsonDocument& data);


protected:
    void send(const QByteArray& data);
    void send(const QJsonObject& data);

    bool getRaw() const;
    void setGetRaw(bool getRaw);

private:
    bool m_isSecure;
    bool m_getRaw {false};

    ConnectionManager::States m_state{ConnectionManager::States::NONE};

    QAbstractSocket* m_socket = nullptr;

    void setupSecureSocket();
    void setupTcpSocket();


    void secureConnect(QString host, quint16 port);
    void tcpConnect(QString host, quint16 port);
    void declareQAbstractSocketSocketError();

private slots:
    void onRead();
    void onError(QAbstractSocket::SocketError socketError);
    void onSslErrors(const QList<QSslError> &errors);
};

#endif // CONNECTIONMANAGER_H
