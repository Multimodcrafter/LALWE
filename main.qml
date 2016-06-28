import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

ApplicationWindow {
    id: window1
    width: 1280
    height: 720
    minimumHeight: 590
    minimumWidth: 973
    visible: true
    title: "LALWE - Learn Assembly Languages With Ease" + fileName.length > 0 ? (" - " + fileName + changesSaved ? "" : "*") : ""
    property var ramModel1: myModel1
    property var ramModel2: myModel2
    property int activeRamSlot1: 0
    property int activeRamSlot2: 0
    property int activeRegister: 10
    property bool aluActive: true
    property bool busToAluActive: true
    property bool busFromAluActive: true
    property int cycleState: 0
    property int pc: 1
    property int ind1: 2
    property int ind2: 3
    property int input: 4
    property int accu: 5
    property int opc: 6
    property int arg: 7
    property int dpt: 8
    property int spt: 9
    property int fpt: 10
    property int fla: 11
    property int operand: 17
    property int result: 22
    property string operation: "+"
    property string decodedOpcode: "Add"
    property string addressMode: "Local"
    property string effectiveAddress: "0x000000ff"
    property string dynOutput: "Dynamic output"
    property bool changesSaved: true
    property string fileName: ""
    signal saveProgram(string code, int mode)
    signal saveProgramAs(string code)
    signal openProgram()
    signal newProgram()
    signal verifyProgram(string code)
    signal assembleProgram(string code)
    signal toggleAnimations(bool newState)
    signal toggleRamHex(bool newState)

    function continueWork(mode) {
        switch(mode) {
        case 1:
            newProgram()
            textArea1.text = ""
            fileName = ""
            changesSaved = true
            break;
        case 2:
            openProgram()
            break;
        case 3:
            window1.close()
            break;
        default:
        }
    }

    MessageDialog{
        id: saveDialog
        title: "Save changes..."
        standardButtons: StandardButton.Save | StandardButton.Discard | StandardButton.Cancel
        property int mode: 0

        onAccepted: {
            saveProgram(textArea1.text, mode)
        }
        onDiscard: {
            continueWork(mode)
        }
        text: "You have made changes to your currently open program! Do you want to save those changes?"
        icon: StandardIcon.Warning
    }

    menuBar: MenuBar {
        Menu {
            title: "&File"
            MenuItem{
                text: "&New"
                shortcut: StandardKey.New
                onTriggered: {
                    if(changesSaved) {
                        newProgram()
                        textArea1.text = ""
                        fileName = ""
                        changesSaved = true
                    } else {
                        saveDialog.mode = 1
                        saveDialog.visible = true
                    }
                }
            }
            MenuItem{
                text: "&Open..."
                shortcut: StandardKey.Open
                onTriggered: {
                    if(changesSaved) {
                        openProgram()
                    } else {
                        saveDialog.mode = 2
                        saveDialog.visible = true
                    }
                }
            }
            MenuItem{
                text: "&Save"
                shortcut: StandardKey.Save
                onTriggered: saveProgram(textArea1.text,0)
            }
            MenuItem{
                text: "Save &as..."
                shortcut: "Ctrl+Shift+S"
                onTriggered: saveProgramAs(textArea1.text)
            }
            MenuItem{
                text: "&Exit"
                shortcut: "Ctrl+Q"
                onTriggered: {
                    if(changesSaved) {
                        window1.close()
                    } else {
                        saveDialog.mode = 3
                        saveDialog.visible = true
                    }
                }
            }
        }
        Menu {
            title: "&Assembler"
            MenuItem {
                text: "&Verify program"
                shortcut: "Ctrl+I"
                onTriggered: verifyProgram(textArea1.text)
            }
            MenuItem {
                text: "&Assemble program"
                shortcut: "Ctrl+T"
                onTriggered: assembleProgram(textArea1.text)
            }
        }

        Menu {
            title:"&Simulation"
            MenuItem{
                text: "&Start/Stop"
                shortcut: "Space"
            }
            MenuItem{
                text: "&Reset"
                shortcut: "Ctrl+R"
            }
            MenuItem{
                text: "Play &Animations"
                checkable: true
                shortcut: "Ctrl+Shift+A"
                onToggled: toggleAnimations(checked)
            }
        }
        Menu {
            title: "&View"
            MenuItem{
                text: "Toggle hexformat on &components"
                shortcut: "Ctrl+H"
                onTriggered: {
                    if(register1.hex || register2.hex ||
                            register3.hex || register4.hex ||
                            register5.hex || register6.hex ||
                            register7.hex || register8.hex ||
                            register9.hex || register10.hex ||
                            aLU1.hex) {
                        register1.hex = false; register2.hex = false
                        register3.hex = false; register4.hex = false
                        register5.hex = false; register6.hex = false
                        register7.hex = false; register8.hex = false
                        register9.hex = false; register10.hex = false
                        aLU1.hex = false
                    } else {
                        register1.hex = true; register2.hex = true
                        register3.hex = true; register4.hex = true
                        register5.hex = true; register6.hex = true
                        register7.hex = true; register8.hex = true
                        register9.hex = true; register10.hex = true
                        aLU1.hex = true
                    }
                }
            }
            MenuItem{
                text: "Format &RAM hexadecimal"
                checkable: true
                shortcut: "Ctrl+Shift+H"
                onToggled: toggleRamHex(checked)
            }
        }
        Menu {
            title: "&Help"
            MenuItem{
                text: "&Documentation..."
                shortcut: "F1"
            }
            MenuItem{
                text: "&About LALWE..."
            }
        }
    }

    statusBar: StatusBar{

    }

    TextArea {
        id: textArea1
        anchors.right: parent.right
        anchors.rightMargin: 928
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        onTextChanged: changesSaved = false
    }

    ColumnLayout {
        id: columnLayout1
        x: 358
        width: 224
        anchors.right: parent.right
        anchors.rightMargin: 698
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5

        RAMView {
            id: rAMView1
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillWidth: true
            rAMmodel: window1.ramModel1
            activeIndex: window1.activeRamSlot1
        }

        RAMView {
            id: rAMView2
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.fillHeight: true
            Layout.fillWidth: true
            rAMmodel: window1.ramModel2
            activeIndex: window1.activeRamSlot2
        }
    }




    Controller {
        id: controller1
        x: 628
        height: 172
        cyclestate: window1.cycleState
        address: window1.effectiveAddress
        mode: window1.addressMode
        opcode: window1.decodedOpcode
        anchors.right: parent.right
        anchors.rightMargin: 334
        anchors.top: parent.top
        anchors.topMargin: 214
    }

    Rectangle {
        id: rectangle1
        x: 588
        width: 692
        height: 147
        color: "#ffd232"
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0

        Register {
            id: register1
            name: "PC"
            active: window1.activeRegister == 0
            value: window1.pc
            x: 23
            width: 131
            height: 65
            numberSize: 22
            anchors.right: parent.right
            anchors.rightMargin: 552
            anchors.top: parent.top
            anchors.topMargin: 5
        }

        Register {
            id: register2
            name: "Indirect 1"
            active: window1.activeRegister == 1
            value: window1.ind1
            x: 160
            width: 131
            height: 65
            numberSize: 22
            anchors.right: parent.right
            anchors.rightMargin: 415
            anchors.top: parent.top
            anchors.topMargin: 5
        }

        Register {
            id: register3
            name: "Indirect 2"
            active: window1.activeRegister == 2
            value: window1.ind2
            x: 297
            width: 131
            height: 65
            numberSize: 22
            anchors.right: parent.right
            anchors.rightMargin: 278
            anchors.top: parent.top
            anchors.topMargin: 5
        }

        Register {
            id: register4
            name: "In"
            active: window1.activeRegister == 3
            value: window1.input
            x: 434
            width: 131
            height: 65
            numberSize: 22
            anchors.right: parent.right
            anchors.rightMargin: 141
            anchors.top: parent.top
            anchors.topMargin: 5
        }

        Register {
            id: register5
            name: "Accumulator"
            active: window1.activeRegister == 4
            value: window1.accu
            x: 594
            y: -142
            width: 131
            height: 65
            numberSize: 22
            anchors.right: parent.right
            anchors.rightMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
        }

        Register {
            id: register6
            name: "OP-Code"
            active: window1.activeRegister == 5
            value: window1.opc
            x: 23
            width: 131
            height: 65
            numberSize: 22
            anchors.right: parent.right
            anchors.rightMargin: 552
            anchors.top: parent.top
            anchors.topMargin: 76
        }

        Register {
            id: register7
            name: "Argument"
            active: window1.activeRegister == 6
            value: window1.arg
            x: 160
            width: 131
            height: 65
            numberSize: 22
            anchors.right: parent.right
            anchors.rightMargin: 415
            anchors.top: parent.top
            anchors.topMargin: 76
        }

        Register {
            id: register8
            name: "Data pointer"
            active: window1.activeRegister == 7
            value: window1.dpt
            x: 297
            width: 131
            height: 65
            numberSize: 22
            anchors.right: parent.right
            anchors.rightMargin: 278
            anchors.top: parent.top
            anchors.topMargin: 76
        }

        Register {
            id: register9
            name: "Stack pointer"
            active: window1.activeRegister == 8
            value: window1.spt
            x: 434
            width: 131
            height: 65
            numberSize: 22
            anchors.right: parent.right
            anchors.rightMargin: 141
            anchors.top: parent.top
            anchors.topMargin: 76
        }

        Register {
            id: register10
            name: "Frame pointer"
            active: window1.activeRegister == 9
            value: window1.fpt
            x: 594
            y: -71
            width: 131
            height: 65
            numberSize: 22
            anchors.rightMargin: 5
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.topMargin: 76
        }

    }

    ALU {
        id: aLU1
        x: 971
        width: 304
        height: 271
        hex: false
        flag: window1.fla
        active: window1.aluActive
        value1: window1.operand
        operator: window1.operation
        value2: register5.value
        value3: window1.result
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 197
        flagTextSize: 10
    }

    TextArea {
        id: textArea2
        x: 588
        width: 687
        text: window1.dynOutput
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 36
        anchors.top: parent.top
        anchors.topMargin: 474
        readOnly: true
    }

    TextField {
        id: textField1
        x: 588
        y: 695
        width: 687
        height: 25
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        placeholderText: qsTr("Dynamic input")
    }

    Bus {
        id: bus1
        active: window1.activeRegister >= 0 && window1.activeRegister < 10
        x: 690
        width: 10
        height: 67
        anchors.right: parent.right
        anchors.rightMargin: 579
        anchors.top: parent.top
        anchors.topMargin: 147
    }

    Bus {
        id: bus2
        active: window1.activeRamSlot1 >= 0 || window1.activeRamSlot2 >= 0
        x: 582
        width: 67
        height: 10
        anchors.right: parent.right
        anchors.rightMargin: 634
        anchors.top: parent.top
        anchors.topMargin: 228
    }

    Bus {
        id: bus3
        active: register5.active
        x: 1213
        y: 147
        width: 10
        height: 70
        anchors.right: parent.right
        anchors.rightMargin: 56
        anchors.top: parent.top
        anchors.topMargin: 147
    }

    Bus {
        id: bus4
        active: window1.busToAluActive
        x: 946
        width: 47
        height: 10
        anchors.right: parent.right
        anchors.rightMargin: 287
        anchors.top: parent.top
        anchors.topMargin: 223
        z: 1
    }

    Bus {
        id: bus5
        active: window1.busFromAluActive
        x: 946
        y: 368
        width: 122
        height: 10
        anchors.right: parent.right
        anchors.rightMargin: 212
        z: 2
        anchors.top: parent.top
        anchors.topMargin: 366
    }

    Bus {
        id: bus6
        active: window1.activeRegister == 10
        x: 791
        width: 10
        height: 68
        anchors.right: parent.right
        anchors.rightMargin: 479
        anchors.top: parent.top
        anchors.topMargin: 386
    }

    Bus {
        id: bus7
        active: bus6.active
        x: 801
        width: 171
        height: 10
        anchors.right: parent.right
        anchors.rightMargin: 308
        anchors.top: parent.top
        anchors.topMargin: 444
    }



}
