import QtQuick 2.0
import Base 1.0

ListView {
    anchors.fill: parent
    spacing: 4
    clip: true
    model: CoreMessageModel
          /* ListModel {
        ListElement {
            post: true
            message: "Too many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many many text"
            time: "23.56.78 12:45:67"
        }
        ListElement {
            post: true
            message: "asd"
            time: "23.56.78 12:45:67"
        }
        ListElement {
            post: true
            message: "aaergaergaregaregargargargaregaergaergaregargtyjrtyjketket67ked67ksd"
            time: "23.56.78 12:45:67"
        }
    }*/

    delegate: Message {

    }

    //Component.onCompleted: positionViewAtEnd()
}
