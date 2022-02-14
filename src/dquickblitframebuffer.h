/*
 * Copyright (C) 2021 UnionTech Technology Co., Ltd.
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DQUICKBLITFRAMEBUFFER_H
#define DQUICKBLITFRAMEBUFFER_H

#include <QQuickItem>
#include <dtkdeclarative_global.h>
#include <DObject>

DQUICK_BEGIN_NAMESPACE

class DQuickBlitFramebufferPrivate;
class Q_DECL_EXPORT DQuickBlitFramebuffer : public QQuickItem, public DCORE_NAMESPACE::DObject
{
    Q_OBJECT
    D_DECLARE_PRIVATE(DQuickBlitFramebuffer)
public:
    explicit DQuickBlitFramebuffer(QQuickItem *parent = nullptr);
    ~DQuickBlitFramebuffer();

    bool isTextureProvider() const override;
    QSGTextureProvider *textureProvider() const override;

private Q_SLOTS:
    void invalidateSceneGraph();

private:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
    void itemChange(ItemChange, const ItemChangeData &) override;
    void releaseResources() override;
};

DQUICK_END_NAMESPACE

#endif // DQUICKBLITFRAMEBUFFER_H
