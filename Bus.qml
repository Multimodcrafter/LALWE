import QtQuick 2.5

Item {
    id: root
    property bool active: false
    Rectangle {
        id: rectangle1
        color: root.active ? "#ff4444" : "#12ff7b"
        anchors.fill: parent
    }

}
