/*
 * Copyright (C) 2020 ~ 2022 Uniontech Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls.impl 2.4
import QtQuick.Layouts 1.11
import QtQuick.Templates 2.4 as T
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

T.ComboBox {
    id: control

    property string iconNameRole

    implicitWidth: DS.Style.control.implicitWidth(control)
    implicitHeight: DS.Style.control.implicitHeight(control)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    padding: DS.Style.comboBox.padding
    leftPadding: padding + (!control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)
    rightPadding: padding + (control.mirrored || !indicator || !indicator.visible ? 0 : indicator.width + spacing)

    delegate: MenuItem {
        useIndicatorPadding: true
        width: parent.width
        text: control.textRole ? (Array.isArray(control.model) ? modelData[control.textRole] : model[control.textRole]) : modelData
        icon.name: (control.iconNameRole && model[control.iconNameRole] !== undefined) ? model[control.iconNameRole] : null
        highlighted: control.highlightedIndex === index
        hoverEnabled: control.hoverEnabled
        checked: control.currentIndex === index
    }

    indicator: Loader {
        sourceComponent: control.editable ? editableIndicator : normalIndicator
        x: control.mirrored ? control.padding : control.width - width - control.padding
        y: control.topPadding + (control.availableHeight - height) / 2
        Component {
            id: editableIndicator
            RowLayout {
                spacing: DS.Style.comboBox.editableIndicatorSpacing
                Rectangle {
                    width: 1
                    height: control.height
                    color: DS.Style.comboBox.editableLineColor
                }

                D.DciIcon {
                    sourceSize {
                        width: DS.Style.comboBox.editableIndicatorSize
                        height: DS.Style.comboBox.editableIndicatorSize
                    }
                    palette: D.DTK.makeIconPalette(control.palette)
                    name: "entry_option_arrow_down"
                    mode: control.D.ColorSelector.controlState
                }
            }
        }

        Component {
            id: normalIndicator
            D.DciIcon {
                sourceSize {
                    width: DS.Style.comboBox.iconSize
                    height: DS.Style.comboBox.iconSize
                }
                palette: D.DTK.makeIconPalette(control.palette)
                name: "arrow_ordinary_down"
                mode: control.D.ColorSelector.controlState
            }
        }
    }

    contentItem: RowLayout {
        spacing: DS.Style.comboBox.spacing
        D.DciIcon {
            palette: D.DTK.makeIconPalette(control.palette)
            mode: control.D.ColorSelector.controlState
            name: (control.iconNameRole && model.get(control.currentIndex)[control.iconNameRole] !== undefined)
                  ? model.get(control.currentIndex)[control.iconNameRole] : null
            visible: name
            sourceSize.width: DS.Style.comboBox.iconSize
            sourceSize.height: DS.Style.comboBox.iconSize
        }

        T.TextField {
            Layout.fillWidth: true
            Layout.fillHeight: true
            text: control.editable ? control.editText : control.displayText

            enabled: control.editable
            autoScroll: control.editable
            readOnly: control.down
            inputMethodHints: control.inputMethodHints
            validator: control.validator
            selectByMouse: true

            font: control.font
            color: control.editable ? control.palette.text : control.palette.buttonText
            selectionColor: control.palette.highlight
            selectedTextColor: control.palette.highlightedText
            verticalAlignment: Text.AlignVCenter
        }
    }

    background: Item {
        implicitWidth: DS.Style.comboBox.width
        implicitHeight: DS.Style.comboBox.height
        Loader {
            anchors.fill: parent
            sourceComponent: control.editable ? editableComponent : floatingComponent
            property alias comboBox: control
            Component {
                id: editableComponent
                EditPanel {
                    control: comboBox
                }
            }

            Component {
                id: floatingComponent
                ButtonPanel {
                    button: comboBox
                }
            }
        }
    }

    popup: Popup {
        implicitWidth: control.width
        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.delegateModel
            currentIndex: control.highlightedIndex
            highlightRangeMode: ListView.ApplyRange
            highlightMoveDuration: 0

            T.ScrollBar.vertical: ScrollBar { }
        }

        background: FloatingPanel {
            implicitWidth: DS.Style.menu.itemWidth
            implicitHeight: DS.Style.menu.itemHeight
            radius: DS.Style.menu.radius
            backgroundColor: D.DTK.selectColor(palette.window, DS.Style.menu.lightBackground, DS.Style.menu.lightBackground)
        }
    }
}
