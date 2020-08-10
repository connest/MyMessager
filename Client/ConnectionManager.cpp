#include "ConnectionManager.h"

ConnectionManager::ConnectionManager(bool secure, QObject *parent)
    : QObject{parent}
    , m_isSecure{secure}
{
    if(secure)
        setupSecureSocket();
    else
        setupTcpSocket();
}

ConnectionManager::ConnectionManager(QAbstractSocket* socket,
                                     bool secure,
                                     QObject *parent)
    : QObject{parent}
    , m_isSecure{secure}
    , m_socket{socket}
{

    if(secure)
        setupSecureSocket();
    else
        setupTcpSocket();

}

ConnectionManager::~ConnectionManager()
{
    if(m_socket)
    {
        m_socket->abort();
        m_socket->deleteLater();
    }
}

void ConnectionManager::connectToHost(QString host, quint16 port)
{

    if(m_isSecure)
        secureConnect(host, port);
    else
        tcpConnect(host, port);
}

ConnectionManager::States ConnectionManager::waitForConnect()
{
   bool result {m_socket->waitForConnected()};

   if(result)
       return States::OK;

   return getState();
}

void ConnectionManager::setupSecureSocket()
{

    declareQAbstractSocketSocketError();

    if(!m_socket)
        m_socket = new QSslSocket();


    connect(m_socket, &QSslSocket::readyRead, this, &ConnectionManager::onRead);
    connect(m_socket, &QAbstractSocket::errorOccurred,
            this, &ConnectionManager::onError);
    connect(m_socket, &QAbstractSocket::connected, [&](){
        m_state = ConnectionManager::States::OK;
        emit connectedSignal();
    });

    QSslSocket* sslSocket = qobject_cast<QSslSocket*>(m_socket);
    connect(sslSocket, QOverload<const QList<QSslError> &>::of(&QSslSocket::sslErrors), this, &ConnectionManager::onSslErrors);

}

void ConnectionManager::setupTcpSocket()
{

    declareQAbstractSocketSocketError();

    if(!m_socket)
        m_socket = new QTcpSocket();

    connect(m_socket, &QSslSocket::readyRead, this, &ConnectionManager::onRead);
    connect(m_socket, &QSslSocket::errorOccurred,
            this, &ConnectionManager::onError);
    /*connect(m_socket, &QAbstractSocket::connected,
             this, &ConnectionManager::connectedSignal);*/

    connect(m_socket, &QAbstractSocket::connected, [&](){
        m_state = ConnectionManager::States::OK;
        emit connectedSignal();
    });
}

void ConnectionManager::secureConnect(QString host, quint16 port)
{

    qInfo() << "Try to secure connect to"<<host<<port;
    QSslConfiguration conf {QSslConfiguration::defaultConfiguration()};

#ifdef LOCALHOST_CERTIFICATE
    conf.addCaCertificates(
                "C:/Users/Constatine/Desktop/projects/MyMessager/shared/Certifications/key_c.pem"
                );
#endif


    QSslSocket* sslSocket = qobject_cast<QSslSocket*>(m_socket);

    if(!sslSocket)
    {
        qCritical() << "Cannot cast m_socket to QSslSocket at"
                    << __FILE__<<":"<<__LINE__;
        //throw std::bad_cast();
        return;
    }
    sslSocket->setSslConfiguration(conf);
    sslSocket->setProtocol(QSsl::TlsV1_3);
    sslSocket->connectToHostEncrypted(host, port);
}

void ConnectionManager::tcpConnect(QString host, quint16 port)
{
    m_socket->connectToHost(host, port);
}

void ConnectionManager::declareQAbstractSocketSocketError()
{

    static std::once_flag QAbstractSocketSocketErrorDeclared;
    std::call_once(QAbstractSocketSocketErrorDeclared, [](){
        qRegisterMetaType<QAbstractSocket::SocketError>();
    });
}

void ConnectionManager::onRead()
{

    QByteArray request_raw {m_socket->readAll()};

    if(m_getRaw)
    {
        emit newDataSignalRaw(request_raw);
    }
    else
    {
        auto request=QJsonDocument::fromJson(request_raw);
        emit newDataSignal(request);
    }
    //processData(request.object());
}

void ConnectionManager::onError(QAbstractSocket::SocketError socketError)
{

    qInfo() << "Client manager error:" <<m_socket->errorString()
            <<"\n(" << socketError << ")";

    switch (socketError) {
    case QAbstractSocket::ConnectionRefusedError:
    case QAbstractSocket::RemoteHostClosedError:
    case QAbstractSocket::SocketTimeoutError:
    case QAbstractSocket::OperationError:
    case QAbstractSocket::TemporaryError:
        m_state = States::DISCONNECT;
        break;

    case QAbstractSocket::HostNotFoundError:
    case QAbstractSocket::SocketAccessError:
    case QAbstractSocket::SocketResourceError:
    case QAbstractSocket::NetworkError:
        m_state = States::SERVER_OFFLINE;
        break;

    case QAbstractSocket::SslHandshakeFailedError:
    case QAbstractSocket::SslInternalError:
    case QAbstractSocket::SslInvalidUserDataError:
        m_state = States::SSL_ERROR;
        break;

    default:
        m_state = States::UNKNOWN_ERROR;
        break;
    }

    emit errorSignal();
}

void ConnectionManager::onSslErrors(const QList<QSslError> &errors)
{

    qInfo() << "Client manager SSL error:" <<m_socket->errorString();
    for(const auto& err: errors)
        qInfo() <<"\n(" << err << ")";

    m_state = States::SSL_ERROR;
    emit errorSignal();
}

void ConnectionManager::send(const QByteArray &data)
{

    //qDebug() << "Client send: " << data;
    m_socket->write(data);
}

void ConnectionManager::send(const QJsonObject &data)
{

    send(QJsonDocument(data).toJson());
}

bool ConnectionManager::getRaw() const
{

    return m_getRaw;
}

void ConnectionManager::setGetRaw(bool getRaw)
{

    m_getRaw = getRaw;
}

ConnectionManager::States ConnectionManager::getState() const
{
    return m_state;
}

void ConnectionManager::close()
{

    m_socket->close();
}
