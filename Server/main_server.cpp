#include "Server.h"

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QtNetwork>

int main(int argc, char *argv[])
{
    if (!QSslSocket::supportsSsl()) {
        qCritical() << "Secure Socket Client \n"
                       "This system does not support TLS.";
        return -1;
    }


    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("MyMessager Server");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;

    parser.setApplicationDescription("The server for connect peers to each other");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption port={{"p", "port"}, "Set <port> for listen", "4567"};
    port.setDefaultValue("4567");

    parser.addOption(port);
    parser.process(a);




    new SSLServer(parser.value("port").toInt());

    return a.exec();
}
