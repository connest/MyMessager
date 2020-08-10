#include "ClientManager.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include "MyMessagerGlobal.h"

ClientManager::ClientManager(QObject *parent)
    : ConnectionManager{true, parent}
{
    connect(this, &ConnectionManager::newDataSignal,
            this, &ClientManager::processData);

}

ClientManager::~ClientManager()
{

}










void ClientManager::processData(const QJsonDocument &data)
{

    //qDebug() << "Client recv:" << data;

    switch(data["operation"].toInt())
    {
    case Operations::NO_SUCH_CLIENT :
        emit onNoClient();
        break;
    case Operations::CONNECT_TO_CLIENT:
        emit onFoundClient(
                    data["IP"].toString(),
                data["port"].toInt(),
                jsonValue2ByteArray(data["publicKey"])
                );
        break;
    }
}

void ClientManager::registration(QString name, QString password,
                                 QByteArray publicKey, quint16 port)
{

    QByteArray hashClient=QCryptographicHash::hash(
                (name+password).toLatin1(),
                QCryptographicHash::Algorithm::Sha256
                );

    qInfo() << "Registration ...";
    send({
             {"operation", Operations::REGISTRATION},
             {"client", byteArray2string(hashClient)},
             {"publicKey", byteArray2string(publicKey)},
             {"port", port}
         });

}
void ClientManager::whoIs(QString clientName, QString clientPassword)
{

    if(waitForConnect()  == ConnectionManager::States::OK)
    {
        QByteArray hashClient=QCryptographicHash::hash(
                    (clientName+clientPassword).toLatin1(),
                    QCryptographicHash::Algorithm::Sha256
                    );


        send({
                 {"operation", Operations::WHOIS},
                 {"client", byteArray2string(hashClient)},
             });
    }
    else
    {
        qInfo() << "Cannot to send WHOIS, because "
                   "client manager has error code:"
                << static_cast<int>(getState());
    }

}
