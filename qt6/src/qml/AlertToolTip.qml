// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

ToolTip {
    id: control
    property Item target

    x: 0
    topPadding: DS.Style.alertToolTip.verticalPadding
    bottomPadding: DS.Style.alertToolTip.verticalPadding
    leftPadding: DS.Style.alertToolTip.horizontalPadding
    rightPadding: DS.Style.alertToolTip.horizontalPadding
    implicitWidth: Math.min(DS.Style.control.implicitWidth(control), target.width)
    implicitHeight: DS.Style.control.implicitHeight(control)
    margins: 0
    closePolicy: Popup.NoAutoClose

    background: FloatingPanel {
        backgroundColor: DS.Style.alertToolTip.background
        insideBorderColor: DS.Style.alertToolTip.insideBorder
        outsideBorderColor: DS.Style.alertToolTip.outsideBorder
    }

    contentItem: Text {
        property D.Palette textColor: DS.Style.alertToolTip.text
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        text: control.text
        font: control.font
        color: D.ColorSelector.textColor
        wrapMode: Text.Wrap
    }

    enter: Transition {
        NumberAnimation { properties: "opacity"; from: 0.0; to: 1.0; duration: 200 }
        NumberAnimation { properties: "y"; from: control.target.height; to: control.target.height + DS.Style.control.spacing; duration: 200 }
    }

    exit: Transition {
        NumberAnimation { properties: "opacity"; from: 1.0; to: 0.0 }
        NumberAnimation { properties: "y"; from: control.target.height + DS.Style.control.spacing ; to: control.target.height }
    }
}
