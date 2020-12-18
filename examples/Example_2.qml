import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import com.deepin.demo 1.0


Rectangle {

    RowLayout {
        id:row_layout_1
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        DIcon {
            name: "dde-file-manager"
            iconSize: Qt.size(100, 100)
        }
        DIcon {
            name: "edit"
            iconSize: Qt.size(100, 100)
            color: "red"
        }

        BusyIndicator {
            running: true
        }
        BusyIndicator {
            running: true
        }

        RoundButton {
            id:roundbutton
            width: 28
            height: 28
        }

        Rectangle {
               id: frame
               clip: true
               width: 160
               height: 160
               border.color: "black"

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


        StackView {
            id: stackview_1
            width: 150
            height: 150

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
}
