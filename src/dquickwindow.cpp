/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
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

#include <DPlatformHandle>

#include "dquickwindow.h"
#include "private/dquickwindow_p.h"

DQUICK_BEGIN_NAMESPACE

DQuickWindowPrivate::DQuickWindowPrivate(DQuickWindow *qq)
    : DTK_CORE_NAMESPACE::DObjectPrivate(qq)
{
}

DQuickWindow::DQuickWindow(QWindow *parent)
    : QQuickWindow(parent)
    , DObject(*new DQuickWindowPrivate(this))
{
    d_func()->handle = new DPlatformHandle(this);
    auto handle = d_func()->handle;
    if (handle && DPlatformHandle::isEnabledDXcb(this)) {
        connect(handle, &DPlatformHandle::borderColorChanged, this, &DQuickWindow::borderColorChanged);
        connect(handle, &DPlatformHandle::borderWidthChanged, this, &DQuickWindow::borderWidthChanged);
        connect(handle, &DPlatformHandle::shadowColorChanged, this, &DQuickWindow::shadowColorChanged);
        connect(handle, &DPlatformHandle::shadowOffsetChanged, this, &DQuickWindow::shadowOffsetChanged);
        connect(handle, &DPlatformHandle::shadowRadiusChanged, this, &DQuickWindow::shadowRadiusChanged);
        connect(handle, &DPlatformHandle::windowRadiusChanged, this, &DQuickWindow::windowRadiusChanged);
        connect(handle, &DPlatformHandle::translucentBackgroundChanged, this, &DQuickWindow::translucentBackgroundChanged);
        connect(handle, &DPlatformHandle::enableSystemMoveChanged, this, &DQuickWindow::enableSystemMoveChanged);
        connect(handle, &DPlatformHandle::enableSystemResizeChanged, this, &DQuickWindow::enableSystemResizeChanged);
        connect(handle, &DPlatformHandle::enableBlurWindowChanged, this, &DQuickWindow::enableBlurWindowChanged);
    }
}

DQuickWindow::~DQuickWindow()
{
}

/*!
 * \property DQuickWindow::isValid
 * \brief 这个属性用于判定DPlatformHandle 是否有效
 */
bool DQuickWindow::isValid() const
{
    D_DC(DQuickWindow);

    if (!d->handle) {
        return false;
    }

    return DPlatformHandle::isEnabledDXcb(this);
}

/*!
 * \property DQuickWindow::windowRadius
 * \brief This property holds the radius of the main window.
 */
int DQuickWindow::windowRadius() const
{
    D_DC(DQuickWindow);

    if (!d->handle) {
        return 0;
    }

    return d->handle->windowRadius();
}

/*!
 * \property DQuickWindow::borderWidth
 * \brief This property holds the width of the main window's border.
 */
int DQuickWindow::borderWidth() const
{
    D_DC(DQuickWindow);

    if (!d->handle) {
        return 0;
    }

    return d->handle->borderWidth();
}

/*!
 * \property DQuickWindow::borderColor
 * \brief This property holds the color of the main window's border.
 */
QColor DQuickWindow::borderColor() const
{
    D_DC(DQuickWindow);

    if (!d->handle) {
        return QColor();
    }

    return d->handle->borderColor();
}

/*!
 * \property DQuickWindow::shadowRadius
 * \brief This property holds the shadow radius of the main widnow.
 */
int DQuickWindow::shadowRadius() const
{
    D_DC(DQuickWindow);

    if (!d->handle) {
        return 0;
    }

    return d->handle->shadowRadius();
}

/*!
 * \property DQuickWindow::shadowOffset
 * \brief This property holds the offset applied on the window shadow.
 */
QPoint DQuickWindow::shadowOffset() const
{
    D_DC(DQuickWindow);

    if (!d->handle) {
        return QPoint();
    }

    return d->handle->shadowOffset();
}

/*!
 * \property DQuickWindow::shadowColor
 * \brief This property holds the color of the window shadow.
 */
QColor DQuickWindow::shadowColor() const
{
    D_DC(DQuickWindow);

    if (!d->handle) {
        return QColor();
    }

    return d->handle->shadowColor();
}

/*!
 * \property DQuickWindow::frameMask
 * \brief This property holds the mask to be applied on the window.
 *
 * For better clip quality, for example antialiasing, use property
 * DQuickWindow::clipPath instead.
 */
QRegion DQuickWindow::frameMask() const
{
    D_DC(DQuickWindow);

    if (!d->handle) {
        return QRegion();
    }

    return d->handle->frameMask();
}

/*!
 * \property DQuickWindow::translucentBackground
 * \brief This property holds whether the window has translucent background.
 */
