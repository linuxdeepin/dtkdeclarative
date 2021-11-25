import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import org.deepin.dtk.controls 1.0 as D

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

    Label {
        id: btnLabel
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
        height: btn.height
        text: qsTr("Button:")
        verticalAlignment: Text.AlignVCenter
    }

    Button {
        id: btn
        anchors.left: btnLabel.right
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
        text: "Button"
    }

    D.WarningButton {
        id: warningBtn
        anchors.top: parent.top // @disable-check M16
        anchors.topMargin: 20 // @disable-check M16
        anchors.left: btn.right // @disable-check M16
        anchors.leftMargin: 20 // @disable-check M16
        text: "Warning" // @disable-check M16
        onPressed: console.log("D.WarningButton pressed") // @disable-check M16
    }

    D.SuggestButton {
        id: suggestionBtn
        anchors.left: warningBtn.right // @disable-check M16
        anchors.leftMargin: 20 // @disable-check M16
        anchors.top: parent.top // @disable-check M16
        anchors.topMargin: 20 // @disable-check M16
        text: "Suggest" // @disable-check M16
    }

    D.ToolButton {
        id: toolButton
        anchors.left: suggestionBtn.right // @disable-check M16
        anchors.leftMargin: 20 // @disable-check M16
        anchors.top: parent.top // @disable-check M16
        anchors.topMargin: 20 // @disable-check M16
    }

    D.IconButton {
        id: iconButton
        width: 36  // @disable-check M16
        anchors.left: toolButton.right // @disable-check M16
        anchors.leftMargin: 20 // @disable-check M16
        anchors.top: parent.top // @disable-check M16
        anchors.topMargin: 20 // @disable-check M16
        icon.name: "org.deepin.Example/test/button_add"
    }

    D.IconButton {
        id: iconButton1
        width: 36  // @disable-check M16
        anchors.left: iconButton.right // @disable-check M16
        anchors.leftMargin: 20 // @disable-check M16
        anchors.top: parent.top // @disable-check M16
        anchors.topMargin: 20 // @disable-check M16
        circle: true // @disable-check M16
        icon.name: "test/button_add"  // @disable-check M16
    }

    D.IconButton {
        id: iconButton2
        width: 36  // @disable-check M16
        anchors.left: iconButton1.right // @disable-check M16
        anchors.leftMargin: 20 // @disable-check M16
        anchors.top: parent.top // @disable-check M16
        anchors.topMargin: 20 // @disable-check M16
        flat: true // @disable-check M16
        icon.name: "button_add"  // @disable-check M16
    }

    D.FloatingButton {
        id: floatingButton
        width: 36  // @disable-check M16
        anchors.left: iconButton2.right // @disable-check M16
        anchors.leftMargin: 20 // @disable-check M16
        anchors.top: parent.top // @disable-check M16
        anchors.topMargin: 20 // @disable-check M16
        circle: true // @disable-check M16
        icon.name: "button_add"
    }

    DelayButton {
        id: dlyBtn
        anchors.left: floatingButton.right
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.topMargin: 20
        text: "DelayButton"

        NumberAnimation on progress {
            from: 0
            to: 1.0
            duration: 4000
            loops: Animation.Infinite
        }
    }

    RoundButton {
        id: roundButton;
        anchors.top: parent.top
        anchors.left: dlyBtn.right
        anchors.leftMargin: 20
        anchors.topMargin: 20

        icon.name: "button_add"
    }

    D.ButtonBox {
        id: buttonBox
        anchors.left: roundButton.right // @disable-check M16
        anchors.leftMargin: 20 // @disable-check M16
        anchors.top: parent.top // @disable-check M16
        anchors.topMargin: 20  // @disable-check M16

        D.ButtonBoxButton {
            icon.name: "go-previous" // @disable-check M16
        }
        D.ButtonBoxButton {
            icon.name: "go-down" // @disable-check M16
        }
        D.ButtonBoxButton {
            icon.name: "go-next" // @disable-check M16
        }
    }

    D.LineEdit {
        id: lineEdit
        anchors.left: parent.left
        anchors.top: btnLabel.bottom
        anchors.topMargin: 20
        text: "Testing the alert message in line edit."

        alertText: qsTr("This is a long sentence.")
        alertDuration: 2000  // millisecond
        showAlert: focus
    }

    ListView {
        model: 5
        implicitHeight: 250

        delegate: CheckDelegate {
            text: index;

            icon.name: "button_add"
        }

        anchors.top: btnLabel.bottom
        anchors.left: lineEdit.right
        anchors.topMargin: 20
        anchors.leftMargin: 20
    }
}
