pragma Singleton

import QtQuick 2.0
import QtQuick.Controls.Material 2.12

QtObject {
    property bool darkThreme: false

    property string primaryColor: darkThreme
                                 ? "#E91E63"
                                 : "#8BC34A"
}
