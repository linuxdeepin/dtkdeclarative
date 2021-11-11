/*
 * Copyright (C) 2020 Uniontech Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@uniontech.com>
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
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.4
import org.deepin.dtk 1.0 as D

Item {
    FastBlur {
        id: blurBack
        anchors.fill: back
        source: back
        radius: 50
    }
    ShaderEffectSource {
        id: blurEffectSource
        sourceItem: blurBack
        anchors.fill: blurBack
        hideSource: true
    }
    Image {
        id: back
        anchors.fill: parent
        source: "file:///usr/share/wallpapers/deepin/Hummingbird_by_Shu_Le.jpg"
    }

    ListView {
        id: testView

        property color foreground: "#55ffffff"
        model: 20
        spacing: 10
        clip: true
        anchors {
            fill: parent
            margins: 10
        }

        delegate: Item {
            property ListView view: ListView.view

            height: 50
            width: 300
            anchors.horizontalCenter: parent.horizontalCenter

            D.ItemViewport {
                radius: 18
                sourceItem: blurEffectSource // 可以为Image或ShaderEffectSource类型的组件
                sourceOffset: Qt.point(-parent.view.contentX + parent.x, -parent.view.contentY + parent.y)
                anchors.fill: parent
                foregroundColor: testView.foreground

                Text {
                    anchors.centerIn: parent
                    text: index
                    color: "white"
                    font.pixelSize: 22
                }
            }
        }
    }


    Button {
        text: "深浅切换"
        onClicked: {
            if (testView.foreground == "#55000000")
                testView.foreground = "#55ffffff"
            else
                testView.foreground = "#55000000"
        }
    }
}
