import QtQuick 2.2
import QtQuick.Window 2.2
import Form 1.0
import Messager 1.0
import QtQuick.Controls 2.12
import Base 1.0

Window {
    id: root
    width: 600
    height: 600

    visible: true

    color: Style.backgroungColor
    StackView {

        anchors.fill: parent
        id: _stackView
        initialItem: _loginView

        Component {

            id: _loginView
            Login {
            }
        }

        /*Component {
            id: _connectView
            Connect {
            }
        }*/

        Component {
            id: _messagerView
            Messager {
            }
        }
    }


    Connections {
        target: qmlconnector
        function onToMessageArea() {
            _stackView.push(_messagerView);
        }
        function onError(err) {
            console.error("Error: " + err);
        }
    }

}
