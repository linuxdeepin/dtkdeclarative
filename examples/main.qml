import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import com.deepin.demo 1.0

DWindow {
    id: root
    visible: true
    width: Screen.desktopAvailableWidth / 5 * 3
    height: Screen.desktopAvailableHeight / 5 * 3
    title: qsTr("dtkdeclarative")
    x:(Screen.desktopAvailableWidth - width) / 2
    y:(Screen.desktopAvailableHeight - height) / 2

    // 测试DWindow的属性
    windowRadius: 16
    borderColor: "red"
    borderWidth: 1

    Component.onCompleted: {
        console.log(root.isValid ? "DPlatformHandle有效" : "DPlatformHandle无效!!!!");
        console.log("hasBlurWindow : ", DGlobalObject.hasBlurWindow ? "true" : "false");
        console.log("windowManagerNameString : ", DGlobalObject.windowManagerNameString);

        if (DGlobalObject.windowManagerName === DWindowManagerHelper.DeepinWM) {
            console.log("windowManagerName : DeepinWM");
        } else if (DGlobalObject.windowManagerName === DWindowManagerHelper.KWinWM) {
            console.log("windowManagerName : KWinWM");
        } else {
            console.log("windowManagerName : OtherWM");
        }
    }

    Rectangle {
        id: titlebar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 45
        color: "lightgray"

        Rectangle {
            anchors.right: parent.right
            anchors.rightMargin: 10
            width: 80
            height: parent.height
            color: "red"
            Text {
                text: "CLOSE"
                font.pixelSize: 18
                anchors.fill: parent
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.close();
                }
            }
        }
    }

    TabView {
        id: frame
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: titlebar.bottom
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

