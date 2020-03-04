import QtQuick 2.0
import QtQuick.Layouts 1.14
import "../qml"

Rectangle {
    color: "#404040"

    width: 700
    height: 500

    ListView{
        anchors.fill: parent
        model: 4
        spacing: 15
        orientation: Qt.Horizontal
        delegate: Fader{}
    }


}

