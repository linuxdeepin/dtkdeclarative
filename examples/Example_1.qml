import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

Rectangle {

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            if (mouse.button === Qt.RightButton)
                contextMenu.popup()
        }
        onPressAndHold: {
            if (mouse.source === Qt.MouseEventNotSynthesized)
                contextMenu.popup()
        }

        Menu {
            id: contextMenu
            MenuItem { text: "Cut" }
            MenuItem { text: "Copy" }
            MenuItem {
                text: "Paste"
                checkable: true
                checked: true
            }
            MenuSeparator { }
            Menu {
                title: "Find/Replace"
                Action { text: "Find Next" }
                Action { text: "Find Previous" }
                Action { text: "Replace" }
            }
            MenuItem { text: "Exit" }
        }
    }

    ColumnLayout {
        id: radioFrame
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 20
        anchors.topMargin: 20

        RadioButton {
            checked: true
            focus: true
            text: qsTr("First")
        }
        RadioButton {
            text: qsTr("Second")
        }
        RadioButton {
            text: qsTr("Third")
        }
    }

    ColumnLayout {
        id: chckBx
        anchors.left: radioFrame.right
        anchors.leftMargin: 20
        anchors.verticalCenter: radioFrame.verticalCenter

        CheckBox {
            checked: true
            focus: true
            text: qsTr("First")
        }
        CheckBox {
            text: qsTr("Second")
        }
        CheckBox {
            text: qsTr("Third")
        }
    }

    ColumnLayout {
        id: swtch
        anchors.left: chckBx.right
        anchors.leftMargin: 20
        anchors.verticalCenter: radioFrame.verticalCenter

        Switch {
            width: 120
            text: qsTr("Wi-Fi")
        }
        Switch {
            text: qsTr("Bluetooth")
        }
    }

    ListView {
        id: chckDlgt
        anchors.left: swtch.right
        anchors.leftMargin: 20
        anchors.verticalCenter: radioFrame.verticalCenter

        width: 100
        height: 100

        model: ["Option 1", "Option 2", "Option 3"]
        delegate: CheckDelegate {
            text: modelData
            highlighted : true
            icon.name: "search_indicator"
            icon.color: "red"
        }
    }

    TextField {
        id: txtFld
        anchors.left: chckDlgt.right
        anchors.leftMargin: 50
        anchors.top: chckDlgt.top
        placeholderText: qsTr("This is TextField")
    }

    Slider {
        id: slider
        anchors.left: parent.left
        anchors.top: radioFrame.bottom
        orientation: Qt.Horizontal
        stepSize: 10
        from: 0
        value: 30
        to: 100
    }

    ProgressBar {
        id: proBar
        anchors.left: slider.right
        anchors.top: radioFrame.bottom
        anchors.leftMargin: 20
        anchors.margins: 20
        from: 0
        to: 100
        value: 80
        //        indeterminate: true
    }

    ComboBox {
        id: cmbx
        anchors.left: proBar.right
        anchors.leftMargin: 20
        anchors.top: proBar.top
        model: ["First", "Second", "Third"]
    }

    Button {
        id: btn
        anchors.left: cmbx.right
        anchors.leftMargin: 20
        anchors.top: proBar.top
        text: "Button"
    }

    DelayButton {
        id: dlyBtn
        anchors.left: btn.right
        anchors.leftMargin: 20
        anchors.top: proBar.top
        text: "DelayButton"

        NumberAnimation on progress {
            from: 0
            to: 1.0
            duration: 4000
            loops: Animation.Infinite
        }
    }

    DialogButtonBox {
        id: dlgBtnBx
        anchors.left: dlyBtn.right
        anchors.leftMargin: 20
        anchors.verticalCenter: dlyBtn.verticalCenter
        standardButtons: DialogButtonBox.Ok | DialogButtonBox.Cancel

        onAccepted: console.log("Ok clicked")
        onRejected: console.log("Cancel clicked")
    }

    GroupBox {
        id: gpBx
        width: 180
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: slider.bottom
        title: qsTr("Synchronize")
        ColumnLayout {
            anchors.fill: parent
            CheckBox { text: qsTr("E-mail") }
            CheckBox { text: qsTr("Calendar") }
            CheckBox { text: qsTr("Contacts") }
        }
    }
}
