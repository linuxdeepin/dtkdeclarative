// SPDX-FileCopyrightText: 2022 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#pragma once

#include <dtkdeclarative_global.h>

#include <QtQml>

#include "dqmlglobalobject_p.h"

#include <private/qquickitemchangelistener_p.h>

QT_BEGIN_NAMESPACE
class QQuickWindow;
class QQuickItem;
QT_END_NAMESPACE
DQUICK_BEGIN_NAMESPACE

class DQuickWindowAttached;

class DPopupWindowHandle : public QObject, public QQuickItemChangeListener
{
    Q_OBJECT
    
    QML_UNCREATABLE("PopupHandle Attached.")
    QML_NAMED_ELEMENT(PopupHandle)
    QML_ATTACHED(DQuickWindowAttached)
    
public:
    explicit DPopupWindowHandle(QObject *popup);
    ~DPopupWindowHandle() override;

    static DQuickWindowAttached *qmlAttachedProperties(QObject *object);

    DQuickWindowAttached *windowAttached() const;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

    // QQuickItemChangeListener
    void itemGeometryChanged(QQuickItem *item,
                             QQuickGeometryChange change,
                             const QRectF &oldGeometry) override;

    void itemVisibilityChanged(QQuickItem *item) override;

    void itemParentChanged(QQuickItem *item, QQuickItem *oldParent) override;

private Q_SLOTS:
    void updateEnabled();
    void onWindowChanged(QQuickWindow *window);

private:
    QQuickWindow *popupWindow() const;
    QQuickItem *popupItem() const;
    void popupItemReparented();

    bool isEnabled() const;
    void adjustPopupPosition();
    
private:
    QObject *m_popup = nullptr;

    bool m_enabled = false;
    DQuickWindowAttached *m_attached = nullptr;
    QPointer<QQuickWindow> m_parentWindow = nullptr;
    QPointer<QQuickWindow> m_popupWin = nullptr;
    QPointer<QQuickItem> m_trackedItem = nullptr;
};

DQUICK_END_NAMESPACE

QML_DECLARE_TYPEINFO(DTK_QUICK_NAMESPACE::DPopupWindowHandle, QML_HAS_ATTACHED_PROPERTIES)
