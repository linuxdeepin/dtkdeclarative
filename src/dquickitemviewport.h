// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DQUICKITEMVIEWPORT_H
#define DQUICKITEMVIEWPORT_H

#include <dtkdeclarative_global.h>
#include <DObject>

#include <QQuickItem>

DQUICK_BEGIN_NAMESPACE

class DQuickItemViewportPrivate;
class DQuickItemViewport : public QQuickItem, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickItem* sourceItem READ sourceItem WRITE setSourceItem NOTIFY sourceItemChanged)
    Q_PROPERTY(QRectF sourceRect READ sourceRect WRITE setSourceRect NOTIFY sourceRectChanged)
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(bool fixed READ fixed WRITE setFixed NOTIFY fixedChanged)
    Q_PROPERTY(bool hideSource READ hideSource WRITE setHideSource NOTIFY hideSourceChanged)
    D_DECLARE_PRIVATE(DQuickItemViewport)

public:
    explicit DQuickItemViewport(QQuickItem *parent = nullptr);
    ~DQuickItemViewport() override;

    QQuickItem* sourceItem() const;
    void setSourceItem(QQuickItem* sourceItem);

    QRectF sourceRect() const;
    void setSourceRect(const QRectF &sourceRect);

    float radius() const;
    void setRadius(float radius);

    bool fixed() const;
    void setFixed(bool newFixed);

    bool hideSource() const;
    void setHideSource(bool newHideSource);

    bool isTextureProvider() const override { return true; }
    QSGTextureProvider *textureProvider() const override;

Q_SIGNALS:
    void sourceItemChanged();
    void sourceRectChanged();
    void radiusChanged();
    void fixedChanged();
    void hideSourceChanged();

private Q_SLOTS:
    void invalidateSceneGraph();

protected:
    void itemChange(ItemChange, const ItemChangeData &) override;
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    QSGNode *updatePaintNode(QSGNode *old, UpdatePaintNodeData *) override;
    void componentComplete() override;
    void releaseResources() override;
};

DQUICK_END_NAMESPACE

#endif // DQUICKITEMVIEWPORT_H
