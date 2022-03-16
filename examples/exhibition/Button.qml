import QtQuick 2.0
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

    readonly property int buttonMinWidth: 204

    Flow {
        spacing: 10
        width: parent.width


        Button {
            width: Math.max(implicitWidth, buttonMinWidth)
            text: "button"
        }
        Button {
            width: Math.max(implicitWidth, buttonMinWidth)
            ColorSelector.hovered: true
            ColorSelector.pressed: false
            ColorSelector.disabled: false
            text: "button hover"
            enabled: false
        }
        Button {
            width: Math.max(implicitWidth, buttonMinWidth)
            ColorSelector.pressed: true
            text: "button pressed"
        }
        Button {
            width: Math.max(implicitWidth, buttonMinWidth)
            text: "button disabled"
            enabled: false
        }

        // break row
        Item {width: parent.width; height: 1}

        WarningButton {
            width: Math.max(implicitWidth, buttonMinWidth)
            text: "warning"
        }
        WarningButton {
            width: Math.max(implicitWidth, buttonMinWidth)
            ColorSelector.hovered: true
            ColorSelector.pressed: false
            ColorSelector.disabled: false
            text: "warning hover"
            enabled: false
        }
        WarningButton {
            width: Math.max(implicitWidth, buttonMinWidth)
            ColorSelector.pressed: true
            text: "warning pressed"
        }
        WarningButton {
            width: Math.max(implicitWidth, buttonMinWidth)
            text: "warning disabled"
            enabled: false
        }

        // break row
        Item {width: parent.width; height: 1}

        RecommandButton {
            width: Math.max(implicitWidth, buttonMinWidth)
            text: "recommand"
        }
        RecommandButton {
            width: Math.max(implicitWidth, buttonMinWidth)
            ColorSelector.hovered: true
            ColorSelector.pressed: false
            ColorSelector.disabled: false
            text: "recommand hover"
            enabled: false
        }
        RecommandButton {
            width: Math.max(implicitWidth, buttonMinWidth)
            ColorSelector.pressed: true
            text: "recommand pressed"
        }
        RecommandButton {
            width: Math.max(implicitWidth, buttonMinWidth)
            text: "recommand disabled"
            enabled: false
        }

        // break row
        Item {width: parent.width; height: 1}

        Button {
            width: Math.max(implicitWidth, buttonMinWidth)
            text: "checked"
            checkable: true
            checked: true
        }
        Button {
            width: Math.max(implicitWidth, buttonMinWidth)
            ColorSelector.hovered: true
            ColorSelector.pressed: false
            ColorSelector.disabled: false
            text: "checked hover"
            enabled: false
            checked: true
        }
        Button {
            width: Math.max(implicitWidth, buttonMinWidth)
            ColorSelector.pressed: true
            text: "checked pressed"
            checked: true
        }
        Button {
            width: Math.max(implicitWidth, buttonMinWidth)
            text: "checked disabled"
            enabled: false
            checked: true
        }

        Item {width: parent.width; height: 1}

        Row {
            spacing: 10
            IconButton {
                icon.name: "go-previous"
            }
            IconButton {
                icon.name: "go-next"
            }
        }

        Row {
            spacing: 30
            IconButton {
                icon.name: "go-previous"
            }
            IconButton {
                icon.name: "go-next"
            }

            IconButton {
                icon.name: "go-up"
            }
            IconButton {
                icon.name: "go-down"
            }

            IconButton {
                icon.name: "add"
            }
            IconButton {
                icon.name: "remove"
            }

            IconButton {
                icon.name: "search"
            }
        }

        Row {
            spacing: 10
            Switch { checked: true }
            Switch { }
            Switch { checked: true; enabled: false }
            Switch { enabled: false }
        }
        Item {width: parent.width; height: 1}

        ComboBox {
            model: [ "Combobox Button", "Apple", "Coconut" ]
        }

        ComboBox {
            textRole: "text"
            iconNameRole: "icon"

            model: ListModel {
                ListElement { text: "Combobox Button"; icon: "go-previous" }
                ListElement { text: "Apple"; icon: "go-down" }
                ListElement { text: "Coconut"; icon: "go-next" }
            }
        }

        ComboBox {
            width: 80
            model: [ "读写", "Apple", "Coconut" ]
            flat: true
        }

        FloatingButton {
            width: 48
            height: 48
            icon.name: "list-add"
        }

        FloatingButton {
            checked: false
            icon {
                width: 12
                height: 12
                name: "combobox_arrow"
            }
        }

        Item {width: parent.width; height: 1}

        ActionButton {
            icon.name: "list-remove"
        }

        ActionButton {
            icon.name: "list-add"
        }

        ActionButton {
            icon.name: "view-close"
        }

        ActionButton {
            icon {
                name: "list-add"
                width: 48
                height: 48
            }
        }

        Row {
            spacing: 10
            Button {
                width: 50
                text: "复制"
                ColorSelector.family: Palette.CrystalColor
            }
            Button {
                width: 50
                text: "复制"
                ColorSelector.family: Palette.CrystalColor
                ColorSelector.hovered: true
                ColorSelector.pressed: false
                ColorSelector.disabled: false
            }
            Button {
                width: 50
                text: "复制"
                ColorSelector.family: Palette.CrystalColor
                ColorSelector.hovered: false
                ColorSelector.pressed: true
                ColorSelector.disabled: false
            }
            Button {
                width: 50
                text: "复制"
                ColorSelector.family: Palette.CrystalColor
                checked: true
            }

            Item {width: 80; height: parent.height}

            Button {
                width: 80
                text: "复制"
                ColorSelector.family: Palette.CrystalColor
                icon.name: "folder-copy"
            }
            Button {
                width: 80
                text: "复制"
                ColorSelector.family: Palette.CrystalColor
                icon.name: "folder-copy"
                display: IconLabel.IconBesideText
                ColorSelector.hovered: true
                ColorSelector.pressed: false
                ColorSelector.disabled: false
            }
            Button {
                width: 80
                text: "复制"
                ColorSelector.family: Palette.CrystalColor
                icon.name: "folder-copy"
                display: IconLabel.IconBesideText
                ColorSelector.hovered: false
                ColorSelector.pressed: true
                ColorSelector.disabled: false
            }
            Button {
                width: 80
                text: "复制"
                ColorSelector.family: Palette.CrystalColor
                icon.name: "folder-copy"
                checked: true
            }
        }

        Item {width: parent.width; height: 1}
        Item {width: 150; height: 1}

        Row {
            spacing: 10
            Button {
                width: 60
                spacing: 0
                text: "复制"
                ColorSelector.family: Palette.CrystalColor
                icon.name: "combobox_arrow"
                icon.width: 16
                icon.height: 16
                display: IconLabel.IconBesideText
            }
            Button {
                width: 60
                spacing: 0
                text: "复制"
                ColorSelector.family: Palette.CrystalColor
                icon.name: "combobox_arrow"
                icon.width: 16
                icon.height: 16
                display: IconLabel.IconBesideText
                ColorSelector.hovered: true
                ColorSelector.pressed: false
                ColorSelector.disabled: false
            }
            Button {
                width: 60
                spacing: 0
                text: "复制"
                ColorSelector.family: Palette.CrystalColor
                icon.name: "combobox_arrow"
                icon.width: 16
                icon.height: 16
                display: IconLabel.IconBesideText
                ColorSelector.hovered: false
                ColorSelector.pressed: true
                ColorSelector.disabled: false
            }

            Item {width: 30; height: parent.height}

            RecommandButton {
                width: 100
                text: "Recommend"
                ColorSelector.family: Palette.CrystalColor
            }
            RecommandButton {
                width: 100
                text: "Recommend"
                ColorSelector.family: Palette.CrystalColor
                ColorSelector.hovered: true
                ColorSelector.pressed: false
                ColorSelector.disabled: false
            }
            RecommandButton {
                width: 100
                text: "Recommend"
                ColorSelector.family: Palette.CrystalColor
                ColorSelector.hovered: false
                ColorSelector.pressed: true
                ColorSelector.disabled: false
            }
            RecommandButton {
                width: 100
                text: "Recommend"
                ColorSelector.family: Palette.CrystalColor
                checked: true
            }
        }

        Item {width: parent.width; height: 1}

        CheckBox {
        }
        CheckBox {
            ColorSelector.hovered: true
        }
        CheckBox {
            ColorSelector.pressed: true
        }
        CheckBox {
            enabled: false
        }

        Item {width: parent.width; height: 1}

        CheckBox {
            checked: true
        }
        CheckBox {
            checked: true
            ColorSelector.hovered: true
        }
        CheckBox {
            checked: true
            ColorSelector.pressed: true
        }
        CheckBox {
            checked: true
            enabled: false
        }

        Item {width: parent.width; height: 1}

        CheckBox {
            checkState: Qt.PartiallyChecked
        }
        CheckBox {
            checkState: Qt.PartiallyChecked
            ColorSelector.hovered: true
        }
        CheckBox {
            checkState: Qt.PartiallyChecked
            ColorSelector.pressed: true
        }
        CheckBox {
            checkState: Qt.PartiallyChecked
            enabled: false
        }
        CheckBox {
            text: "复选按钮选项"
        }

        Item {width: parent.width; height: 1}

        Row {
            ActionButton { icon.name: "go-down" }
            Column {
                ButtonGroup {
                    id: childGroup
                    exclusive: false
                    checkState: parentBox.checkState
                }
                CheckBox {
                    id: parentBox
                    text: "父级复选选项"
                    checkState: childGroup.checkState
                }
                Repeater {
                    model: 4
                    delegate: CheckBox {
                        checked: index < 2
                        text: "子级复选选项"
                        leftPadding: indicator.width
                        ButtonGroup.group: childGroup
                    }
                }
            }
        }

        Item {width: parent.width; height: 1}

        Row {
            spacing: 30
            ButtonBox {
                ButtonBoxButton { text: "日" ; checked: true}
                ButtonBoxButton { text: "周" }
                ButtonBoxButton { text: "月" }
                ButtonBoxButton { text: "年" }
            }

            ButtonBox {
                ButtonBoxButton { width: 50; text: "日"}
                ButtonBoxButton { width: 50; text: "周"; checked: true}
                ButtonBoxButton {
                    width: 50; text: "月"
                    ColorSelector.hovered: true
                }
                ButtonBoxButton { width: 50; text: "年"}
            }

            ButtonBox {
                ButtonBoxButton { width: 50; text: "设置"}
                ButtonBoxButton { width: 100; text: "更新设置"; checked: true}
            }
        }
    }
}
