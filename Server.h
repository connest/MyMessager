#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include <QMessageBox>


class Server : public QTcpServer
{
    Q_OBJECT

    QSslSocket* socket;
    int serverPort;

    bool initSocket(qintptr socketID)
    {
        if (this->socket->setSocketDescriptor(socketID))
        {
            socket->setLocalCertificate(
                        "C:/Users/Constatine/Desktop/projects/MyMessager/Certifications/key_c.pem");
            socket->setPrivateKey(
                        "C:/Users/Constatine/Desktop/projects/MyMessager/Certifications/key_c.key");

            //указываем версию протокола
            this->socket->setProtocol(QSsl::TlsV1_2);

            //устанвливаем наше соединение по сокету
            this->socket->startServerEncryption();


            connect(socket, &QSslSocket::stateChanged,
                    this, &Server::slotStateChanged);

            connect(socket, &QSslSocket::errorOccurred,
                    this, &Server::slotError);

            connect(socket,
                    QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors),
                    this, &Server::sslErrors);

            connect(socket, &QSslSocket::readyRead,
                    this, &Server::recv);

            return true;
        }
        return false;
    }
    void incomingConnection(qintptr socketID)
    {
        if(initSocket(socketID))
        {
            send<QString>("hello world");
        }
    }
public:
    Server(int port)
        : QTcpServer()
        , serverPort(port)
    {
        start();
        socket=new QSslSocket();
    }
    bool start()
    {
        if(listen(QHostAddress::Any, serverPort))
        {
            qDebug()<<"Server start on port:" << this->serverPort;

            emit started();
            return true;
        }

        return false;
    }

    template <typename T>
    void send(const T& data)
    //void send(const QString& data)
    {
        QByteArray arr;
        QDataStream out(&arr, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_15);
        out << data;

        socket->write(arr);
        socket->flush();
    }

    void processData(QByteArray& data)
    {
        QString str;
        QDataStream in(&data, QIODevice::ReadOnly);
        in.setVersion(QDataStream::Qt_5_15);
        in >> str;

        qDebug()<< "Server recv: " << str;
    }
public slots:
    void recv()
    {
        QByteArray data=socket->readAll();
        processData(data);
    }
    void slotStateChanged(QAbstractSocket::SocketState state)
    {
        qDebug() << "Server: " << state;
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
        qDebug()<< "Server error: " << socket->errorString()<<error;
    }
    void sslErrors(const QList<QSslError> &errors)
    {
        for(const auto& err: errors)
            qDebug() << "Server ssl error: "<< err;
    }
signals:
    void started();
};



#endif // SERVER_H
