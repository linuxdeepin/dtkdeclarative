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
//    wmWindowTypes: DWindowManagerHelper.Desktop

    // 测试DWindow的属性
    windowRadius: 16
    borderColor: "red"
    borderWidth: 1

    DFontManager {
        id: font_manager
        fontGenericPixelSize: 13
        onFontGenericPixelSizeChanged: {
            console.log("font_manager onFontGenericPixelSizeChanged ..........");
        }
    }

    Component.onCompleted: {
        console.log(wmWindowTypes)
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

        console.log("test applicationTheme info : " , DGlobalObject.applicationTheme.window, DGlobalObject.applicationTheme.themeName);
        console.log("test systemTheme info : " , DGlobalObject.systemTheme.window, DGlobalObject.systemTheme.themeName);

        //测试DPlatformThemeProxy信号传递(控制中心切换主题测试)
        DGlobalObject.applicationTheme.themeNameChanged.connect(function(themeName) {
            console.log("applicationTheme themeNameChanged : ", themeName);
        });

        //在控制中心修改字体大小可以看到打印输出
        DGlobalObject.fontManager.fontGenericPixelSizeChanged.connect(function() {
            console.log("fontGenericPixelSizeChanged ....", DGlobalObject.fontManager.fontGenericPixelSize);
        });
        console.log("fontManager t1 字体信息", DGlobalObject.fontManager.t1);
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
                font: DGlobalObject.fontManager.t4
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
        Tab {
            title: "DItemViewport"
            Example_3 {}
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
                        font: font_manager.t7
                        text: styleData.title
                        color: styleData.selected ? "white" : "black"
                    }
                }
            }
    }
}

