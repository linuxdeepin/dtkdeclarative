/*
 * Copyright (C) 2021 ~ 2022 UnionTech Technology Co., Ltd.
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.11
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11
import org.deepin.dtk.impl 1.0 as D

DialogWindow {
    id: control
    width: 380

    property alias windowTitle: control.title
    property alias productName: productNameLabel.text
    property alias productIcon: logoLabel.icon.name
    property alias version: versionLabel.text
    property alias description: descriptionLabel.text
    property alias license: licenseLabel.text
    property alias companyLogo: companyLogoLabel.source
    property string websiteName
    property string websiteLink

    readonly property string __websiteLinkTemplate:
        "<a href='%1' style='text-decoration: none; font-size:13px; color: #004EE5;'>%2</a>"

    ScrollView {
        id: contentView
        width: control.width
        anchors {
            leftMargin: 11
            topMargin: 20
            rightMargin: 11
            bottomMargin: 10
        }

        implicitHeight: Math.min(control.maxContentHeight, contentLayout.childrenRect.height)
        palette.window: "transparent"

        ColumnLayout {
            id: contentLayout
            spacing: 0
            width: contentView.width

            D.IconLabel {
                id: logoLabel
                Layout.alignment: Qt.AlignCenter
                Layout.topMargin: 0
                display: D.IconLabel.IconOnly
            }
            Label {
                id: productNameLabel
                Layout.alignment: Qt.AlignCenter
                Layout.topMargin: 3
            }
            Label {
                id: versionLabel
                Layout.alignment: Qt.AlignCenter
                Layout.topMargin: 8
            }
            Image {
                id: companyLogoLabel
                Layout.alignment: Qt.AlignCenter
                Layout.topMargin: 0
            }
            Label {
                id: websiteLabel
                Layout.alignment: Qt.AlignCenter
                Layout.topMargin: 5
                text: (websiteLink === "" || websiteName === "") ?
                          "" : __websiteLinkTemplate.arg(websiteLink).arg(websiteName)
            }
            Label {
                id: descriptionLabel
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 12
                wrapMode: Text.Wrap
            }
            Label {
                id: licenseLabel
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 7
                visible: license !== ""
            }

            Component.onCompleted: {
                websiteLabel.linkActivated.connect(D.ApplicationHelper.openUrl)
                descriptionLabel.linkActivated.connect(D.ApplicationHelper.openUrl)
                licenseLabel.linkActivated.connect(D.ApplicationHelper.openUrl)
                console.log("logo path", D.DTK.deepinDistributionOrgLogo)
            }
        }

        focus: true
        Keys.onEscapePressed: {
            control.close()
            event.accepted = true
        }
    }
}
