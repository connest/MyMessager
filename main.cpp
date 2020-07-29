#include "MainWindow.h"
#include "Client.h"
#include "Server.h"

#include <QApplication>
#include <QMessageBox>
#include <QtNetwork>
#include <qrsaencryption.h>
#include <QObject>


int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(securesocketclient);

    QApplication a(argc, argv);


    if (!QSslSocket::supportsSsl()) {
        QMessageBox::information(nullptr, "Secure Socket Client",
                                 "This system does not support TLS.");
        return -1;
    }

   /* MainWindow w;
    w.show();*/



    new SSLServer(4567);

    QTimer::singleShot(1000, [&](){
        Client* c = new Client("localhost" ,4567, &a);
        c->setNamePassword("client1", "123456");
        c->secureConnect();
        c->whoIs("client2", "1234567");

        QObject::connect(c, &Client::peerCreated, [=](Peer* peer)
        {
            QObject::connect(peer, &Peer::readSignal, [=](const QString& message)
            {
                qDebug() << message;
            });
        });
    });



    QTimer::singleShot(5000, [&](){

        Client* c1 = new Client("localhost" ,4567, &a);
        c1->setNamePassword("client2", "1234567");
        c1->secureConnect();
        c1->whoIs("client1", "123456");

        QObject::connect(c1, &Client::peerCreated, [&](Peer* peer)
        {
            QTimer::singleShot(500, [=]()
            {

               peer->slotSend("HELLO");
               QObject::connect(peer, &Peer::readSignal, [=](const QString& message)
               {
                   qDebug() << message;
               });
            });
        });

    });



    return a.exec();
}
