import QtQuick 2.0


Rectangle {
    id: root;

    property alias borderColor: root.color
    property alias innerColor: _innerRect.color


    property int borderWidth: 0

    property int lBorderwidth : borderWidth
    property int rBorderwidth : borderWidth
    property int tBorderwidth : borderWidth
    property int bBorderwidth : borderWidth

    Rectangle {
        id: _innerRect
        anchors {
            fill: parent

            leftMargin: lBorderwidth
            rightMargin: rBorderwidth
            topMargin: tBorderwidth
            bottomMargin: bBorderwidth
        }
    }
}
