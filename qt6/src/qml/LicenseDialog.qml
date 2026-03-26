// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

DialogWindow {
    id: control
    width: 800
    height: 900
    title: qsTr("open-source software")

    property alias licensePath: licenseProvider.path

    header: D.DialogTitleBar {
        title: control.title
        leftContent: Item {
            width: 32
            D.IconButton {
                anchors.centerIn: parent
                visible: stackView.depth > 1
                icon.name: "arrow_ordinary_left"
                implicitWidth: 32
                implicitHeight: 32
                icon.width: 12
                icon.height: 12
                onClicked: stackView.pop()
            }
        }  
    }

    Item {
        implicitWidth: control.width - control.leftPadding - control.rightPadding
        implicitHeight: control.height - DS.Style.dialogWindow.titleBarHeight - DS.Style.dialogWindow.contentHMargin

        D.LicenseInfoProvider {
            id: licenseProvider
        }

        StackView {
            id: stackView
            anchors.fill: parent
            initialItem: listPage
            clip: true
        }

        Component {
            id: listPage
            Item {
                ListView {
                    id: licenseList
                    anchors.fill: parent
                    clip: true
                    model: licenseProvider.licenseList
                    ScrollBar.vertical: ScrollBar {}

                    delegate: D.ItemDelegate {
                        id: delegateItem
                        implicitWidth: licenseList.width
                        implicitHeight: 50
                        text: modelData.name
                        palette.windowText: undefined
                        property D.Palette colorOdd: D.Palette {
                            normal: Qt.rgba(0, 0, 0, 0.05)
                            hovered: Qt.rgba(0, 0, 0, 0.1)
                        }
                        property D.Palette colorEven: D.Palette {
                            normal: Qt.rgba(0, 0, 0, 0.02)
                            hovered: Qt.rgba(0, 0, 0, 0.1)
                        }

                        onClicked: {
                            stackView.push(detailPage, {
                                packageName: modelData.name,
                                licenseText: licenseProvider.licenseContent(modelData.licenseName)
                            })
                        }
                        background: D.BoxPanel {
                            color1: index % 2 === 0 ? delegateItem.colorOdd : delegateItem.colorEven
                            insideBorderColor: null
                            outsideBorderColor: null
                            dropShadowColor: null
                            innerShadowColor1: null
                            innerShadowColor2: null
                        }
                    }
                }
            }
        }

        Component {
            id: detailPage
            Item {
                property string packageName: ""
                property string licenseText: ""

                Flickable {
                    anchors.fill: parent
                    contentWidth: width
                    contentHeight: detailColumn.implicitHeight
                    clip: true
                    topMargin: 20
                    ScrollBar.vertical: ScrollBar {}

                    ColumnLayout {
                        id: detailColumn
                        width: parent.width
                        spacing: 20

                        Text {
                            Layout.fillWidth: true
                            text: packageName
                            font.pixelSize: D.DTK.fontManager.t5.pixelSize
                            font.family: D.DTK.fontManager.t5.family
                            font.bold: true
                            color: D.DTK.palette.windowText
                        }

                        MenuSeparator{}

                        Text {
                            Layout.fillWidth: true
                            text: licenseText
                            font: D.DTK.fontManager.t6
                            color: D.DTK.palette.windowText
                            wrapMode: Text.WordWrap
                            verticalAlignment: Text.AlignTop
                        }
                    }
                }
            }
        }
    }
}
