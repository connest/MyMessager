pragma Singleton

import QtQuick 2.0
import QtQuick.Controls.Material 2.12

QtObject {
    property bool darkThreme: false

    property string primaryColor: darkThreme
                                  ? "#E91E63"
                                  : "#8BC34A"

    property string secondColor: darkThreme
                                  ? "#FF3E93"
                                  : "#6BA32A"


    property string backgroungColor: darkThreme
                                     ? "#121212"
                                     : "#F0F0F0"

    property string lowBackgroundColor: darkThreme
                                     ? "#323232"
                                     : "#DDDDDD"

    property string textColor: darkThreme
                               ? "#AAAAAA"
                               : "#333333"
}
