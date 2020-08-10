import QtQuick 2.2
import QtQuick.Layouts 1.15
import Base 1.0
import "/QML/Logic/Logic.js" as Logic
Item {
    id: root
   // anchors.fill: parent

    GridLayout {
        id: grid
        columns: 2

        anchors.centerIn: parent


        Layout.maximumHeight: 600
        Layout.maximumWidth: 600

        Item {
            Layout.preferredWidth: 100
            Layout.minimumHeight: 30
            Text {
                text: "Server"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        EditLine {
            id: _host
            text: "localhost:4567"

            Layout.preferredWidth: 200

            height: 20
            focus: true

            KeyNavigation.tab: _login
        }
        Item {
            Layout.preferredWidth: 100
            Layout.minimumHeight: 30
            Text {
                text: "Login"
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        EditLine {
            id: _login

            text: "client1"

            Layout.preferredWidth: 200
            height: 20

            KeyNavigation.tab: _password
        }
        Item {
            Layout.preferredWidth: 100
            Layout.minimumHeight: 30
            Text {
                text: "Password"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        EditLine {
            id: _password
            text: "1234"

            Layout.preferredWidth: 200
            height: 20
            echoMode: TextInput.Password

            KeyNavigation.tab: _peerLogin
        }
        Item {
            Layout.preferredWidth: 100
            Layout.minimumHeight: 30
            Text {
                text: "Peer login"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        EditLine {
            id: _peerLogin

            text: "client2"

            height: 20
            Layout.preferredWidth: 200

            KeyNavigation.tab: _peerPassword
        }
        Item {
            Layout.preferredWidth: 100
            Layout.minimumHeight: 30
            Text {
                text: "Peer password"
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        EditLine {
            id: _peerPassword
            text: "1234"

            Layout.preferredWidth: 200
            height: 20
            echoMode: TextInput.Password

            KeyNavigation.tab: _loginButton
        }
        Item {
            Layout.fillWidth: true
            Layout.columnSpan: 2
            CustomButton {

                anchors.horizontalCenter: parent.horizontalCenter
                id: _loginButton
                width: 80
                height: 40
                text: "Login"

                KeyNavigation.tab: _host

                onClicked: {
                    Logic.setConnectionParameters(
                                _host.text,
                               _login.text,
                                _password.text,
                                _peerLogin.text,
                                _peerPassword.text
                               /* "localhost:4567",
                                "client1",
                                "1234",
                                "client2",
                                "1234"*/
                    )
                   /* Logic.setLoginParameters(_login.input.text,
                                       _password.input.text,
                                       _host.input.text)*/
                }
            }
        }


    }


}
