import QtQuick 2.11
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0 as D

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
            MenuItem {
                text: "Cut"
                icon.name: "edit-cut"
                display: AbstractButton.TextBesideIcon
            }
            MenuItem {
                text: "Copy"
                icon.name: "edit-copy"
                display: D.IconLabel.IconBesideText
            }
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

    D.RecommandButton {
        id: highlightedionBtn
        anchors.left: warningBtn.right // @disable-check M16
        anchors.leftMargin: 20 // @disable-check M16
        anchors.top: parent.top // @disable-check M16
        anchors.topMargin: 20 // @disable-check M16
        text: "Recommand" // @disable-check M16
    }

    D.ToolButton {
        id: toolButton
        anchors.left: highlightedionBtn.right // @disable-check M16
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

    D.SearchEdit {
        id: searcherEdit
        anchors.left: lineEdit.right
        anchors.top: lineEdit.top
        anchors.leftMargin: 20
    }

    SpinBox {
        id: spinBox
        anchors.left: searcherEdit.right
        anchors.top: searcherEdit.top
        anchors.leftMargin: 20
        editable: true
        alertText: qsTr("This is a long sentence.")
        alertDuration: 2000  // millisecond
        showAlert: focus
    }

    D.PlusMinusSpinBox {
        id: plusMinusSpinBox
        anchors.left: spinBox.right
        anchors.top: spinBox.top
        anchors.leftMargin: 20
        spinBox.editable: true
        spinBox.alertText: qsTr("Only numbers can be entered.")
        spinBox.alertDuration: 2000  // millisecond
        spinBox.showAlert: focus
    }

    ListView {
        model: 5
        implicitHeight: 250

        delegate: CheckDelegate {
            text: index;

            icon.name: "button_add"
        }

        anchors.top: btnLabel.bottom
        anchors.left: plusMinusSpinBox.right
        anchors.topMargin: 20
        anchors.leftMargin: 20
    }

    Row {
        id: sliderRow
        anchors {
            top: lineEdit.bottom
            topMargin: 20
        }

        spacing: 10
        height: 350

        D.Slider {
            highlightPassArea: true
            orientation: Qt.Vertical
            height: parent.height
        }

        D.Slider {
            height: parent.height
            orientation: Qt.Vertical
            tickPosition: D.Slider.TickPosition.TicksLeft
        }

        D.Slider {
            height: parent.height
            highlightPassArea: true
            orientation: Qt.Vertical
            tickPosition: D.Slider.TickPosition.TicksRight
        }

        D.Slider {
            stepSize: 10
            from: 0
            to: 60
            tickCount: 7
            height: parent.height
            orientation: Qt.Vertical
            bothSidesTextHorizontalAlign: false
            tickPosition: D.Slider.TickPosition.TicksLeft
            tips: [qsTr("1m"), qsTr("5m"), qsTr("10m"), qsTr("15m"), qsTr("30m"), qsTr("1h"), qsTr("Never")]
        }

        D.Slider {
            stepSize: 10
            from: 0
            to: 60
            tickCount: 7
            height: parent.height
            orientation: Qt.Vertical
            bothSidesTextHorizontalAlign: false
            tickPosition: D.Slider.TickPosition.TicksRight
            tips: [qsTr("1m"), qsTr("5m"), qsTr("10m"), qsTr("15m"), qsTr("30m"), qsTr("1h"), qsTr("Never")]
        }

        D.Slider {
            tickCount: 9
            height: parent.height
            orientation: Qt.Vertical
            tickPosition: D.Slider.TickPosition.TicksRight
            tips: ["", "", "", "", "", "", "", "", ""]
        }

        D.Slider {
            tickCount: 9
            height: parent.height
            orientation: Qt.Vertical
            tickPosition: D.Slider.TickPosition.TicksLeft
            tips: ["", "", "", "", "", "", "", "", ""]
        }
    }

    Column {
        id: sliderColum
        anchors {
            top: lineEdit.bottom
            topMargin: 40
            left: sliderRow.right
            leftMargin: 60
        }
        spacing: 10
        width: 400

        D.Slider {
            highlightPassArea: true
            width: parent.width
        }

        D.Slider {
            width: parent.width
            tickPosition: Slider.TickPosition.TicksBelow
        }

        D.Slider {
            width: parent.width
            highlightPassArea: true
            tickPosition: Slider.TickPosition.TicksAbove
        }

        D.Slider {
            id: testSlider

            stepSize: 10
            from: 0
            to: 60
            tickCount: 7
            width: parent.width
            bothSidesTextHorizontalAlign: false
            tickPosition: D.Slider.TickPosition.TicksAbove
            tips: [qsTr("1m111111111111111111"), qsTr("5m11111111111111111"), qsTr("10m"), qsTr("15m111111111111111111111")
                , qsTr("30m"), qsTr("1h1111111111111111111"), qsTr("Never111111111111111111111")]

            // test end Indicator
            Rectangle {
                width: 30
                height: 30
                color: "blue"
                anchors {
                    left: parent.right
                    leftMargin: 20
                    verticalCenter: parent.handle.verticalCenter
                }

                Text {
                    anchors.centerIn: parent
                    text: testSlider.value
                    color: "white"
                }
            }

            // test start Indicator
            Rectangle {
                width: 30
                height: 30
                color: "red"
                anchors {
                    right: parent.left
                    rightMargin: 20
                    verticalCenter: parent.handle.verticalCenter
                }

                Text {
                    anchors.centerIn: parent
                    text: "S"
                    color: "white"
                }
            }
        }

        D.Slider {
            tickCount: 9
            width: parent.width
            tickPosition: D.Slider.TickPosition.TicksBelow
            tips: [qsTr("Fast"), "", "", "", "", "", "", "", qsTr("Slow")]
        }

        D.Slider {
            tickCount: 9
            width: parent.width
            tickPosition: D.Slider.TickPosition.TicksBelow
            tips: ["", "", "", "", "", "", "", "", ""]
        }
    }

    Row {
        anchors {
            top: sliderColum.bottom
            left: sliderRow.right
            topMargin: 30
            leftMargin: 40
        }
        spacing: 10
        D.Switch {
            checked: true
        }

        D.Switch {
        }

        D.Switch {
            checked: true
            enabled: false
        }

        D.Switch {
            enabled: false
        }
    }
}
