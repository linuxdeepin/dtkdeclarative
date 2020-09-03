import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    BusyIndicator {
        running: true
    }

    Rectangle {
           id: frame
           clip: true
           width: 160
           height: 160
           border.color: "black"
           anchors.right: parent.right
           anchors.top: parent.top
           anchors.rightMargin: 20
           anchors.topMargin: 20

           Text {
               id: content
               text: "HELLO WORLD"
               font.pixelSize: 160
               x: -hbar.position * width
               y: -vbar.position * height
           }

           ScrollBar {
               id: vbar
               hoverEnabled: true
               active: hovered || pressed
               orientation: Qt.Vertical
               size: frame.height / content.height
               anchors.top: parent.top
               anchors.right: parent.right
               anchors.bottom: parent.bottom

               ToolTip.visible: hovered
               ToolTip.delay: 1000
               ToolTip.timeout: 5000
               ToolTip.text: "This is a ScrollBar!!!"
           }

           ScrollBar {
               id: hbar
               hoverEnabled: true
               active: hovered || pressed
               orientation: Qt.Horizontal
               size: frame.width / content.width
               anchors.left: parent.left
               anchors.right: parent.right
               anchors.bottom: parent.bottom
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

    Rectangle {
        id:swipe_view

        width: 300
        height: 150
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        SwipeView {
            id: view

            currentIndex: 1
            anchors.fill: parent

            Rectangle {
                color: "red"
                width: swipe_view.width
                height: swipe_view.height
            }

            Rectangle {
                color: "green"
                width: swipe_view.width
                height: swipe_view.height
            }

            Rectangle {
                color: "blue"
                width: swipe_view.width
                height: swipe_view.height
            }
        }

        PageIndicator {
            id: indicator

            count: view.count
            currentIndex: view.currentIndex

            anchors.bottom: view.bottom
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    StackView {
        id: stackview_1
        width: 150
        height: 150
        anchors.right: parent.right
        anchors.top: frame.bottom
        anchors.topMargin: 50
        anchors.rightMargin: 50

        Component {
             id: page
             Rectangle {
                 property real hue: Math.random()
                 color: Qt.hsla(hue, 0.5, 0.8, 0.6)
                 border.color: Qt.hsla(hue, 0.5, 0.5, 0.9)
                 StackView.visible: true
             }
        }

        initialItem: page

        MouseArea {
            id:area
            anchors.fill: parent
            onClicked: {
                stackview_1.push(page)
            }
        }
    }
    ComboBox {
        id: cmbx
        anchors.left: proBar.right
        anchors.leftMargin: 20
        anchors.top: proBar.top
        model: ["First", "Second", "Third"]
      }
}
