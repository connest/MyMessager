import QtQuick 2.2
import QtQuick.Layouts 1.15
import Base 1.0
Item {
    id: root
    anchors.fill: parent

    GridLayout {
        id: grid
        columns: 2

        anchors.centerIn: parent


        Layout.maximumHeight: 600
        Layout.maximumWidth: 600

        Item {
            Layout.preferredWidth: 100
            Layout.minimumHeight: 50
            Text {
                text: "Server"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        EditLine {
            id: _host

            Layout.preferredWidth: 200

            height: 20
            isFocus: true

            KeyNavigation.tab: _login.input
        }
        Item {
            Layout.preferredWidth: 100
            Layout.minimumHeight: 50
            Text {
                text: "Login"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        EditLine {
            id: _login

            Layout.preferredWidth: 200
            height: 20

            KeyNavigation.tab: _password.input
        }
        Item {
            Layout.preferredWidth: 100
            Layout.minimumHeight: 50
            Text {
                text: "Password"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        EditLine {
            Layout.preferredWidth: 200
            id: _password

            height: 20
            isPassword: true

            KeyNavigation.tab: _loginButton
        }
        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 2
            CustomButton {

                anchors.right: parent.right
                id: _loginButton
                width: 80
                height: 40
                text: "Login"

                KeyNavigation.tab: _host.input

                onClicked: {
                    qmlconnector.setName(_login.input.text)
                    qmlconnector.setPassword(_password.input.text)
                    qmlconnector.setHost(_host.input.text)
                    qmlconnector.onLogin();

                }
            }
        }


    }


}
