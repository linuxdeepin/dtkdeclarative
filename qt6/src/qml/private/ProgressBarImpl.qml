// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

import QtQuick
import QtQuick.Controls.impl
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS
import org.deepin.dtk 1.0

Item {
    id: control
    property Item progressBar
    property string formatText
    property bool animationStop: false
    property D.Palette shadowPaletteColor: DS.Style.progressBar.shadowColor
    property D.Palette handleGradientColor: DS.Style.progressBar.handleGradientColor

    Loader {
        anchors.fill: parent
        sourceComponent: progressBar.indeterminate ? _indeterminateComponent
                                                   : _valueComponent
    }

    Component {
        id: _valueComponent

        Item {
            BoxShadow {
                anchors.fill: parent
                anchors.rightMargin: (1 - progressBar.visualPosition) * progressBar.width
                shadowOffsetY: 4
                shadowBlur: 6
                cornerRadius: DS.Style.control.radius
                shadowColor: control.D.ColorSelector.shadowPaletteColor
                visible: progressBar.visualPosition > 0
            }

            Item {
                id: item
                // 向左偏移height显示，避免宽度过窄时产生的圆角显示问题
                x: - height
                width: progressBar.width + height
                height: progressBar.height
                Rectangle {
                    id: rect
                    property color gradientColor: progressBar.palette.highlight
                    property real lightPosition: 0
                    width: progressBar.width * progressBar.visualPosition + height
                    height: progressBar.height
                    gradient: Gradient { 
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0; color: progressBar.palette.highlight }
                        GradientStop { position: rect.lightPosition; color: rect.gradientColor }
                        GradientStop { position: 1; color: progressBar.palette.highlight }
                    }

                    LightSweepAnimation {
                        targetItem: rect
                        running: !control.animationStop && rect.visible
                    }

                    radius: DS.Style.control.radius
                    layer.enabled: true
                    clip: true
                    layer.effect: OpacityMask {
                        maskSource: Rectangle {
                                width: rect.width
                                height: rect.height
                                radius: DS.Style.control.radius
                            }
                    }
                }
                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Item {
                        width: item.width
                        height: item.height
                        Rectangle {
                            anchors.fill: parent
                            anchors.leftMargin: -item.x
                            radius: DS.Style.control.radius
                        }
                    }
                }
            }

            ItemGroup {
                anchors.fill: parent
                ClippedText {
                    clip: progressBar.visualPosition > 0
                    clipX: -progressBar.leftPadding + progressBar.visualPosition * control.width
                    clipWidth: (1.0 - progressBar.visualPosition) * control.width
                    text: formatText
                    font: progressBar.font
                    color: progressBar.palette.buttonText
                    opacity: enabled ? 1 : 0.4
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                ClippedText {
                    visible: progressBar.visualPosition > 0
                    clip: progressBar.visualPosition > 0
                    clipX: -progressBar.leftPadding
                    clipWidth: progressBar.visualPosition * control.width
                    text: formatText
                    font: progressBar.font
                    color: palette.highlightedText
                    opacity: enabled ? 1 : 0.4
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
            }
        }
    }

    Component {
        id: _indeterminateComponent
        Item {
            BoxShadow {
                id: indeterminateProgressContent
                property real limitWidth: (indeterminateProgressContent.height - indeterminateProgressContent.width) / 2
                y: -limitWidth
                x: limitWidth
                implicitHeight: DS.Style.progressBar.indeterminateProgressBarWidth
                width: progressBar.height
                shadowOffsetX: -4
                shadowBlur: 6
                cornerRadius: DS.Style.control.radius
                shadowColor: control.D.ColorSelector.shadowPaletteColor
                rotation: -90

                Rectangle {
                    id: indeterminateRect
                    anchors.fill: parent
                    radius: indeterminateProgressContent.cornerRadius
                    property real lightPosition: 0
                    property color gradientColor: control.D.ColorSelector.handleGradientColor
                    gradient: Gradient {
                        GradientStop { position: 0; color: progressBar.palette.highlight }
                        GradientStop { position: indeterminateRect.lightPosition; color: indeterminateRect.gradientColor }
                        GradientStop { position: 1; color: progressBar.palette.highlight }
                    }
                    LightSweepAnimation {
                        targetItem: indeterminateRect
                        running: progressBar.indeterminate && !control.animationStop && indeterminateRect.visible
                    }
                }

                SequentialAnimation {
                    id: indeterminateAnimation
                    running: progressBar.indeterminate && !control.animationStop
                    loops: Animation.Infinite
                    NumberAnimation {
                        target: indeterminateProgressContent
                        property: "x"
                        from: -indeterminateProgressContent.y
                        to: progressBar.width - indeterminateProgressContent.limitWidth - indeterminateProgressContent.width
                        duration: DS.Style.progressBar.indeterminateProgressBarAnimationDuration * progressBar.width / DS.Style.progressBar.width
                        onFromChanged: if (!delayAnimationTimer.running) delayAnimationTimer.start()
                        onToChanged:  if (!delayAnimationTimer.running) delayAnimationTimer.start()
                    }

                    NumberAnimation {
                        target: indeterminateProgressContent
                        property: "x"
                        to: -indeterminateProgressContent.y
                        duration: DS.Style.progressBar.indeterminateProgressBarAnimationDuration * progressBar.width / DS.Style.progressBar.width
                        onFromChanged: if (!delayAnimationTimer.running) delayAnimationTimer.start()
                        onToChanged: if (!delayAnimationTimer.running) delayAnimationTimer.start()
                    }
                }
            }

            Timer {
                // ###(Chen Bin) Layout bug: Put the progressbar into the layout,
                // the animation will get a wrong position
                id: delayAnimationTimer
                interval: 200
                repeat: false
                onTriggered: {
                    indeterminateAnimation.restart()
                }
            }

            ItemGroup {
                anchors.fill: parent
                ClippedText {
                    clip: true
                    clipX: indeterminateProgressContent.x - indeterminateProgressContent.limitWidth - progressBar.leftPadding
                    clipWidth: indeterminateProgressContent.height
                    text: formatText
                    font: progressBar.font
                    color: palette.highlightedText
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                ClippedText {
                    clip: true
                    visible: indeterminateProgressContent.x - indeterminateProgressContent.limitWidth + progressBar.leftPadding > 0
                    clipX: -progressBar.leftPadding
                    clipWidth: indeterminateProgressContent.x - indeterminateProgressContent.limitWidth + progressBar.leftPadding
                    text: formatText
                    font: progressBar.font
                    color: progressBar.palette.buttonText
                    opacity: enabled ? 1 : 0.4
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                ClippedText {
                    clip: true
                    clipX: indeterminateProgressContent.x - indeterminateProgressContent.limitWidth - progressBar.leftPadding + indeterminateProgressContent.height
                    clipWidth: progressBar.width - progressBar.rightPadding - indeterminateProgressContent.x - indeterminateProgressContent.limitWidth
                               - progressBar.leftPadding + indeterminateProgressContent.height
                    text: formatText
                    font: progressBar.font
                    color: progressBar.palette.buttonText
                    opacity: enabled ? 1 : 0.4
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
            }
        }
    }

    component LightSweepAnimation: SequentialAnimation{
        id: anim
        property Item targetItem: null
        loops: Animation.Infinite

        ParallelAnimation {
            SequentialAnimation {
                ColorAnimation {
                    target: anim.targetItem
                    property: "gradientColor"
                    from: progressBar.palette.highlight
                    to: control.D.ColorSelector.handleGradientColor
                    duration: 500
                }
                PauseAnimation { duration: 2000 }
                ColorAnimation {
                    target: anim.targetItem
                    property: "gradientColor"
                    from: control.D.ColorSelector.handleGradientColor
                    to: progressBar.palette.highlight
                    duration: 500
                }
            }

            NumberAnimation {
                target: anim.targetItem
                property: "lightPosition"
                from: 0
                to: 1
                duration: 3000
                easing.type: Easing.InOutSine
            }
        }

        PauseAnimation { duration: 2000 }
    }
}
