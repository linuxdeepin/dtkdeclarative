/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     JiDe Zhang <zhangjide@deepin.org>
 *
 * Maintainer: JiDe Zhang <zhangjide@deepin.org>
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

import QtQuick 2.0
import QtQuick.Layouts 1.0
import org.deepin.dtk 1.0

Item {
    property url url

    onUrlChanged: {
        edit.text = globalObject.readFile(url)
    }

    ColumnLayout {
        id: layout
        anchors.fill: parent
        spacing: 10

        Item {id: guest}

        Text {
            id: errorMessage

            Layout.fillWidth: true

            color: "#ff5736"
            font: DTK.fontManager.t6
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            horizontalAlignment: Qt.AlignCenter
        }

        Flickable {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea.flickable: TextArea {
                id: edit

                property Item lastPreview: guest

                width: parent.width
                selectByMouse: true
                selectByKeyboard: true

                onTextChanged: {
                    try {
                        var obj = Qt.createQmlObject(edit.text, layout)
                        globalObject.replace(lastPreview, obj)
                        lastPreview = obj
                        obj.Layout.fillWidth = true
                        errorMessage.text = ""
                    } catch (error) {
                        errorMessage.text = String(error.lineNumber ? error.lineNumber : "未知") + "行，"
                                + String(error.columnNumber ? error.columnNumber : "未知") + "列：" + error.message
                    }
                }
            }
        }
    }
}
