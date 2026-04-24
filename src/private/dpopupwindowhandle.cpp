// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dpopupwindowhandle_p.h"
#include "dquickwindow.h"

#include <QQuickItem>
#include <QQuickWindow>
#include <QPointer>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>
#include <QMouseEvent>
#include <algorithm>

#include <private/qquickitem_p.h>

DQUICK_BEGIN_NAMESPACE

static bool isPopupWindow(QWindow *window)
{
    return window && window->inherits("QQuickPopupWindow");
}

static bool isPopupItem(QQuickItem *item)
{
    return item && item->inherits("QQuickPopupItem");
}

DPopupWindowHandle::~DPopupWindowHandle()
{
    if (m_trackedItem)
        QQuickItemPrivate::get(m_trackedItem)->removeItemChangeListener(this, QQuickItemPrivate::Geometry);
    if (m_parentWindow)
        m_parentWindow->removeEventFilter(this);
    if (m_popupWin)
        m_popupWin->removeEventFilter(this);
}

DPopupWindowHandle::DPopupWindowHandle(QObject *popup)
    : QObject(popup)
    , m_popup(popup)
{
    m_attached = DQuickWindow::qmlAttachedProperties(popup);
    if (!m_attached)
        return;
        
    // Initial update
    updateEnabled();
    
    popupItemReparented();

    connect(popup, SIGNAL(popupTypeChanged()), this, SLOT(updateEnabled()));
}

DQuickWindowAttached *DPopupWindowHandle::qmlAttachedProperties(QObject *object)
{
    auto handle = new DPopupWindowHandle(object);
    return handle->m_attached;
}

DQuickWindowAttached *DPopupWindowHandle::windowAttached() const
{
    return m_attached;
}

QQuickItem *DPopupWindowHandle::popupItem() const
{
    if (!m_popup)
        return nullptr;

    const auto children = m_popup->findChildren<QQuickItem *>(Qt::FindDirectChildrenOnly);
    auto it = std::find_if(children.begin(), children.end(), isPopupItem);
    if (it != children.end())
        return *it;
    return nullptr;
}

void DPopupWindowHandle::popupItemReparented()
{
    QQuickItem *item = popupItem();
    if (m_trackedItem == item)
        return;

    if (m_trackedItem) {
        QQuickItemPrivate::get(m_trackedItem)->removeItemChangeListener(this, QQuickItemPrivate::Geometry);
        disconnect(m_trackedItem, &QQuickItem::windowChanged, this, &DPopupWindowHandle::onWindowChanged);
    }

    m_trackedItem = item;

    QQuickItemPrivate::get(item)->addItemChangeListener(this, QQuickItemPrivate::Geometry | QQuickItemPrivate::Visibility);
    connect(item, &QQuickItem::windowChanged, this, &DPopupWindowHandle::onWindowChanged);


    if (QQuickWindow *window = popupWindow())
        m_attached->setWindow(window);
}

QQuickWindow *DPopupWindowHandle::popupWindow() const
{
    QQuickItem *item = popupItem();
    return item ? item->window() : nullptr;
}

void DPopupWindowHandle::updateEnabled()
{
    if (!m_popup || !m_attached)
        return;
    
    QVariant popupTypeVar = m_popup->property("popupType");
    bool shouldEnable = popupTypeVar.isValid() && popupTypeVar.toInt() == 1;
    if (shouldEnable == m_enabled)
        return;
    m_enabled = shouldEnable;
    m_attached->setEnabled(shouldEnable);
}

bool DPopupWindowHandle::isEnabled() const
{
    return m_enabled;
}

void DPopupWindowHandle::onWindowChanged(QQuickWindow *window)
{
    // Cleanup old filters
    if (m_popupWin) {
        m_popupWin->removeEventFilter(this);
        m_popupWin = nullptr;
    }
    if (m_parentWindow) {
        m_parentWindow->removeEventFilter(this);
        m_parentWindow = nullptr;
    }

    // Apply attached properties (blur, radius, etc.) to popup windows.
    if (m_attached) {
        if (!window || isPopupWindow(window))
            m_attached->setWindow(window);
    }
    
    m_popupWin = window;
    if (window && isEnabled() && isPopupWindow(window)) {

        window->installEventFilter(this);

        // Install event filter on parent window for close-on-click.
        // Done here so it is registered once per popup window instance.
        if (QQuickWindow *main = qobject_cast<QQuickWindow *>(window->transientParent())) {
            m_parentWindow = main;
            m_parentWindow->installEventFilter(this);
        }
    }
}

