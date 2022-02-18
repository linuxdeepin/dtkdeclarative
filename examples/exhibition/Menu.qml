/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
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

import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQml 2.11
import org.deepin.dtk 1.0

Column {
    id: panel
    Label {
        width: parent.width
        wrapMode: Text.WordWrap
        text: "菜单控件，主要出现在右键，DCombobox弹出，主菜单，搜索框的补全等一些地方。带尖角的菜单有明确的指向，告诉用户这个菜单对应的是哪个地方的。"
        horizontalAlignment: Qt.AlignHCenter
    }

    spacing: 10

    Flow {
        spacing: 10
        width: parent.width

        Button {
            text: "open normalMenu"
            onClicked: normalMenu.open()
        }
        Menu {
            id: normalMenu
            MenuItem { text: qsTr("打开") }
            MenuItem { text: qsTr("在新窗口打开") }
            MenuItem { text: qsTr("在新标签中打开") }
            MenuItem { text: qsTr("以管理员身份打开") }
            MenuSeparator {}
            MenuItem { text: qsTr("复制") }
            MenuSeparator {}
            MenuItem { text: qsTr("共享文件夹")}
            MenuItem { text: qsTr("创建链接")}
            MenuItem { text: qsTr("发送到桌面")}
            MenuItem { text: qsTr("在终端中打开")}
            Menu { title: qsTr("自定义")}
            MenuItem { text: qsTr("压缩")}
            MenuItem { text: qsTr("属性")}
        }

        Button {
            text: "open separatorMenu"
            onClicked: separatorMenu.open()
        }
        Menu {
            id: separatorMenu
            MenuSeparator { text: qsTr("单曲")}
            MenuItem { icon.name: "folder-music-symbolic"; text: qsTr("Tomorrow-Fly By Midnight") }
            MenuItem { icon.name: "folder-music-symbolic"; text: qsTr("Tomorrow With You (EN]ABA)- Senpai Kondor") }
            MenuItem { icon.name: "folder-music-symbolic"; text: qsTr("Tomorrow will be fine. - Sodagreen") }
            MenuItem { icon.name: "folder-music-symbolic"; text: qsTr("CROWN - TOMORROW X TOGETHER") }
            MenuSeparator { text: qsTr("歌手")}
            MenuItem { icon.name: "music"; text: qsTr("Tomorrow - Fly By Midnight") }
            MenuSeparator { text: qsTr("专辑")}
            MenuItem { icon.name: "music"; text: qsTr("DTK-Menu-Menuitem-ICON-Right-Light") }
            MenuItem { icon.name: "music"; text: qsTr("CROWN - TOMORROW X TOGETHER") }
        }

        Button {
            text: "open searchAndArrowMenu"
            onClicked: searchAndArrowMenu.popup()
        }
        Menu {
            id: searchAndArrowMenu
            header: SearchEdit {
                placeholder: qsTr("搜索")
            }
            maxVisibleItems: 10
            MenuItem { text: qsTr("Greek(cp869)") }
            MenuItem { text: qsTr("Cyrillic (ISO 8859-5)") }
            MenuItem { text: qsTr("Cyrillic(KOI8-R)") }
            MenuItem { text: qsTr("Devanagari(x-mac-davanagari)") }
            MenuItem { text: qsTr("Gurmukhi(x-mac-gurmukhi)") }
            MenuItem { text: qsTr("Thai (ISO 8859-11)") }
            MenuItem { text: qsTr("Simplified Chinese(x-mac-sim-chinese)") }
            MenuItem { text: qsTr("Chinese(GBK)"); checked: true}
            MenuItem { text: qsTr("Chinese(GB 18030)") }
            MenuItem { text: qsTr("Chinese(ISO 2022-CN)") }
            MenuItem { text: qsTr("Simplified Chinese (GB2312)") }
            MenuItem { text: qsTr("Tibetan (x-mac-tibetan)") }
            MenuItem { text: qsTr("Central European(ISO Latin 2)") }
            MenuItem { text: qsTr("Central Eu 188 opx(ISO Latin 4)") }
            MenuItem { text: qsTr("Baltic (ISO Latin 7)") }
        }

        Button {
            text: "open fontMenu"
            onClicked: fontMenu.popup()
        }
        Menu {
            id: fontMenu
            Instantiator {
                id: menuCreateInstantiator
                asynchronous: true
                model: Qt.fontFamilies()
                delegate: MenuItem { }
                onObjectAdded: {
                    object.text = model[index]
                    object.font.family = object.text
                    fontMenu.insertItem(index, object)
                }
            }
        }

        Button {
            text: "open iconMenu"
            onClicked: iconMenu.popup()
        }
        Menu {
            id: iconMenu
            MenuItem { icon.name: "music"; text: qsTr("撤销") }
            MenuItem { icon.name: "music"; text: qsTr("恢复") }
            MenuSeparator {}
            MenuItem { icon.name: "music"; text: qsTr("剪切") }
            MenuItem { icon.name: "music"; text: qsTr("复制") }
            MenuItem { icon.name: "music"; text: qsTr("粘贴") }
            MenuItem { icon.name: "music"; text: qsTr("删除") }
            MenuSeparator {}
            MenuItem { icon.name: "music"; text: qsTr("全选") }
        }

        Button {
            text: "open iconBesideTextMenu"
            onClicked: iconBesideTextMenu.popup()
        }
        Menu {
            id: iconBesideTextMenu
            MenuItem { display: IconLabel.IconBesideText; icon.name: "music"; text: qsTr("所有项") }
            MenuItem { display: IconLabel.IconBesideText; icon.name: "music"; text: qsTr("照片") }
            MenuItem { display: IconLabel.IconBesideText; icon.name: "music"; text: qsTr("视频") }
        }

        Button {
            text: "open subMenuAndCheckMenu"
            onClicked: subMenuAndCheckMenu.popup()
        }
        Menu {
            id: subMenuAndCheckMenu
            header: SearchEdit {
                placeholder: qsTr("搜索")
            }

            Menu { title: "新建" }
            Menu {
                title: qsTr("排序方式")
                MenuItem { autoExclusive: true; checkable: true; text: qsTr("名称"); checked: true }
                MenuItem { autoExclusive: true; checkable: true; text: qsTr("修改时间") }
                MenuItem { autoExclusive: true; checkable: true; text: qsTr("创建时间") }
                MenuItem { autoExclusive: true; checkable: true; text: qsTr("大小") }
                MenuItem { autoExclusive: true; checkable: true; text: qsTr("文件类型") }
            }
            Menu { title: "桌面显示" }
            MenuItem { text: "全选" }
            MenuItem { text: "在终端打开" }
            MenuSeparator {}
            Menu { title: "显示设置" }
            Menu { title: "壁纸与屏保" }
        }

        Button {
            text: "open iconAndCheckMenu"
            onClicked: iconAndCheckMenu.popup()
        }
        Menu {
            id: iconAndCheckMenu
            MenuItem { checkable: true; icon.name: "music"; text: qsTr("所有项") }
            MenuItem { checkable: true; icon.name: "music"; text: qsTr("照片"); checked: true }
            MenuItem { checkable: true; icon.name: "music"; text: qsTr("视频") }
        }
    }
}
