/*
 * Copyright (C) 2020 Uniontech Technology Co., Ltd.
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#define protected public
#include <QSGMaterialShader>
#undef protected

#include "dquickitemviewport.h"

#include <DVtableHook>
#include <DObjectPrivate>

#include <QSGMaterial>
#include <QSGImageNode>
#include <QOpenGLFunctions>
#include <QQuickImageProvider>

#include <private/qsgadaptationlayer_p.h>
#include <private/qquickitem_p.h>
#include <private/qquickitemchangelistener_p.h>

DCORE_USE_NAMESPACE
DQUICK_BEGIN_NAMESPACE

class Q_DECL_HIDDEN MaskTextureCache {
public:
    class Texture : public QSharedData {
    public:
        Texture(QSGTexture *t)
            : texture(t)
        {
            MaskTextureCache::instance()->m_cache[cacheKey] = this;
        }

        ~Texture() {
            MaskTextureCache::instance()->m_cache.remove(cacheKey);
            delete texture;
        }

        qint8 cacheKey;
        QSGTexture *texture;
    };

    typedef QExplicitlySharedDataPointer<Texture> TextureData;

    static MaskTextureCache *instance()
    {
        static MaskTextureCache *object = new MaskTextureCache();
        return object;
    }

    // 根据圆角大小获取一个蒙版材质，此材质将用于片段着色器中实现圆角效果
    TextureData getTexture(QSGRenderContext *context, float radius, bool antialiasing)
    {
        // 排除无效的数据
        if (qIsNull(radius))
            return TextureData();

        // 用于获取材质缓存key的key
        qint8 to_cache_key_key = ((antialiasing << 7) | qRound(radius));
        Texture *texture = nullptr;

        if (m_radiusToCacheKey.contains(to_cache_key_key)) {
            texture = m_cache.value(m_radiusToCacheKey.value(to_cache_key_key));
        }

        if (!texture) {
            QImage mask(QSize(qRound(radius), qRound(radius)), QImage::Format_ARGB32);
            mask.fill(Qt::transparent);
            QPainter pa(&mask);
            pa.setRenderHint(QPainter::Antialiasing, antialiasing);
            QPainterPath path;
            path.moveTo(radius, radius);
            path.arcTo(0, 0, radius * 2,  radius * 2, 90, 90);
            path.lineTo(radius, radius);
            path.closeSubpath();
            // 必须填充为白色，在着色器中运算时会使用rgb三个通道相乘
            pa.fillPath(path, Qt::white);
            pa.end();

            texture = new Texture(context->createTexture(mask));
            texture->texture->setFiltering(QSGTexture::Nearest);
            texture->texture->setVerticalWrapMode(QSGTexture::ClampToEdge);
            texture->texture->setHorizontalWrapMode(QSGTexture::ClampToEdge);

            m_radiusToCacheKey[to_cache_key_key] = texture->cacheKey;
        }

        // 为窗口保存mask材质
        TextureData data(texture);

        return data;
    }

private:
    MaskTextureCache() {

    }

    QHash<qint8, Texture*> m_cache;
    QMap<int, qint8> m_radiusToCacheKey;

    friend class Texture;
};

class Q_DECL_HIDDEN DQuickItemViewportPrivate : public DCORE_NAMESPACE::DObjectPrivate, public QQuickItemChangeListener
{
public:
    enum DirtyStateBit {
        DirtyNothing = 0x0,
        DirtySourceTexture = 0x1,
        DirtySourceSizeRatio = 0x2,
        DirtyMaskTexture = 0x4,
        DirtyMaskSizeRatio = 0x8,
        DirtyMaskOffset = 0x10
    };
    Q_DECLARE_FLAGS(DirtyState, DirtyStateBit)

    DQuickItemViewportPrivate(DQuickItemViewport *qq)
        : DObjectPrivate(qq)
    {

    }

    ~DQuickItemViewportPrivate() override
    {
        resetOverrideMaterial(nullptr);
        // 清理对sourceItem的操作
        initSourceItem(sourceItem, nullptr);
    }

    inline void markDirtys(DirtyState states) {
        dirtyState |= states;
    }
    inline void markDirty(DirtyStateBit state, bool dirty = true) {
        if (dirty) {
            dirtyState |= state;
        } else {
            dirtyState &= ~state;
        }
    }
    // 重写此node对象的行为，确保使用到的是自定义的着色器
    void doOverrideMaterial(QSGImageNode *node);
    void resetOverrideMaterial(QSGImageNode *node = nullptr);
    // 根据radius获取对应的蒙版材质
    inline QSGTexture *textureForRadiusMask();
    void initSourceItem(QQuickItem *old, QQuickItem *item);

    void itemGeometryChanged(QQuickItem *item, QQuickGeometryChange data, const QRectF &) override;

    inline const QVector2D &getSoureSizeRatio() {
        if (Q_LIKELY(!dirtyState.testFlag(DirtySourceSizeRatio))) {
            return soureSizeRatio;
        }

        Q_ASSERT(sourceItem);
        D_Q(DQuickItemViewport);
        markDirty(DirtySourceSizeRatio, false);
        soureSizeRatio.setX(static_cast<float>(sourceItem->width() / q->width()));
        soureSizeRatio.setY(static_cast<float>(sourceItem->height() / q->height()));
        return soureSizeRatio;
    }
    inline const QVector2D &getMaskSizeRatio() {
        if (Q_LIKELY(!dirtyState.testFlag(DirtyMaskSizeRatio))) {
            return maskSizeRatio;
        }

        Q_ASSERT(radius > 0);
        D_Q(DQuickItemViewport);
        markDirty(DirtyMaskSizeRatio, false);
        maskSizeRatio.setX(static_cast<float>(q->width() / static_cast<qreal>(radius)));
        maskSizeRatio.setY(static_cast<float>(q->height() / static_cast<qreal>(radius)));
        return maskSizeRatio;
    }
    inline const QVector2D &getMaskOffset() {
        if (Q_LIKELY(!dirtyState.testFlag(DirtyMaskOffset))) {
            return maskOffset;
        }

        Q_ASSERT(sourceItem && sourceItem->width() > 0 && sourceItem->height() > 0);
        D_Q(DQuickItemViewport);
        markDirty(DirtyMaskOffset, false);
        auto offset = q->position() + sourceOffset;
        maskOffset.setX(static_cast<float>(offset.x() / sourceItem->width()));
        maskOffset.setY(static_cast<float>(offset.y() / sourceItem->height()));
        return maskOffset;
    }

    D_DECLARE_PUBLIC(DQuickItemViewport)

    QPointer<QQuickItem> sourceItem;
    // 记录sourceItem的大小是自身的多少倍
    QVector2D soureSizeRatio;
    // 显示圆角的mask材质
    MaskTextureCache::TextureData maskTexture;
    // item自身相对于圆角大小的比例
    QVector2D maskSizeRatio;
    // mask材质相对于sourceItem材质的偏移量
    QVector2D maskOffset;
    QMetaObject::Connection textureChangedConnection;
    // 自身parentItem位置相对于sourceItem的偏移量
    QPointF sourceOffset = QPointF(0, 0);
    QSGImageNode *overridedNode = nullptr;
    // 记录待更新的数据类型
    DirtyState dirtyState = DirtyNothing;
    // 圆角半径大小
    float radius = 0;

    static QHash<QSGMaterial*, DQuickItemViewportPrivate*> materialMap;
};
QHash<QSGMaterial*, DQuickItemViewportPrivate*> DQuickItemViewportPrivate::materialMap;
Q_DECLARE_OPERATORS_FOR_FLAGS(DQuickItemViewportPrivate::DirtyState)

static inline void overrideShaderUpdateState(QSGMaterialShader *shader, const QSGMaterialShader::RenderState &state,
                                      QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    auto viewport = DQuickItemViewportPrivate::materialMap[newMaterial];
    Q_ASSERT(viewport);
    auto gl = const_cast<QOpenGLContext *>(state.context())->functions();
    gl->glActiveTexture(GL_TEXTURE1);
    auto texture = viewport->textureForRadiusMask();
    texture->bind();
    gl->glActiveTexture(GL_TEXTURE0);
    shader->program()->setUniformValue("mask", 1);
    shader->program()->setUniformValue("maskScale", viewport->getMaskSizeRatio());
    shader->program()->setUniformValue("maskOffset", viewport->getMaskOffset());
    shader->program()->setUniformValue("sourceScale", viewport->getSoureSizeRatio());

    DVtableHook::callOriginalFun(shader, &QSGMaterialShader::updateState, state, newMaterial, oldMaterial);
}

static QSGMaterialType *overrideCreateOpacityShaderType(QSGMaterial *)
{
    static QSGMaterialType type;
    return &type;
}

static QSGMaterialType *overrideCreateOpaqueShaderType(QSGMaterial *)
{
    static QSGMaterialType type;
    return &type;
}

static QSGMaterialShader *overrideCreateShader(QSGMaterial *material)
{
    auto shader = DVtableHook::callOriginalFun(material, &QSGMaterial::createShader);

    if (material->type() == overrideCreateOpacityShaderType(material)) {
        shader->setShaderSourceFile(QOpenGLShader::Fragment,
                                    ":/dtk/declarative/shaders/quickitemviewport.frag");
    } else {
        shader->setShaderSourceFile(QOpenGLShader::Fragment,
                                    ":/dtk/declarative/shaders/quickitemviewport-opaque.frag");
    }
    DVtableHook::overrideVfptrFun(shader, &QSGMaterialShader::updateState, overrideShaderUpdateState);

    return shader;
}

static int overrideCompare(QSGMaterial *self, const QSGMaterial *other)
{
    return Q_UNLIKELY(self == other) ? 0 : 1;
}

void DQuickItemViewportPrivate::doOverrideMaterial(QSGImageNode *node)
{
    // createShader用于替换片段着色器
    DVtableHook::overrideVfptrFun(node->material(), &QSGMaterial::createShader, overrideCreateShader);
    // type函数被Renderer所调用，用于从shader缓存中取QSGMaterialShader对象，此处必须要改变此函数返回值
    // 否则会导致Renderer直接使用缓存的QSGMaterialShader对象，也就会导致没有用到quickitemviewport.frag
    DVtableHook::overrideVfptrFun(node->material(), &QSGMaterial::type, overrideCreateOpacityShaderType);
    // compare被用于判断是否应该调用QSGMaterialShader::updateState，由于quickitemviewport.frag中添加了
    // 一些跟DQuickItemViewport对象强相关的变量，因此需覆盖此方法，确保未每一个新的QSGMaterial调用updateState更新状态
    DVtableHook::overrideVfptrFun(node->material(), &QSGMaterial::compare, overrideCompare);
    DVtableHook::overrideVfptrFun(node->opaqueMaterial(), &QSGMaterial::createShader, overrideCreateShader);
    DVtableHook::overrideVfptrFun(node->opaqueMaterial(), &QSGMaterial::type, overrideCreateOpaqueShaderType);
    DVtableHook::overrideVfptrFun(node->opaqueMaterial(), &QSGMaterial::compare, overrideCompare);
    // 记录QSGMaterial到DQuickItemViewportPrivate的映射关系，用于updateState中获取数据
    materialMap[node->material()] = this;
    materialMap[node->opaqueMaterial()] = this;
    overridedNode = node;
}

void DQuickItemViewportPrivate::resetOverrideMaterial(QSGImageNode *node)
{
    if (node && node == overridedNode) {
        DVtableHook::resetVtable(node->material());
        DVtableHook::resetVtable(node->opaqueMaterial());
    }

    if (overridedNode) {
        for (auto key : materialMap.keys(this)) {
            materialMap.remove(key);
        }
    }
    overridedNode = nullptr;
}

QSGTexture *DQuickItemViewportPrivate::textureForRadiusMask()
{
    if (Q_UNLIKELY(dirtyState.testFlag(DirtyMaskTexture) || !maskTexture)) {
        QQuickItemPrivate *d = QQuickItemPrivate::get(q_func());
        maskTexture = MaskTextureCache::instance()->getTexture(d->sceneGraphRenderContext(),
                                                               static_cast<float>(radius * d->window->effectiveDevicePixelRatio()),
                                                               true);
        markDirty(DirtyMaskTexture, false);
    }

    return maskTexture->texture;
}

void DQuickItemViewportPrivate::initSourceItem(QQuickItem *old, QQuickItem *item)
{
    if (old) {
        QQuickItemPrivate *sd = QQuickItemPrivate::get(old);
        sd->removeItemChangeListener(this, QQuickItemPrivate::Geometry);
    }

    // 监听材质变化的信号
    if (textureChangedConnection)
        QObject::disconnect(textureChangedConnection);

    if (item) {
        QQuickItemPrivate *sd = QQuickItemPrivate::get(item);
        sd->addItemChangeListener(this, QQuickItemPrivate::Geometry);
    }
}

void DQuickItemViewportPrivate::itemGeometryChanged(QQuickItem *item, QQuickGeometryChange data, const QRectF &)
{
    D_Q(DQuickItemViewport);
    // 当DQuickItemViewport或其sourceItem的大小改变时应当更新sourceSizeRatio和maskOffset
    if (data.sizeChange()) {
        markDirtys(DirtySourceSizeRatio | DirtyMaskOffset);

        if (item == q_func())
            markDirty(DirtyMaskSizeRatio);
        q->update();
    }

    // 当DQuickItemViewport的位置改变时应当更新maskOffset
    if (data.positionChange() && item == q_func()) {
        markDirty(DirtyMaskOffset);
        q->update();
    }
}

/*!
 * \~chinese \class DQuickItemViewport
 * \~chinese \brief DQuickItemViewport 类是根据 \a sourceItem 属性设定的 \a QQuickItem
 * \~chinese 作为绘制时的材质来源，这个行为依赖于 \a QQuickItem::textureProvider 提供组件的
 * \~chinese 材质。故可将qml的 Image 和 ShaderEffectSource 作为 sourceItem 使用。
 * \~chinese
 * \~chinese 绘制材质的起始位置为：DQuickItemViewport 自身的位置 + \a sourceOffset 属性的值。
 * \~chinese 一般需将 \a sourceOffset 设置为 \a sourceItem 和 DQuickItemViewport 坐标系的
 * \~chinese 差值，当两者为兄弟关系时此值应该为 "0,0"，否则应该为 DQuickItemViewport 父组件相对
 * \~chinese 于 \a sourceItem 的位置。绘制材质的大小为 DQuickItemViewport 自身的组件大小。
 * \~chinese
 * \~chinese 当 \a radius 属性为 0 时，将直接使用 \a QSGImageNode 作为渲染节点，当其值大于 0
 * \~chinese 时，将使用 QPainter 渲染按 radius 的值绘制一张圆角 QImage 资源，并将其作为 mask
 * \~chinese 材质跟 \a sourceItem 的材质一起运算，并且会使用自定义的片段着色器为材质实现圆角效果。
 * \~chinese \note 自定义片段着色器是通过使用 \a DVtableHook::overrideVfptrFun 覆盖 \a QSGImageNode
 * \~chinese 的 \a QSGMaterial 成员的 createShader 虚函数实现的，故要注意 DVtableHook::overrideVfptrFun
 * \~chinese 在使用环境中的有效性，在一个陌生平台上有可能因为 DVtableHook 的功能失效而导致 \a radius
 * \~chinese 属性无效。
 * \~chinese \sa QQuickItem::textureProvider
 * \~chinese \sa QQuickItem::isTextureProvider
 * \~chinese \sa DVtableHook::overrideVfptrFun
 */
