import QtQuick 2.2
import Base 1.0
import QtQuick.Controls 2.15
Rectangle {
    id: root

    width: Math.max(100, parent.width *0.6)
    height: _timeText.implicitHeight + _messageText.implicitHeight + 10

    anchors {
        left: post? undefined
                  : parent.left

        right: post? parent.right
                   : undefined
    }

    radius: 5

    color: Style.lowBackgroundColor



    Item {
        clip: true
        height: root.height - 10
        width: root.width - 10
        anchors.centerIn: parent



        Text {
            id: _timeText
            text: time
            width: 100
            wrapMode: Text.Wrap
            anchors.top: parent.top

        }
        Text {
            id: _messageText
            text: message
            width: root.width
            wrapMode: Text.Wrap



            anchors.top: _timeText.bottom
        }
    }

}
