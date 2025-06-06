// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Templates as T
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

T.TextArea {
    id: control

    property D.Palette placeholderTextPalette: DS.Style.edit.placeholderText
    placeholderTextColor: D.ColorSelector.placeholderTextPalette
    implicitWidth: Math.max(contentWidth + leftPadding + rightPadding,
                            implicitBackgroundWidth + leftInset + rightInset,
                            placeholder.implicitWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(contentHeight + topPadding + bottomPadding,
                             implicitBackgroundHeight + topInset + bottomInset,
                             placeholder.implicitHeight + topPadding + bottomPadding)
                             
    padding: DS.Style.control.padding

    color: palette.text
    selectionColor: palette.highlight
    selectedTextColor: palette.highlightedText
    onEffectiveHorizontalAlignmentChanged: placeholder.effectiveHorizontalAlignmentChanged()

    Loader {
        id: placeholder
        active: !control.length && !control.preeditText && (!control.activeFocus || control.horizontalAlignment !== Qt.AlignHCenter)
        x: control.leftPadding
        y: control.topPadding
        width: control.width - (control.leftPadding + control.rightPadding)
        height: control.height - (control.topPadding + control.bottomPadding)
        signal effectiveHorizontalAlignmentChanged

        sourceComponent: PlaceholderText {
            text: control.placeholderText
            font: control.font
            color: control.placeholderTextColor
            verticalAlignment: control.verticalAlignment
            elide: Text.ElideRight
            renderType: control.renderType
        }
    }

    background: EditPanel {
        control: control
        implicitWidth: DS.Style.edit.width
        implicitHeight: DS.Style.edit.textAreaHeight
    }
}
