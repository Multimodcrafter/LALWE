import QtQuick 2.5
import QtQuick.Extras 1.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.5

Item {
    id: root
    width: 510
    height: 450
    property bool active: false
    property bool hex: false
    property int numberSize: 16
    property int flagTextSize: 20
    property int value1: 0
    property int value2: 0
    property int value3: 0
    property int flag: 0
    property string operator: "+"
    onHexChanged: checkBox1.checked = hex
    Image {
        id: image1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 48
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        sourceSize.height: 402
        sourceSize.width: 510
        source: "ALU-Graphic.png"

        Text {
            id: text1
            x: parent.width / 13.5
            y: parent.height / 11.17
            width: parent.width / 2.83
            height: parent.height / 9.57
            text: root.hex ? "0x" + Number(root.value1).toString(16): root.value1
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: root.numberSize
        }

        Text {
            id: text2
            x: parent.width / 1.75
            y: parent.height / 11.17
            width: parent.width / 2.83
            height: parent.height / 9.57
            text: root.hex ? "0x" + Number(root.value2).toString(16): root.value2
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: root.numberSize
        }

        Text {
            id: text3
            x: parent.width / 3.13
            y: parent.height / 1.38
            width: parent.width / 2.83
            height: parent.height / 8.74
            text: root.hex ? "0x" + Number(root.value3).toString(16): root.value3
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: root.numberSize
        }

        Text {
            id: text9
            x: parent.width / 2.24
            y: parent.height / 2.25
            width: parent.width / 9.44
            height: parent.height / 7.18
            text: root.operator
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: root.flagTextSize

            Rectangle {
                id: rectangle2
                color: "#ff04ff"
                visible: root.active
                z: -1
                anchors.rightMargin: 2
                anchors.leftMargin: 2
                anchors.bottomMargin: 2
                anchors.topMargin: 2
                anchors.fill: parent
            }
        }

        CheckBox {
            id: checkBox1
            x: 435
            y: 377
            text: qsTr("Format hexadecimal")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 8
            onCheckedChanged: root.hex = checked
            checked: root.hex
        }
    }

    Rectangle {
        id: rectangle1
        color: "#ffa312"
        anchors.right: parent.right
        anchors.rightMargin: 1
        anchors.left: parent.left
        anchors.leftMargin: 1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 1
        anchors.top: image1.bottom
        anchors.topMargin: 1

        RowLayout {
            id: rowLayout1
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent

            StatusIndicator {
                id: statusIndicator1
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.minimumHeight: 1
                Layout.fillHeight: true;
                Layout.fillWidth: true;
                active: (root.flag & 16) != 0
            }

            Text {
                id: text4
                text: qsTr("Sign")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: root.flagTextSize
                Layout.fillHeight: true;
                Layout.fillWidth: true;
            }

            StatusIndicator {
                id: statusIndicator2
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true;
                Layout.fillWidth: true;
                active: (root.flag & 8) != 0
            }

            Text {
                id: text5
                text: qsTr("Parity")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: root.flagTextSize
                Layout.fillHeight: true;
                Layout.fillWidth: true;
            }

            StatusIndicator {
                id: statusIndicator3
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true;
                Layout.fillWidth: true;
                active: (root.flag & 4) != 0
            }

            Text {
                id: text6
                text: qsTr("Zero")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: root.flagTextSize
                Layout.fillHeight: true;
                Layout.fillWidth: true;
            }

            StatusIndicator {
                id: statusIndicator4
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true;
                Layout.fillWidth: true;
                active: (root.flag & 2) != 0
            }

            Text {
                id: text7
                text: qsTr("Overflow")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: root.flagTextSize
                Layout.fillHeight: true;
                Layout.fillWidth: true;
            }

            StatusIndicator {
                id: statusIndicator5
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.fillHeight: true;
                Layout.fillWidth: true;
                active: (root.flag & 1) != 0
            }

            Text {
                id: text8
                text: qsTr("Carry")
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: root.flagTextSize
                Layout.fillHeight: true;
                Layout.fillWidth: true;
            }
        }
    }















}
