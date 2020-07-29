#include "Client.h"

/*Client::Client(QObject *parent) : QObject(parent)
{

}
*/

QByteArray Peer::getPrivKey() const
{
    return privKey;
}

void Peer::setPrivKey(const QByteArray &value)
{
    privKey = value;
}

void Peer::setClientPubKey(const QByteArray &value)
{
    clientPubKey = value;
}

QByteArray Peer::getClientPubKey() const
{
    return clientPubKey;
}

QByteArray Peer::getPubKey() const
{
    return pubKey;
}

void Peer::setPubKey(const QByteArray &value)
{
    pubKey = value;
}
