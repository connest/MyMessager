import QtQuick 2.2

BorderRect {
    id: root

    property bool isPassword: false
    property bool isFocus: false
    readonly property TextInput input: _input


    bBorderwidth: 2
    borderColor: Style.primaryColor

    TextInput {
        id: _input
        clip: true
        anchors.fill: parent
        focus: isFocus
        //font.pointSize: root.height * 0.5
        anchors.bottom: root.bottom
        echoMode: root.isPassword
                      ? TextInput.Password
                      : TextInput.Normal
    }
}
