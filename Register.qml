import QtQuick 2.5
import QtQuick.Controls 1.5

Item {
    id: root
    property int value: 0
    property int numberSize: 30
    property int nameSize: 12
    property bool hex: false
    property bool active: false
    property string name: "Name"
    onHexChanged: checkHex.checked = hex
    Rectangle {
        id: backgound
        color: root.active ? "#ff4444" : "#12ff7b"
        anchors.fill: parent

        Text {
            id: displayValue
            text: root.hex ? "0x" + Number(root.value).toString(16): root.value
            anchors.top: displayName.bottom
            anchors.right: parent.right
            anchors.bottom: checkHex.top
            anchors.left: parent.left
            font.pixelSize: root.numberSize
            anchors.margins: 10
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        CheckBox {
            id: checkHex
            text: qsTr("Format hexadecimal")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.horizontalCenter: parent.horizontalCenter
            onCheckedChanged: root.hex = checked
            checked: root.hex
        }

        Text {
            id: displayName
            text: root.name
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 5
            font.pixelSize: root.nameSize
        }
    }

}
