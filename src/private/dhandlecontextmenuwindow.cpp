// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dhandlecontextmenuwindow_p.h"
#include "private/qquickwindow_p.h"

DHandleContextMenuWindow::DHandleContextMenuWindow(QWindow *parent)
    : QQuickWindow (parent)
{

}

bool DHandleContextMenuWindow::event(QEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress) {
        handleMouseEvent(static_cast<QMouseEvent *>(e));
    }

    return QQuickWindow::event(e);
}

void DHandleContextMenuWindow::handleMouseEvent(QMouseEvent *event)
{
    QQuickWindowPrivate *d = reinterpret_cast<QQuickWindowPrivate *>(qGetPtrHelper(d_ptr));
    if (!mouseGrabberItem() && !d->pointerEventInstance(QQuickPointerDevice::genericMouseDevice())->point(0)->exclusiveGrabber()) {
        // 右键点击事件模拟发送菜单显示事件
        if (event->button() == Qt::RightButton && event->type() == QEvent::MouseButtonPress) {
            QPointF last = d->lastMousePosition.isNull() ? event->windowPos() : d->lastMousePosition;
            d->lastMousePosition = event->windowPos();

            QContextMenuEvent e(QContextMenuEvent::Mouse, last.toPoint(), event->windowPos().toPoint(), event->modifiers());
            QGuiApplication::sendEvent(this, &e);
        }
    }
}
