import QtQuick 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

/*BorderRect {
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
        anchors.bottom: root.bottom
        echoMode: root.isPassword
                      ? TextInput.Password
                      : TextInput.Normal

        color: Style.textColor

    }
}
*/

TextField {

    style: TextFieldStyle {
        textColor: Style.textColor
        background: BorderRect {
            bBorderwidth: 2
            borderColor: Style.primaryColor
            innerColor: Style.lowBackgroundColor
        }
    }
}
