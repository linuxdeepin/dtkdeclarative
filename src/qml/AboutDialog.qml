// SPDX-FileCopyrightText: 2021 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick 2.11
import QtQuick.Layouts 1.11
import QtQuick.Window 2.11
import org.deepin.dtk.impl 1.0 as D
import org.deepin.dtk.style 1.0 as DS

DialogWindow {
    id: control

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
        width: parent.width
        implicitHeight: contentLayout.implicitHeight
        topPadding: DS.Style.aboutDialog.topMargin
        bottomPadding: DS.Style.aboutDialog.bottomMargin
        palette.window: "transparent"
        ColumnLayout {
            id: contentLayout
            spacing: 0
            width: contentView.width

            D.IconLabel {
                id: logoLabel
                Layout.fillWidth: true
                Layout.preferredHeight: DS.Style.aboutDialog.productIconHeight
                Layout.alignment: Qt.AlignHCenter
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
                Layout.fillWidth: true
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
