import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4


Window {
    visible: true
    width: Screen.desktopAvailableWidth / 5 * 3
    height: Screen.desktopAvailableHeight / 5 * 3
    title: qsTr("dtkdeclarative")
    x:(Screen.desktopAvailableWidth - width) / 2
    y:(Screen.desktopAvailableHeight - height) / 2

    TabView {
        id: frame
        anchors.fill: parent
        anchors.margins: 4
        Tab {
            title: "Example_1"
            Example_1 {}
        }
        Tab {
            title: "Example_2"
            Example_2 {}
        }

        SystemPalette { id: myPalette; colorGroup: SystemPalette.Active }

        style: TabViewStyle {
                frameOverlap: 1
                tab: Rectangle {
                    color: styleData.selected ? myPalette.highlight : "lightgray"
                    implicitWidth: Math.max(text.width + 4, 100)
                    implicitHeight: 30
                    radius: 2
                    Text {
                        id: text
                        anchors.centerIn: parent
                        text: styleData.title
                        color: styleData.selected ? "white" : "black"
                    }
                }
            }
    }
}

