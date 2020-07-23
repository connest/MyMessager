#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include <QMessageBox>

class Client : public QObject
{
    Q_OBJECT

    QSslSocket *socket = nullptr;
    int portSocket;
public:
    explicit Client(int port, QObject *parent = nullptr)
        : QObject(parent)
        , portSocket(port)
    {
        setupSocket();
    }

    ~Client()
    {
        if(socket)
            delete socket;
    }
    void setupSocket()
    {
        if (socket)
            return;

        socket = new QSslSocket(this);

        connect(socket, &QSslSocket::stateChanged,
                this, &Client::slotStateChanged);
        connect(socket, &QSslSocket::encrypted,
                this, &Client::slotEncrypted);
        connect(socket, &QSslSocket::errorOccurred,
                this, &Client::slotError);
        connect(socket,
                QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
                this, &Client::sslErrors);
        connect(socket, &QSslSocket::readyRead,
                this, &Client::slotReadyRead);
    }

    void processData(QByteArray& data)
    {
        QString str;
        QDataStream in(&data, QIODevice::ReadWrite);
        in.setVersion(QDataStream::Qt_5_15);
        in >> str;

        in << "Echo from client: " << str;


        send(data);
    }

    void send(const QByteArray& data)
    {
        socket->write(data);
    }

public slots:
    void secureConnect()
    {
        QSslConfiguration conf;
        conf.addCaCertificates(
             "C:/Users/Constatine/Desktop/projects/MyMessager/Certifications/key_c.pem"
        );


        socket->setSslConfiguration(conf);


        socket->connectToHostEncrypted("localhost", portSocket);




    }
    void slotStateChanged(QAbstractSocket::SocketState state)
    {
        qDebug()<< "Client: " << state;
    }
    void slotEncrypted()
    {

    }
    void slotReadyRead()
    {
        QByteArray data=socket->readAll();
        processData(data);
    }
    void slotError(QAbstractSocket::SocketError error)
    {
        qDebug() << "Client error: " << socket->errorString()<<error;
    }
    void sslErrors(const QList<QSslError> &errors)
    {
        for(const auto& err: errors)
            qDebug() << "Client ssl error: "<< err;
    }
};

#endif // CLIENT_H
