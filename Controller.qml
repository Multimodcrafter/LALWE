import QtQuick 2.5
import QtQuick.Layouts 1.3

Item {
    width: 300
    height: 150
    id: root
    property int cyclestate: 1
    property string opcode: "N/A"
    property string mode: "N/A"
    property string address: "N/A"

    Rectangle {
        id: rectangle10
        color: "#ffa4a4"
        anchors.topMargin: 0
        anchors.fill: parent

        ColumnLayout {
            id: columnLayout1
            anchors.rightMargin: 5
            anchors.leftMargin: 5
            anchors.bottomMargin: 5
            anchors.topMargin: 5
            anchors.fill: parent

            Text {
                id: text10
                text: qsTr("Controller")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.bold: true
                Layout.fillHeight: false
                Layout.fillWidth: true
                font.pixelSize: 17
            }

            RowLayout {
                id: rowLayout1
                width: 624
                height: 0
                Layout.fillHeight: true
                Layout.fillWidth: true

                Rectangle {
                    id: rectangle1
                    width: 200
                    height: 200
                    color: root.cyclestate == 0 ? "#ff4444" : "#44ffa2"
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Text {
                        id: text7
                        text: qsTr("Fetch")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.fill: parent
                        font.pixelSize: 12
                    }
                }

                Rectangle {
                    id: rectangle2
                    width: 200
                    height: 200
                    color: root.cyclestate == 1 ? "#ff4444" : "#44ffa2"
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Text {
                        id: text8
                        text: qsTr("Decode")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.fill: parent
                        font.pixelSize: 12
                    }
                }

                Rectangle {
                    id: rectangle3
                    width: 200
                    height: 200
                    color: root.cyclestate == 2 ? "#ff4444" : "#44ffa2"
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Text {
                        id: text9
                        text: qsTr("Execute")
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        anchors.fill: parent
                        font.pixelSize: 12
                    }
                }
            }


            ColumnLayout {
                id: columnLayout2
                width: 100
                height: 131
                Layout.fillHeight: false
                Layout.fillWidth: true

                RowLayout {
                    id: rowLayout2
                    width: 100
                    height: 100
                    spacing: 0
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Rectangle {
                        id: rectangle4
                        width: 200
                        height: 25
                        color: "#56abff"
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        Text {
                            id: text1
                            text: qsTr("Decoded instruction")
                            anchors.rightMargin: 5
                            anchors.leftMargin: 5
                            anchors.bottomMargin: 0
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            font.pixelSize: 12
                        }
                    }

                    Rectangle {
                        id: rectangle5
                        width: 200
                        height: 25
                        color: "#56abff"
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        Text {
                            id: text2
                            text: root.opcode
                            anchors.rightMargin: 5
                            anchors.leftMargin: 5
                            anchors.fill: parent
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            Layout.fillHeight: true
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            font.pixelSize: 12
                        }
                    }
                }

                RowLayout {
                    id: rowLayout3
                    width: 100
                    height: 100
                    spacing: 0
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Rectangle {
                        id: rectangle6
                        width: 200
                        height: 25
                        color: "#56abff"
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        Text {
                            id: text3
                            text: qsTr("Address calculation mode")
                            anchors.rightMargin: 0
                            anchors.leftMargin: 5
                            anchors.fill: parent
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            font.pixelSize: 12
                            Layout.fillWidth: true
                        }
                    }

                    Rectangle {
                        id: rectangle7
                        width: 200
                        height: 25
                        color: "#56abff"
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        Text {
                            id: text4
                            text: root.mode
                            anchors.rightMargin: 5
                            anchors.leftMargin: 5
                            anchors.fill: parent
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            font.pixelSize: 12
                            Layout.fillWidth: true
                        }
                    }
                }

                RowLayout {
                    id: rowLayout4
                    width: 100
                    height: 100
                    spacing: 0
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Rectangle {
                        id: rectangle8
                        width: 200
                        height: 25
                        color: "#56abff"
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        Text {
                            id: text5
                            text: qsTr("Effective address")
                            anchors.rightMargin: 5
                            anchors.leftMargin: 5
                            anchors.fill: parent
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            font.pixelSize: 12
                            Layout.fillWidth: true
                        }
                    }

                    Rectangle {
                        id: rectangle9
                        x: 323
                        width: 200
                        height: 25
                        color: "#56abff"
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        Text {
                            id: text6
                            text: root.address
                            anchors.rightMargin: 5
                            anchors.leftMargin: 5
                            anchors.fill: parent
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            font.pixelSize: 12
                            Layout.fillWidth: true
                        }
                    }


                }
            }


        }
    }

}
