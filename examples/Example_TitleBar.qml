/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
 *
 * Author:     yeshanshan <yeshanshan@uniontech.com>
 *
 * Maintainer: yeshanshan <yeshanshan@uniontech.com>
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

import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Window 2.11
import QtQuick.Layouts 1.11
import org.deepin.dtk 1.0 as D

D.TitleBar {
    id: titleBar
    iconName: "music"
    title: "title custom"

    property string appProductName: Qt.application.displayName
    property string appLicense

//    menu: Menu {
//        delegate: MenuItem {
//            icon.name: "emblem-checked"
//            icon.color: palette.highlight
//        }
//        D.ThemeMenu {}

//        MenuSeparator {
//            contentItem: Rectangle {
//                implicitHeight: 1
//                color: "black"
//            }
//        }
//        D.HelpAction {}
//        D.AboutAction {}
//        D.QuitAction {}

//        Action {
//            text: qsTr("custom action")
//        }
//        MenuItem {
//            text: qsTr("Window State")
//            onTriggered: toggleWindowState()
//        }
//    }
    aboutDialog: D.AboutDialog {
        modality: Qt.NonModal
        version: qsTr(String("Version: %1").arg(Qt.application.version))
        productName: qsTr(appProductName)
        companyLogo: "file://" + D.DTK.deepinDistributionOrgLogo
        websiteName: D.DTK.deepinWebsiteName
        websiteLink: D.DTK.deepinWebsiteLink
        license: appLicense === "" ? "" : qsTr(String("%1 is released under %2").arg(appProductName).arg(appLicense))
    }

    embedMode: false
//    separatorVisible: false
    autoHideOnFullscreen: true
//    menuDisabled: true

//    content: RowLayout {
//        D.LineEdit {
//            text: "label2"
//        }
//        D.LineEdit {
//            text: "center"
//            Layout.alignment: Qt.AlignCenter
//        }
//        Button {
//            text: "center custom"
//            Layout.alignment: Qt.AlignRight
//        }
//    }

    focus: true
    Keys.onSpacePressed: Window.window.visibility = Window.FullScreen
}
