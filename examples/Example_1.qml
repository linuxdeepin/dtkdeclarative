import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

Rectangle {

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
