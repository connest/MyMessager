//#include "Client.h"


#include <QtNetwork>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "QMLConnector.h"
#include "Client.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    if (!QSslSocket::supportsSsl()) {
        qCritical() << "This system does not support TLS.";
        return -1;
    }

    QObject o;
    QMLConnector q;
    QThread t;
    QObject::connect(&app, &QGuiApplication::aboutToQuit, &t, &QThread::quit);

    q.moveToThread(&t);
    t.start();

    QQmlApplicationEngine engine;
    engine.addImportPath(":/QML");

    engine.rootContext()->setContextProperty("qmlconnector", &q);

    const QUrl url(QStringLiteral("qrc:/QML/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


/*


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
*/




     return app.exec();
}
