import QtQuick 2.0
import QtQuick.Controls 2.0
// 确保dtk的模块在最后被引入
import org.deepin.dtk 1.0

Column {
    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        text: "按钮控件最为常用，所有类型的按钮都继承于 AbstractButton，可以为其设置文本和图标图标等内容，当使用 DCI 格式的图标时，图标的颜色可以跟随按钮的文字颜色一起改变。"
        horizontalAlignment: Qt.AlignHCenter
    }

    spacing: 10

    Flow {
        spacing: 10
        width: parent.width

        Button {
            text: "button"
        }
        Button {
            ColorSelector.hovered: true
            ColorSelector.pressed: false
            ColorSelector.disabled: false
            text: "button hover"
            enabled: false
        }
        Button {
            ColorSelector.pressed: true
            text: "button pressed"
        }
        Button {
            text: "button disabled"
            enabled: false
        }

        // break row
        Item {width: parent.width; height: 1}

        WarningButton {
            text: "warning"
        }
        WarningButton {
            ColorSelector.hovered: true
            ColorSelector.pressed: false
            ColorSelector.disabled: false
            text: "warning hover"
            enabled: false
        }
        WarningButton {
            ColorSelector.pressed: true
            text: "warning pressed"
        }
        WarningButton {
            text: "warning disabled"
            enabled: false
        }

        // break row
        Item {width: parent.width; height: 1}

        RecommandButton {
            text: "recommand"
        }
        RecommandButton {
            ColorSelector.hovered: true
            ColorSelector.pressed: false
            ColorSelector.disabled: false
            text: "recommand hover"
            enabled: false
        }
        RecommandButton {
            ColorSelector.pressed: true
            text: "recommand pressed"
        }
        RecommandButton {
            text: "recommand disabled"
            enabled: false
        }

        // break row
        Item {width: parent.width; height: 1}

        Button {
            text: "button"
            checkable: true
            checked: true
        }
        Button {
            ColorSelector.hovered: true
            ColorSelector.pressed: false
            ColorSelector.disabled: false
            text: "button hover"
            enabled: false
            checked: true
        }
        Button {
            ColorSelector.pressed: true
            text: "button pressed"
            checked: true
        }
        Button {
            text: "button disabled"
            enabled: false
            checked: true
        }
    }
}
