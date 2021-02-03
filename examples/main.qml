import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4 as V2
import QtQuick.Layouts 1.11
import com.deepin.dtk 1.0

V2.ApplicationWindow {
    id: root
    visible: true
    width: Screen.desktopAvailableWidth / 5 * 3
    height: Screen.desktopAvailableHeight / 5 * 3
    title: qsTr("dtkdeclarative")
    x:(Screen.desktopAvailableWidth - width) / 2
    y:(Screen.desktopAvailableHeight - height) / 2
//    DWindow.wmWindowTypes: DWindowManagerHelper.DesktopType

    // 测试DWindow的属性
    DWindow.enabled: true
    DWindow.windowRadius: 16
    DWindow.borderColor: palette.highlight
    DWindow.borderWidth: 1

    DFontManager {
        id: font_manager
        baseFont: font_manager.get(13, Qt.application.font)
        onFontChanged: {
            console.log("font_manager fontChanged ..........");
        }
    }

    Component.onCompleted: {
        console.log(DWindow.wmWindowTypes)
        console.log(root.isValid ? "DPlatformHandle有效" : "DPlatformHandle无效!!!!");
        console.log("hasBlurWindow : ", DTK.hasBlurWindow ? "true" : "false");
        console.log("windowManagerNameString : ", DTK.windowManagerNameString);

        // 测试系统亮暗色（枚举类型）
        if (DTK.themeType === DApplicationHelper.LightType) {
            console.log("themeType : Light Theme");
        } else if (DTK.themeType === DApplicationHelper.DarkType) {
            console.log("themeType : Dark Theme");
        } else {
            console.log("themeType : Unknown Theme");
        }
        DTK.themeTypeChanged.connect(function() {
            console.log("themeNameChanged : ", DTK.themeType);
        });

        if (DTK.windowManagerName === DWindowManagerHelper.DeepinWM) {
            console.log("windowManagerName : DeepinWM");
        } else if (DTK.windowManagerName === DWindowManagerHelper.KWinWM) {
            console.log("windowManagerName : KWinWM");
        } else {
            console.log("windowManagerName : OtherWM");
        }

        console.log("test platformTheme info : " , DTK.platformTheme.window, DTK.platformTheme.themeName);

        //测试DPlatformThemeProxy信号传递(控制中心切换主题测试)
        DTK.platformTheme.themeNameChanged.connect(function(themeName) {
            console.log("platformTheme themeNameChanged : ", themeName);
        });

        //在控制中心修改字体大小可以看到打印输出
        DTK.fontManager.fontChanged.connect(function() {
            console.log("base font changed ....", DTK.fontManager.baseFont);
        });
        console.log("fontManager t1 字体信息", DTK.fontManager.t1);

        // 测试 DPalette 值
        console.log("DPalette window value: ", root.palette.window)
        console.log("DPalette windowText value: ", root.palette.windowText)
        console.log("DPalette base value: ", root.palette.base)
        console.log("DPalette itemBackground value: ", root.palette.itemBackground)
        console.log("DPalette textTitle value: ", root.palette.textTitle)
        console.log("DPalette textTips value: ", root.palette.textTips)
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
                font: DTK.fontManager.t4
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
            text: qsTr("DItemViewport")
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
