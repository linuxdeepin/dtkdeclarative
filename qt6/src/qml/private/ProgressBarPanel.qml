// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import org.deepin.dtk.style 1.0 as DS
import org.deepin.dtk 1.0

Item {
    id: control
    property Item progressBar

    Loader {
        anchors.fill: parent
        sourceComponent: progressBar.formatText ? _textPanelComponent : _normalTextComponent
    }

    Component {
        id: _textPanelComponent
        BoxPanel {}
    }

    Component {
        id: _normalTextComponent
        Rectangle {
            property Palette backgroundColor: DS.Style.progressBar.background
            radius: DS.Style.control.radius
            color: ColorSelector.backgroundColor
        }
    }
}