DQuickItemViewport::DQuickItemViewport(QQuickItem *parent)
    : QQuickItem (parent)
    , DCORE_NAMESPACE::DObject(*new DQuickItemViewportPrivate(this))
{
    QQuickItemPrivate::get(this)->addItemChangeListener(d_func(), QQuickItemPrivate::Geometry);
    setFlag(ItemHasContents);
}

QQuickItem *DQuickItemViewport::sourceItem() const
{
    D_DC(DQuickItemViewport);
    return d->sourceItem;
}

QPointF DQuickItemViewport::sourceOffset() const
{
    D_DC(DQuickItemViewport);
    return d->sourceOffset;
}

float DQuickItemViewport::radius() const
{
    D_DC(DQuickItemViewport);
    return d->radius;
}

void DQuickItemViewport::setSourceItem(QQuickItem *sourceItem)
{
    D_D(DQuickItemViewport);

    if (sourceItem && !sourceItem->isTextureProvider()) {
        qWarning() << "DQuickItemViewport: sourceItem is missing or not a texture provider";
        return;
    }

    if (d->sourceItem == sourceItem)
        return;

    if (isComponentComplete()) {
        d->initSourceItem(d->sourceItem, sourceItem);
    }

    d->sourceItem = sourceItem;
    d->markDirtys(DQuickItemViewportPrivate::DirtySourceTexture
                  | DQuickItemViewportPrivate::DirtySourceSizeRatio
                  | DQuickItemViewportPrivate::DirtyMaskOffset);
    Q_EMIT sourceItemChanged(d->sourceItem);
    update();
}

