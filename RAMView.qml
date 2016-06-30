import QtQuick 2.5
import QtQuick.Controls 1.5

Item {
    id: item1
    width: 300
    height: 512
    property alias button1: button1
    property alias button2: button2
    property alias button3: button3
    property alias textField1: textField1
    property var rAMmodel
    property int activeIndex: 0
    property int lastactiveIndex: 0

    onActiveIndexChanged: {
        if(activeIndex > -1) {
            listView1.positionViewAtIndex(activeIndex, ListView.Contain);
            lastactiveIndex = activeIndex;
        } else {
            listView1.positionViewAtIndex(lastactiveIndex, ListView.Contain);
        }
    }

    TextField {
        id: textField1
        text: ""
        anchors.bottom: button2.top
        anchors.bottomMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: button1.left
        anchors.rightMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 3
        placeholderText: qsTr("Address")
    }

    Button {
        id: button1
        x: 174
        width: 105
        height: 22
        text: qsTr("Go")
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.top: parent.top
        anchors.topMargin: 0
    }

    ListView {
        id: listView1
        anchors.right: parent.right
        anchors.rightMargin: 3
        anchors.left: parent.left
        anchors.leftMargin: 3
        anchors.bottom: button3.top
        anchors.bottomMargin: 5
        anchors.top: button2.bottom
        anchors.topMargin: 5
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        currentIndex: 0
        focus: true
        z: -1

        model: rAMmodel

        onModelChanged: {
            if(activeIndex > -1) {
                listView1.positionViewAtIndex(activeIndex, ListView.Contain);
                lastactiveIndex = activeIndex;
            } else {
                listView1.positionViewAtIndex(lastactiveIndex, ListView.Contain);
            }
        }

        delegate: MouseArea {
            id: wrapper
            height: 20
            width: parent.width
            onClicked: {
                listView1.currentIndex = index
                listView1.focus = true
                textField1.focus = false
            }
            Rectangle{
                color: index == item1.activeIndex ? "#ff4444" : (wrapper.ListView.isCurrentItem ? "#45D8F9" : (index % 2 == 0 ? "#B7F7FD" : "#B7E3FD"))
                anchors.fill: parent
                Text {
                    text: modelData
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    font.bold: wrapper.ListView.isCurrentItem || index == item1.activeIndex
                }
            }
        }
    }

    Button {
        id: button2
        text: qsTr("<<<")
        anchors.top: button1.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 3
        anchors.right: parent.right
        anchors.rightMargin: 3
    }

    Button {
        id: button3
        x: -3
        y: -8
        text: qsTr(">>>")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.rightMargin: 3
        anchors.left: parent.left
        anchors.leftMargin: 3
        anchors.right: parent.right
    }






}
