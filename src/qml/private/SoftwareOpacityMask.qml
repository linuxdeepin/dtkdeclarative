/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     xiaoyaobing <xiaoyaobing@uniontech.com>
 *
 * Maintainer: xiaoyaobing <xiaoyaobing@uniontech.com>
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
import QtGraphicalEffects.private 1.12
import org.deepin.dtk.impl 1.0 as D

/*!
    The Qt Graphical Effects module provides a SourceProxy types.
    the user can simply use ShaderEffectSource or Item as input

*/
Item {
    id: rootItem

    property variant source
    property variant maskSource
    property bool cached: false
    property alias invert: mask.invert

    SourceProxy {
        id: sourceProxy
        input: rootItem.source
    }

    SourceProxy {
        id: maskSourceProxy
        input: rootItem.maskSource
    }

    ShaderEffectSource {
        id: cacheItem
        anchors.fill: parent
        visible: rootItem.cached
        smooth: true
        sourceItem: shaderItem
        live: true
        hideSource: visible
    }

    D.SoftwareOpacityMask {
        id: mask
        anchors.fill: parent
        source: sourceProxy.output
        maskSource: maskSourceProxy.output
    }
}