void DQuickItemViewport::setSourceOffset(QPointF sourceOffset)
{
    D_D(DQuickItemViewport);
    if (d->sourceOffset == sourceOffset)
        return;

    d->sourceOffset = sourceOffset;
    d->markDirty(DQuickItemViewportPrivate::DirtyMaskOffset);
    Q_EMIT sourceOffsetChanged(d->sourceOffset);
    update();
}

void DQuickItemViewport::setRadius(float radius)
{
    D_D(DQuickItemViewport);
    if (qFuzzyCompare(d->radius, radius))
        return;

    d->radius = radius;
    d->markDirtys(DQuickItemViewportPrivate::DirtyMaskSizeRatio
                  | DQuickItemViewportPrivate::DirtyMaskTexture);
    Q_EMIT radiusChanged(d->radius);
    update();
}

void DQuickItemViewport::itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &data)
{
    D_D(DQuickItemViewport);

    switch (static_cast<int>(change)) {
    case ItemAntialiasingHasChanged:
        d->markDirty(DQuickItemViewportPrivate::DirtyMaskTexture);
        update();
        break;
    case ItemDevicePixelRatioHasChanged:
        d->markDirtys(DQuickItemViewportPrivate::DirtyMaskTexture
                      | DQuickItemViewportPrivate::DirtyMaskSizeRatio);
        update();
        break;
    }

    QQuickItem::itemChange(change, data);
}