bool DQuickWindow::translucentBackground() const
{
    D_DC(DQuickWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->translucentBackground();
}

/*!
 * \brief DQuickWindow::enableSystemResize
 * \return This property holds whether the window can be resized by the user.
 *
 * The default value of this property is true.
 *
 * You can set this property to false and implement the resize polizy of this
 * window by you self.
 */
bool DQuickWindow::enableSystemResize() const
{
    D_DC(DQuickWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->enableSystemResize();
}

/*!
 * \property DQuickWindow::enableSystemMove
 * \brief This property holds whether the window can be moved by the user.
 *
 * The default value of this property is true.
 *
 * You can set this property to false and choose the effective area to drag and move.
 */
bool DQuickWindow::enableSystemMove() const
{
    D_DC(DQuickWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->enableSystemResize();
}

/*!
 * \property DQuickWindow::enableBlurWindow
 * \brief This property holds whether the window background is blurred.
 */
bool DQuickWindow::enableBlurWindow() const
{
    D_DC(DQuickWindow);

    if (!d->handle) {
        return false;
    }

    return d->handle->enableBlurWindow();
}

/*!
 * \~chinese \brief DQuickWindow::setWindowRadius　设定窗口的圆角
 * \~chinese \param windowRadius　窗口的圆角值
 */
void DQuickWindow::setWindowRadius(int windowRadius)
{
    D_D(DQuickWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setWindowRadius(windowRadius);
}

/*!
 * \~chinese \brief DQuickWindow::setBorderWidth 设定边框的宽度
 * \~chinese \param borderWidth　边框的宽度
 */
void DQuickWindow::setBorderWidth(int borderWidth)
{
    D_D(DQuickWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setBorderWidth(borderWidth);
}

/*!
 * \~chinese \brief DQuickWindow::setBorderColor 设定边框的颜色
 * \~chinese \param borderColor　边框的颜色
 */
void DQuickWindow::setBorderColor(const QColor &borderColor)
{
    D_D(DQuickWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setBorderColor(borderColor);
}

/*!
 * \~chinese \brief DQuickWindow::setShadowRadius 设定阴影区域的圆角
 * \~chinese \param shadowRadius　阴影区域圆角大小
 */
void DQuickWindow::setShadowRadius(int shadowRadius)
{
    D_D(DQuickWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setShadowRadius(shadowRadius);
}

/*!
 * \~chinese \brief DQuickWindow::setShadowOffset 设定阴影区域的偏移距离
 * \~chinese \param shadowOffset　阴影区域的偏移距离
 */
void DQuickWindow::setShadowOffset(const QPoint &shadowOffset)
{
    D_D(DQuickWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setShadowOffset(shadowOffset);
}

/*!
 * \~chinese \brief DQuickWindow::setShadowColor 设定阴影的颜色
 * \~chinese \param shadowColor　阴影的颜色
 */
void DQuickWindow::setShadowColor(const QColor &shadowColor)
{
    D_D(DQuickWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setShadowColor(shadowColor);
}

/*!
 * \~chinese \brief DQuickWindow::setTranslucentBackground 设定时候擦除背景
 * \~chinese \param translucentBackground true擦除背景　false不擦除背景
 */
void DQuickWindow::setTranslucentBackground(bool translucentBackground)
{
    D_D(DQuickWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setTranslucentBackground(translucentBackground);
}

/*!
 * \~chinese \brief DQuickWindow::setEnableSystemResize　设定是否允许系统调整窗口大小
 * \~chinese \param enableSystemResize　true允许系统调整　false不允许系统调整
 */
void DQuickWindow::setEnableSystemResize(bool enableSystemResize)
{
    D_D(DQuickWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setEnableSystemResize(enableSystemResize);
}

/*!
 * \~chinese \brief DQuickWindow::setEnableSystemMove 设定时候允许系统移动窗口
 * \~chinese \param enableSystemMove　true允许移动　false不允许移动
 */
void DQuickWindow::setEnableSystemMove(bool enableSystemMove)
{
    D_D(DQuickWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setEnableSystemMove(enableSystemMove);
}

/*!
 * \~chinese \brief DQuickWindow::setEnableBlurWindow　设定时候能伸缩窗口
 * \~chinese \param enableBlurWindow true能伸缩　false不能伸缩
 */
void DQuickWindow::setEnableBlurWindow(bool enableBlurWindow)
{
    D_D(DQuickWindow);

    if (!d->handle) {
        return;
    }

    d->handle->setEnableBlurWindow(enableBlurWindow);
}

DQUICK_END_NAMESPACE
