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

AbstractArrowShapePath {
    id: control

    PathLine {
        x: control.arrowPosition.x + control.arrowWidth / 2
        y: control.arrowPosition.y + control.arrowHeight
    }

    PathLine {
        x: control.width - control.arrowRightRaidus
        y: control.arrowPosition.y + control.arrowHeight
    }

    PathArc {
        x: control.width
        y: control.arrowPosition.y + control.arrowHeight + control.arrowRightRaidus
        radiusX: control.arrowRightRaidus
        radiusY: control.arrowRightRaidus
    }

    PathLine {
        x: control.width
        y: control.height - control.roundJoinRadius
    }

    PathArc {
        x: control.width - control.roundJoinRadius
        y: control.height
        radiusX: control.roundJoinRadius
        radiusY: control.roundJoinRadius
    }

    PathLine {
        x: control.roundJoinRadius
        y: control.height
    }

    PathArc {
        x: 0
        y: control.height - control.roundJoinRadius
        radiusX: control.roundJoinRadius
        radiusY: control.roundJoinRadius
    }

    PathLine {
        x: 0
        y: control.arrowPosition.y + control.arrowHeight + control.arrowLeftRaidus
    }

    PathArc {
        x: control.arrowLeftRaidus
        y: control.arrowPosition.y + control.arrowHeight
        radiusX: control.arrowLeftRaidus
        radiusY: control.arrowLeftRaidus
    }

    PathLine {
        x: control.arrowPosition.x - control.arrowWidth / 2
        y: control.arrowPosition.y + control.arrowHeight
    }

    PathLine {
        x: control.arrowPosition.x
        y: control.arrowPosition.y
    }
}
