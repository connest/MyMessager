import QtQuick 2.2
import QtQuick.Window 2.2
import Form 1.0
import QtQuick.Controls 2.12

Window {
    id: root
    width: 600
    height: 600

    visible: true

    StackView {
        anchors.fill: parent
        id: _stackView
        initialItem: _loginView

        Component {

            id: _loginView
            Login {
            }
        }

        Component {
            id: _connectView
            Connect {
            }
        }
    }


    Connections {
        target: qmlconnector
        function onLogined() {
            _stackView.push(_connectView);
        }
        function onError(err) {
            console.error("Error: " + err);
        }
    }

}
