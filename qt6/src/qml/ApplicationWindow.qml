// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Templates as T
import org.deepin.dtk 1.0 as D

T.ApplicationWindow {
    palette: active ? D.DTK.palette : D.DTK.inactivePalette
    font: D.DTK.fontManager.t6
    color: palette.window
}
