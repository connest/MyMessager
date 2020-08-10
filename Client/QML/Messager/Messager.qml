import QtQuick 2.2
import QtQuick.Layouts 1.15

import Base 1.0

Item {
    id: root
    //anchors.fill: parent

    GridLayout {
        columns: 2
        anchors.fill: parent
        Item {
            Layout.columnSpan: 2
            Layout.fillHeight: true
            Layout.fillWidth: true

            MessageArea {
            }
        }
        Item {
            height: 40
            Layout.fillWidth: true

            EditLine {
                id: _messageInput
                height: 20
                width: parent.width - 20
                focus: true
                anchors.centerIn: parent
            }
        }
        Item {
            height: 40
            Layout.preferredWidth: _sendButton.width + 20

            CustomButton {
                anchors.centerIn: parent
                id: _sendButton
                text: "Send"
                height: 20
                width: Math.min(80, root.width * 0.20)

                onClicked: qmlconnector.send(_messageInput.text)
            }
        }
    }
}
