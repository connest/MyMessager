QT       += core quick network

greaterThan(QT_MAJOR_VERSION, 4): QT += quick

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
            LOCALHOST_CERTIFICATE

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Client.cpp \
    $$PWD/../shared/includes/MyMessagerGlobal.cpp \
    Peer.cpp \
    QMLConnector.cpp \
    main_client.cpp

HEADERS += \
    Client.h \
    $$PWD/../shared/includes/MyMessagerGlobal.h \
    Peer.h \
    QMLConnector.h

RESOURCES += qml.qrc

INCLUDEPATH += $$PWD/../shared/includes


QML_IMPORT_PATH += $$PWD/QML/Base \
                   $$PWD/QML/Form \
                   $$PWD/QML/Messager

QML2_IMPORT_PATH += $$PWD/QML/Base \
                   $$PWD/QML/Form \
                   $$PWD/QML/Messager

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include ($$PWD/Qt-Secret/src/Qt-Secret.pri)


