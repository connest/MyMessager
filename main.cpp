#include "MainWindow.h"
#include "Client.h"
#include "Server.h"

#include <QApplication>
#include <QMessageBox>
#include <QtNetwork>
#include <qrsaencryption.h>

bool testEncryptAndDecryptExample() {

    QByteArray pub, priv;
    QRSAEncryption e(QRSAEncryption::Rsa::RSA_64);
    e.generatePairKey(pub, priv); // or other rsa size

    QByteArray msg = "test message";

    auto encryptMessage = e.encode(msg, pub);

    if (encryptMessage == msg)
        return false;

    auto decodeMessage = e.decode(encryptMessage, priv);
    qDebug() << encryptMessage;

    return decodeMessage == msg;
}

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

    /*Server s(4567);
    Client c(4567);
    c.secureConnect();*/


    if (testEncryptAndDecryptExample()) {
            qInfo() <<"success!";
        }

    return 0;
    return a.exec();
}
