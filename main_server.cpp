#include "MainWindow.h"
#include "Server.h"

#include <QApplication>
#include <QtNetwork>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    if (!QSslSocket::supportsSsl()) {
        QMessageBox::information(nullptr, "Secure Socket Client",
                                 "This system does not support TLS.");
        return -1;
    }


    new SSLServer(4567);

    return a.exec();
}
