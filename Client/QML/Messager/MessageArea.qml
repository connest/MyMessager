import QtQuick 2.2
import QtQuick.Layouts 1.15

import Base 1.0
Rectangle {
    height: parent.height
    width: parent.width - 20
    anchors.centerIn: parent
    color: Style.backgroungColor
    MessageModel {
        anchors.fill: parent
    }
}
