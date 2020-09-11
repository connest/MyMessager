//#include "Client.h"


#include <QtNetwork>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "QMLConnector.h"
#include "MessageModel.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    if (!QSslSocket::supportsSsl()) {
        qCritical() << "This system does not support TLS.";
        return -1;
    }
    /*QmlConnector client1;

    client1.setConnectionData("localhost:4567",
                              "client2",
                              "1234",
                              "client1",
                              "1234");
*/
    QmlConnector c;


    QQmlApplicationEngine engine;
    engine.addImportPath(":/QML");
    engine.rootContext()->setContextProperty("qmlconnector", &c);
    engine.rootContext()->setContextProperty("CoreMessageModel", c.getModel());




    const QUrl url(QStringLiteral("qrc:/QML/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    return app.exec();
}
