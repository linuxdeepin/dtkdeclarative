import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

import com.deepin.dtk 1.0

import "common"

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

    DLineEdit {
        id: lineEdit
        anchors.left: txtFld.right
        anchors.leftMargin: 20
        anchors.top: txtFld.top
        placeholderText: qsTr("This is DLineEdit")
    }

    DSearchEdit {
        id: srchDt
        anchors.left: lineEdit.left
        anchors.top: lineEdit.bottom
        anchors.topMargin: 10
        placeholder: qsTr("search")
    }

    DPasswordEdit {
        id: pswdDt
        anchors.left: srchDt.left
        anchors.top: srchDt.bottom
        anchors.topMargin: 20
    }

    TextArea {
        id: txtArea
        anchors.left: chckDlgt.right
        anchors.leftMargin: 50
        anchors.top: txtFld.bottom
        anchors.topMargin: 20
        placeholderText: qsTr("This is TextArea")
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

    SpinBox {
        id: spnBx
        anchors.left: dlgBtnBx.right
        anchors.leftMargin: 20
        anchors.verticalCenter: dlyBtn.verticalCenter

//        LayoutMirroring.enabled: true
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

    DProgressBar {
        id: dpbar1
        width: 50
        height: 100
        color: "gray"
        radius: 20
        value: 1
        orientation: Qt.Vertical
        anchors.left: gpBx.right
        anchors.bottom: gpBx.bottom
        anchors.leftMargin: 20

        DOSDSlider {
            anchors.fill: parent
            backgroundColor: Qt.rgba(1, 0, 0, 1)
            radius: 20
            value: 0
            orientation: Qt.Vertical
        }
    }

    DProgressBar {
        id: dpbar2
        width: 100
        height: 50
        color: Qt.rgba(0, 1, 0, 1)
        radius: 20
        value: slider2.value
        orientation: Qt.Horizontal
        rotation: 180

        anchors.left: dpbar1.right
        anchors.leftMargin: 20
        anchors.verticalCenter: gpBx.verticalCenter
    }

    Slider {
        id: slider2
        width: 100
        height: 40
        anchors.left: dpbar2.right
        anchors.leftMargin: 20
        anchors.verticalCenter: gpBx.verticalCenter
    }

    DSlider {
        id: dslider
        width: 100
        height: 20
        stepSize: 0.1
        leftPadding: 0
        handelWidth: 24
        handelHeight: 28
        handelRadius: 8
        grooveColor: "gray"
        activeColor: "blue"
        grooveSize: 8
        anchors.left: slider2.right
        anchors.leftMargin: 20
        anchors.verticalCenter: gpBx.verticalCenter
    }

    DImage {
        id: image1
        source: "qrc:/res/battery.svg"
        sourceSize: Qt.size(128, 128)
        specifyColor: Qt.rgba(1, 0, 0, 1)
        anchors.left: dslider.right
        anchors.leftMargin: 20
        anchors.verticalCenter: gpBx.verticalCenter
        MouseArea {
            anchors.fill: parent

            onClicked: {
                if (image1.renderTpye === DImage.AutoRender)
                    image1.renderTpye = DImage.RenderWithColor
                else
                    image1.renderTpye = DImage.AutoRender
            }
        }
    }

    DImage {
        id: image2
        source: "qrc:/res/3.jpg"
        radius: 30
        sourceSize: Qt.size(128, 128)
        specifyColor: Qt.rgba(1, 0, 0, 1)
        anchors.left: image1.right
        anchors.leftMargin: 20
        anchors.verticalCenter: gpBx.verticalCenter
        MouseArea {
            anchors.fill: parent

            onClicked: {
                if (image2.renderTpye === DImage.AutoRender)
                    image2.renderTpye = DImage.RenderWithColor
                else
                    image2.renderTpye = DImage.AutoRender
            }
        }
    }
}
