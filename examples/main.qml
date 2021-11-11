import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4 as V2
import QtQuick.Layouts 1.11
import org.deepin.dtk 1.0 as D

V2.ApplicationWindow {
    id: root
    visible: true
    width: Screen.desktopAvailableWidth / 5 * 3
    height: Screen.desktopAvailableHeight / 5 * 3
    title: qsTr("dtkdeclarative")
    x:(Screen.desktopAvailableWidth - width) / 2
    y:(Screen.desktopAvailableHeight - height) / 2
//    D.Window.wmWindowTypes: D.WindowManagerHelper.DesktopType

    // 测试D.Window的属性
    D.Window.enabled: true
    D.Window.windowRadius: 16
    D.Window.borderColor: palette.highlight
    D.Window.borderWidth: 1

    D.FontManager {
        id: font_manager
        baseFont: font_manager.get(13, Qt.application.font)
        onFontChanged: {
            console.log("font_manager fontChanged ..........");
        }
    }

    Component.onCompleted: {
        console.log(D.Window.wmWindowTypes)
        console.log(root.isValid ? "DPlatformHandle有效" : "DPlatformHandle无效!!!!");
        console.log("hasBlurWindow : ", D.DTK.hasBlurWindow ? "true" : "false");
        console.log("windowManagerNameString : ", D.DTK.windowManagerNameString);

        // 测试系统亮暗色（枚举类型）
        if (D.DTK.themeType === D.ApplicationHelper.LightType) {
            console.log("themeType : Light Theme");
        } else if (D.DTK.themeType === D.ApplicationHelper.DarkType) {
            console.log("themeType : Dark Theme");
        } else {
            console.log("themeType : Unknown Theme");
        }
        D.DTK.themeTypeChanged.connect(function() {
            console.log("themeNameChanged : ", D.DTK.themeType);
        });

        if (D.DTK.windowManagerName === D.WindowManagerHelper.DeepinWM) {
            console.log("windowManagerName : DeepinWM");
        } else if (D.DTK.windowManagerName === D.WindowManagerHelper.KWinWM) {
            console.log("windowManagerName : KWinWM");
        } else {
            console.log("windowManagerName : OtherWM");
        }

        console.log("test platformTheme info : " , D.DTK.platformTheme.window, D.DTK.platformTheme.themeName);

        //测试DPlatformThemeProxy信号传递(控制中心切换主题测试)
        D.DTK.platformTheme.themeNameChanged.connect(function(themeName) {
            console.log("platformTheme themeNameChanged : ", themeName);
        });

        //在控制中心修改字体大小可以看到打印输出
        D.DTK.fontManager.fontChanged.connect(function() {
            console.log("base font changed ....", D.DTK.fontManager.baseFont);
        });
        console.log("fontManager t1 字体信息", D.DTK.fontManager.t1);

        // 测试 D.Palette 值
        console.log("D.Palette window value: ", root.palette.window)
        console.log("D.Palette windowText value: ", root.palette.windowText)
        console.log("D.Palette base value: ", root.palette.base)
        console.log("D.Palette itemBackground value: ", root.palette.itemBackground)
        console.log("D.Palette textTitle value: ", root.palette.textTitle)
        console.log("D.Palette textTips value: ", root.palette.textTips)
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
                font: D.DTK.fontManager.t4
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

    V2.TabBar {
        id: tabBar
        anchors.top: titlebar.bottom
        V2.TabButton {
            text: qsTr("Example_1")
        }
        V2.TabButton {
            text: qsTr("Example_2")
        }
        V2.TabButton {
            text: qsTr("ItemViewport")
        }
    }
    StackLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: tabBar.bottom
        anchors.bottom: parent.bottom
        anchors.margins: 4

        currentIndex: tabBar.currentIndex
        Example_1 {}
        Example_2 {}
        Example_3 {}
    }
}