bool DPopupWindowHandle::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_popupWin && event->type() == QEvent::Move) {
        adjustPopupPosition();
    }

    // Close popup on parent window click (only while popup is visible)
    if (watched == m_parentWindow && event->type() == QEvent::MouseButtonPress
            && m_popup->property("visible").toBool()) {
        int closePolicy = m_popup->property("closePolicy").toInt();
        bool closeOnPressOutside = closePolicy & 0x01;
        bool closeOnPressOutsideParent = closePolicy & 0x02;

        if (closeOnPressOutside || closeOnPressOutsideParent) {
            QMetaObject::invokeMethod(m_popup, "close", Qt::QueuedConnection);
        }
    }

    return QObject::eventFilter(watched, event);
}

void DPopupWindowHandle::itemGeometryChanged(QQuickItem *,
                                              QQuickGeometryChange change,
                                              const QRectF &)
{
    if (!change.positionChange() && !change.sizeChange())
        return;

    adjustPopupPosition();
}

void DPopupWindowHandle::itemVisibilityChanged(QQuickItem *item)
{
    if (!item->isVisible())
        return;

    adjustPopupPosition();
}

void DPopupWindowHandle::itemParentChanged(QQuickItem *item, QQuickItem *)
{
    if (!item)
        return;

    adjustPopupPosition();
}

void DPopupWindowHandle::adjustPopupPosition()
{
    if (!isEnabled() ||!m_popupWin)
        return;

    const QSize size = m_popupWin->size();
    if (size.width() <= 0 || size.height() <= 0)
        return;

    // Nested popup (submenu) detection:
    // A submenu's popupWindow uses the parent menu's popupWindow as its transientParent.
    QWindow *transient = m_popupWin->transientParent();
    const bool isNested = isPopupWindow(transient);
    QRectF parentWindowRect;
    if (isNested)
        parentWindowRect = QRectF(QPointF(transient->position()), QSizeF(transient->size()));

    // Screen selection:
    // - Nested popups (submenus) must use the parent menu's screen to avoid
    //   screenAt() returning an adjacent screen or null when the submenu's
    //   initial position is already off-screen.
    // - Flat popups use their own position to determine the screen.
    QScreen *screen = nullptr;
    if (isNested) {
        screen = transient->screen();
    } else {
        const QPoint winCenter = m_popupWin->position() + QPoint(size.width() / 2, size.height() / 2);
        screen = QGuiApplication::screenAt(winCenter);
    }
    if (!screen)
        screen = m_popupWin->screen();
    if (!screen)
        return;

    const QRectF bounds(screen->availableGeometry());
    QRectF rect(QPointF(m_popupWin->position()), QSizeF(size));

    // Horizontal flip for submenus:
    // Qt places submenus to the right of the parent menu by default;
    // flip to the left if there is not enough space on the right, and vice versa.
    if (isNested && !parentWindowRect.isNull()) {
        const bool overflowsRight = rect.right() > bounds.right();
        const bool overflowsLeft  = rect.left()  < bounds.left();

        if (overflowsRight && !overflowsLeft) {
            // Not enough space on the right — try flipping to the left of the parent.
            const qreal leftCandidate = parentWindowRect.left() - size.width();
            if (leftCandidate >= bounds.left()) {
                rect.moveLeft(leftCandidate);
            } else {
                // Not enough space on either side — push against the right edge.
                rect.moveRight(bounds.right());
            }
        } else if (overflowsLeft && !overflowsRight) {
            // Not enough space on the left — flip to the right of the parent.
            const qreal rightCandidate = parentWindowRect.right();
            if (rightCandidate + size.width() <= bounds.right()) {
                rect.moveLeft(rightCandidate);
            } else {
                rect.moveLeft(bounds.left());
            }
        }
    }

    // Vertical clamping and final bounds enforcement.
    if (rect.right() > bounds.right())
        rect.moveRight(bounds.right());
    if (rect.left() < bounds.left())
        rect.moveLeft(bounds.left());
    if (rect.bottom() > bounds.bottom())
        rect.moveBottom(bounds.bottom());
    if (rect.top() < bounds.top())
        rect.moveTop(bounds.top());

    const QPoint newPos = rect.topLeft().toPoint();
    if (newPos != m_popupWin->position())
        m_popupWin->setPosition(newPos);
}

DQUICK_END_NAMESPACE

#include "moc_dpopupwindowhandle_p.cpp"