QSGNode *DQuickItemViewport::updatePaintNode(QSGNode *old, QQuickItem::UpdatePaintNodeData *)
{
    D_D(DQuickItemViewport);
    QSGImageNode *node = static_cast<QSGImageNode *>(old);
    if (!d->sourceItem || !d->sourceItem->textureProvider()->texture()) {
        d->resetOverrideMaterial(node);
        delete old;
        return nullptr;
    }

    // 计算sourceItem应该被绘制的区域，如果此区域大小为0, 则没有必要再继续绘制
    const QPointF &sourceOffset = position() + d->sourceOffset;
    const QRectF &targetRect = QRectF(QPointF(0, 0), size()) & QRectF(-sourceOffset, d->sourceItem->size());
    if (!targetRect.isValid()) {
        d->resetOverrideMaterial(node);
        delete old;
        return nullptr;
    }

    if (Q_UNLIKELY(!node)) {
        d->resetOverrideMaterial();
        // 创建image node
        auto qd = QQuickItemPrivate::get(this);
        node = qd->sceneGraphContext()->createImageNode();
        d->markDirty(DQuickItemViewportPrivate::DirtySourceTexture);
    }

    // 判断是否应该需要更新材质
    if (d->dirtyState.testFlag(DQuickItemViewportPrivate::DirtySourceTexture)) {
        auto provider = d->sourceItem->textureProvider();
        d->markDirty(DQuickItemViewportPrivate::DirtySourceTexture, false);
        node->setTexture(provider->texture());
        node->setOwnsTexture(false);

        if (!d->textureChangedConnection) {
            // 注意不要将此代码移动到别处，有些对象不允许在非渲染线程中获取 textureProvider
            auto onTextureChanged = [this, d] {
                d->markDirty(DQuickItemViewportPrivate::DirtySourceTexture);
                update();
            };

            d->textureChangedConnection = QObject::connect(provider,
                                                           &QSGTextureProvider::textureChanged,
                                                           this, onTextureChanged);
        }
    }

    if (d->radius > 0) {
        QSGRendererInterface *ri = window()->rendererInterface();
        if (Q_UNLIKELY(ri && ri->graphicsApi() == QSGRendererInterface::Software)) {
            // TODO(zccrs): 软件绘制时暂未支持radius属性
        } else {
            if (!d->overridedNode) {
                d->doOverrideMaterial(node);
            }

            // 为mask材质开启alpha通道混合功能
            node->material()->setFlag(QSGMaterial::Blending);
            node->opaqueMaterial()->setFlag(QSGMaterial::Blending);
        }
    } else if (d->overridedNode) {
        d->resetOverrideMaterial(node);
        // 重设blending模式
        bool hasAlpha = node->texture() && node->texture()->hasAlphaChannel();
        node->material()->setFlag(QSGMaterial::Blending, hasAlpha);
        node->opaqueMaterial()->setFlag(QSGMaterial::Blending, hasAlpha);
    }

    const QSizeF &textureSize = node->texture()->textureSize();
    qreal xScale = textureSize.width() / d->sourceItem->width();
    qreal yScale = textureSize.height() / d->sourceItem->height();

    node->setFiltering(smooth() ? QSGTexture::Linear : QSGTexture::Nearest);
    // 更新 DQuickItemViewport 所对应的sourceItem的材质区域
    node->setSourceRect(QRectF((targetRect.x() + sourceOffset.x()) * xScale,
                               (targetRect.y() + sourceOffset.y()) * yScale,
                               targetRect.width() * xScale, targetRect.height() * yScale));
    // 目标绘制区域
    node->setRect(targetRect);

    return node;
}

void DQuickItemViewport::componentComplete()
{
    D_D(DQuickItemViewport);

    if (d->sourceItem)
        d->initSourceItem(nullptr, d->sourceItem);

    return QQuickItem::componentComplete();
}

DQUICK_END_NAMESPACE
