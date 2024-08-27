// SPDX-FileCopyrightText: 2024 - 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dquickradiuseffect.h"
#include "dquickradiuseffect_p.h"
#include "private/dsgradiusdefaultimagenode.h"

#include <private/qquickitem_p.h>

#include <QQuickItem>
#include <QSGTextureProvider>
#include <QRunnable>

DQUICK_BEGIN_NAMESPACE

class DQuickRadiusEffectTextureProvider : public QSGTextureProvider
{
    Q_OBJECT
public:
    DQuickRadiusEffectTextureProvider()
        : sourceTexture(nullptr)
        , mipmapFiltering(QSGTexture::None)
        , filtering(QSGTexture::Nearest)
        , horizontalWrap(QSGTexture::ClampToEdge)
        , verticalWrap(QSGTexture::ClampToEdge)
    {
    }

    QSGTexture *texture() const override {
        sourceTexture->setMipmapFiltering(mipmapFiltering);
        sourceTexture->setFiltering(filtering);
        sourceTexture->setHorizontalWrapMode(horizontalWrap);
        sourceTexture->setVerticalWrapMode(verticalWrap);
        return sourceTexture;
    }

    QSGLayer *sourceTexture;

    QSGTexture::Filtering mipmapFiltering;
    QSGTexture::Filtering filtering;
    QSGTexture::WrapMode horizontalWrap;
    QSGTexture::WrapMode verticalWrap;
};

class DQuickRadiusEffectCleanup : public QRunnable
{
public:
    DQuickRadiusEffectCleanup(QSGLayer *t, DQuickRadiusEffectTextureProvider *p)
        : texture(t)
        , provider(p)
    {}
    void run() override {
        delete texture;
        delete provider;
    }
    QSGLayer *texture;
    DQuickRadiusEffectTextureProvider *provider;
};

DQuickRadiusEffect::DQuickRadiusEffect(QQuickItem *parent)
    : QQuickItem(*(new DQuickRadiusEffectPrivate), parent)
{
    setFlag(ItemHasContents);
}

DQuickRadiusEffect::~DQuickRadiusEffect()
{
    Q_D(const DQuickRadiusEffect);

    if (window()) {
        window()->scheduleRenderJob(new DQuickRadiusEffectCleanup(d->texture, d->provider),
                                    QQuickWindow::AfterSynchronizingStage);
    } else {
        Q_ASSERT(!d->texture);
        Q_ASSERT(!d->provider);
    }

    if (d->sourceItem) {
        QQuickItemPrivate *sd = QQuickItemPrivate::get(d->sourceItem);
        sd->removeItemChangeListener(this, QQuickItemPrivate::Geometry);
        sd->derefFromEffectItem(d->hideSource);
        if (window())
            sd->derefWindow();
    }
}

qreal DQuickRadiusEffect::radius() const
{
    Q_D(const DQuickRadiusEffect);

    return d->radius;
}

void DQuickRadiusEffect::setRadius(qreal radius)
{
    Q_D(DQuickRadiusEffect);

    if (d->radius == radius)
        return;

    d->radius = radius;
    maybeSetImplicitAntialiasing();

    update();
    emit radiusChanged();

    if (d->extraRectangle.isAllocated()) {
        if (d->extraRectangle->topLeftRadius < 0.)
            emit topLeftRadiusChanged();
        if (d->extraRectangle->topRightRadius < 0.)
            emit topRightRadiusChanged();
        if (d->extraRectangle->bottomLeftRadius < 0.)
            emit bottomLeftRadiusChanged();
        if (d->extraRectangle->bottomRightRadius < 0.)
            emit bottomRightRadiusChanged();
    } else {
        emit topLeftRadiusChanged();
        emit topRightRadiusChanged();
        emit bottomLeftRadiusChanged();
        emit bottomRightRadiusChanged();
    }
}

qreal DQuickRadiusEffect::topLeftRadius() const
{
    Q_D(const DQuickRadiusEffect);

    if (d->extraRectangle.isAllocated() && d->extraRectangle->topLeftRadius >= 0.)
        return d->extraRectangle.value().topLeftRadius;

    return d->radius;
}

void DQuickRadiusEffect::setTopLeftRadius(qreal radius)
{
    Q_D(DQuickRadiusEffect);

    if (d->extraRectangle.value().topLeftRadius == radius)
        return;

    if (radius < 0) { // use the fact that radius < 0 resets the radius.
        qmlWarning(this) << "topLeftRadius (" << radius << ") cannot be less than 0.";
        return;
    }
    d->extraRectangle.value().topLeftRadius = radius;
    maybeSetImplicitAntialiasing();

    update();
    emit topLeftRadiusChanged();
}

void DQuickRadiusEffect::resetTopLeftRadius()
{
    Q_D(DQuickRadiusEffect);

    if (!d->extraRectangle.isAllocated())
        return;
    if (d->extraRectangle.value().topLeftRadius < 0)
        return;

    d->extraRectangle.value().topLeftRadius = -1.;
    maybeSetImplicitAntialiasing();

    update();
    emit topLeftRadiusChanged();
}

qreal DQuickRadiusEffect::topRightRadius() const
{
    Q_D(const DQuickRadiusEffect);

    if (d->extraRectangle.isAllocated()  && d->extraRectangle->topRightRadius >= 0.)
        return d->extraRectangle.value().topRightRadius;

    return d->radius;
}

void DQuickRadiusEffect::setTopRightRadius(qreal radius)
{
    Q_D(DQuickRadiusEffect);

    if (d->extraRectangle.value().topRightRadius == radius)
        return;

    if (radius < 0) { // use the fact that radius < 0 resets the radius.
        qmlWarning(this) << "topRightRadius (" << radius << ") cannot be less than 0.";
        return;
    }
    d->extraRectangle.value().topRightRadius = radius;
    maybeSetImplicitAntialiasing();

    update();
    emit topRightRadiusChanged();
}

void DQuickRadiusEffect::resetTopRightRadius()
{
    Q_D(DQuickRadiusEffect);

    if (!d->extraRectangle.isAllocated())
        return;
    if (d->extraRectangle.value().topRightRadius < 0)
        return;

    d->extraRectangle.value().topRightRadius = -1.;
    maybeSetImplicitAntialiasing();

    update();
    emit topRightRadiusChanged();
}

qreal DQuickRadiusEffect::bottomLeftRadius() const
{
    Q_D(const DQuickRadiusEffect);

    if (d->extraRectangle.isAllocated() && d->extraRectangle->bottomLeftRadius >= 0.)
        return d->extraRectangle.value().bottomLeftRadius;
    return d->radius;
}

void DQuickRadiusEffect::setBottomLeftRadius(qreal radius)
{
    Q_D(DQuickRadiusEffect);

    if (d->extraRectangle.value().bottomLeftRadius == radius)
        return;

    if (radius < 0) { // use the fact that radius < 0 resets the radius.
        qmlWarning(this) << "bottomLeftRadius (" << radius << ") cannot be less than 0.";
        return;
    }

    d->extraRectangle.value().bottomLeftRadius = radius;
    maybeSetImplicitAntialiasing();

    update();
    emit bottomLeftRadiusChanged();
}

void DQuickRadiusEffect::resetBottomLeftRadius()
{
    Q_D(DQuickRadiusEffect);

    if (!d->extraRectangle.isAllocated())
        return;
    if (d->extraRectangle.value().bottomLeftRadius < 0)
        return;

    d->extraRectangle.value().bottomLeftRadius = -1.;
    maybeSetImplicitAntialiasing();

    update();
    emit bottomLeftRadiusChanged();
}

qreal DQuickRadiusEffect::bottomRightRadius() const
{
    Q_D(const DQuickRadiusEffect);

    if (d->extraRectangle.isAllocated() && d->extraRectangle->bottomRightRadius >= 0.)
        return d->extraRectangle.value().bottomRightRadius;
    return d->radius;
}

void DQuickRadiusEffect::setBottomRightRadius(qreal radius)
{
    Q_D(DQuickRadiusEffect);

    if (d->extraRectangle.value().bottomRightRadius == radius)
        return;

    if (radius < 0) { // use the fact that radius < 0 resets the radius.
        qmlWarning(this) << "bottomRightRadius (" << radius << ") cannot be less than 0.";
        return;
    }

    d->extraRectangle.value().bottomRightRadius = radius;
    maybeSetImplicitAntialiasing();

    update();
    emit bottomRightRadiusChanged();
}

void DQuickRadiusEffect::resetBottomRightRadius()
{
    Q_D(DQuickRadiusEffect);

    if (!d->extraRectangle.isAllocated())
        return;
    if (d->extraRectangle.value().bottomRightRadius < 0)
        return;

    d->extraRectangle.value().bottomRightRadius = -1.;
    maybeSetImplicitAntialiasing();

    update();
    emit bottomRightRadiusChanged();
}

void DQuickRadiusEffect::maybeSetImplicitAntialiasing()
{
    Q_D(const DQuickRadiusEffect);

    bool implicitAA = (d->radius != 0);
    if (d->extraRectangle.isAllocated() && !implicitAA) {
        implicitAA = d->extraRectangle.value().topLeftRadius > 0.0
                     || d->extraRectangle.value().topRightRadius > 0.0
                     || d->extraRectangle.value().bottomLeftRadius > 0.0
                     || d->extraRectangle.value().bottomRightRadius > 0.0;
    }
    setImplicitAntialiasing(implicitAA);
}

void DQuickRadiusEffect::setImplicitAntialiasing(bool antialiasing)
{
    QQuickItemPrivate *d = QQuickItemPrivate::get(this);
    bool prev = this->antialiasing();
    d->implicitAntialiasing = antialiasing;
    if (d->componentComplete && (this->antialiasing() != prev))
        emit antialiasingChanged(this->antialiasing());
}

void DQuickRadiusEffect::ensureTexture()
{
    Q_D(DQuickRadiusEffect);

    if (d->texture)
        return;

    Q_ASSERT_X(QQuickItemPrivate::get(this)->window
                   && QQuickItemPrivate::get(this)->sceneGraphRenderContext()
                   && QThread::currentThread() == QQuickItemPrivate::get(this)->sceneGraphRenderContext()->thread(),
               "DQuickRadiusEffect::ensureTexture",
               "Cannot be used outside the rendering thread");

    if (d->sourceItem->textureProvider()) {
        d->texture = static_cast<QSGLayer *>(d->sourceItem->textureProvider()->texture());
        QSGPlainTexture *plaintexture = dynamic_cast<QSGPlainTexture *>(d->sourceItem->textureProvider()->texture());
        QSGDynamicTexture *dynamicTexture = dynamic_cast<QSGDynamicTexture *>(d->sourceItem->textureProvider()->texture());
        d->isQSGPlaintexture = plaintexture;
        d->isQSGDynamicTexture = dynamicTexture;
    } else {
        QSGRenderContext *rc = QQuickItemPrivate::get(this)->sceneGraphRenderContext();
        d->texture = rc->sceneGraphContext()->createLayer(rc);
        d->isQSGPlaintexture = false;
    }

    connect(QQuickItemPrivate::get(this)->window, SIGNAL(sceneGraphInvalidated()), d->texture, SLOT(invalidated()), Qt::DirectConnection);
    if (!d->isQSGPlaintexture && !d->isQSGDynamicTexture) {
        connect(d->texture, SIGNAL(updateRequested()), this, SLOT(update()));
        connect(d->texture, SIGNAL(scheduledUpdateCompleted()), this, SIGNAL(scheduledUpdateCompleted()));
    }
}

static void get_wrap_mode(DQuickRadiusEffect::WrapMode mode, QSGTexture::WrapMode *hWrap, QSGTexture::WrapMode *vWrap);

QSGTextureProvider *DQuickRadiusEffect::textureProvider() const
{
    Q_D(const DQuickRadiusEffect);

    if (!d->window || !d->sceneGraphRenderContext() || QThread::currentThread() != d->sceneGraphRenderContext()->thread()) {
        return nullptr;
    }

    if (!d->provider) {
        d->provider = new DQuickRadiusEffectTextureProvider();
        const_cast<DQuickRadiusEffect *>(this)->ensureTexture();
        connect(d->texture, SIGNAL(updateRequested()), d->provider, SIGNAL(textureChanged()));

        get_wrap_mode(d->wrapMode, &d->provider->horizontalWrap, &d->provider->verticalWrap);
        d->provider->mipmapFiltering = mipmap() ? QSGTexture::Linear : QSGTexture::None;
        d->provider->filtering = smooth() ? QSGTexture::Linear : QSGTexture::Nearest;
        d->provider->sourceTexture = d->texture;
    }
    return d->provider;
}

DQuickRadiusEffect::WrapMode DQuickRadiusEffect::wrapMode() const
{
    Q_D(const DQuickRadiusEffect);

    return d->wrapMode;
}

void DQuickRadiusEffect::setWrapMode(WrapMode mode)
{
    Q_D(DQuickRadiusEffect);

    if (mode == d->wrapMode)
        return;
    d->wrapMode = mode;
    update();
    emit wrapModeChanged();
}

QQuickItem *DQuickRadiusEffect::sourceItem() const
{
    Q_D(const DQuickRadiusEffect);

    return d->sourceItem;
}

void DQuickRadiusEffect::itemGeometryChanged(QQuickItem *item, QQuickGeometryChange change, const QRectF &)
{
    Q_D(const DQuickRadiusEffect);

    Q_ASSERT(item == d->sourceItem);
    Q_UNUSED(item);
    if (change.sizeChange())
        update();
}

void DQuickRadiusEffect::setSourceItem(QQuickItem *item)
{
    Q_D(DQuickRadiusEffect);

    if (item == d->sourceItem)
        return;
    if (d->sourceItem) {
        QQuickItemPrivate *sourceItemPrivate = QQuickItemPrivate::get(d->sourceItem);
        sourceItemPrivate->derefFromEffectItem(d->hideSource);
        sourceItemPrivate->removeItemChangeListener(this, QQuickItemPrivate::Geometry);
        disconnect(d->sourceItem, SIGNAL(destroyed(QObject*)), this, SLOT(sourceItemDestroyed(QObject*)));
        if (window())
            sourceItemPrivate->derefWindow();
    }

    d->sourceItem = item;

    if (d->sourceItem) {
        if (window() == d->sourceItem->window()
            || (window() == nullptr && d->sourceItem->window())
            || (d->sourceItem->window() == nullptr && window())) {
            QQuickItemPrivate *sourceItemPrivate = QQuickItemPrivate::get(d->sourceItem);
            if (window())
                sourceItemPrivate->refWindow(window());
            else if (d->sourceItem->window())
                sourceItemPrivate->refWindow(d->sourceItem->window());
            sourceItemPrivate->refFromEffectItem(d->hideSource);
            sourceItemPrivate->addItemChangeListener(this, QQuickItemPrivate::Geometry);
            connect(d->sourceItem, SIGNAL(destroyed(QObject*)), this, SLOT(sourceItemDestroyed(QObject*)));
        } else {
            qWarning("DRadiusEffect: sourceItem and DRadiusEffect must both be children of the same window.");
            d->sourceItem = nullptr;
        }
    }
    update();
    emit sourceItemChanged();
}

void DQuickRadiusEffect::sourceItemDestroyed(QObject *item)
{
    Q_D(DQuickRadiusEffect);

    Q_ASSERT(item == d->sourceItem);
    Q_UNUSED(item);
    d->sourceItem = nullptr;
    update();
    emit sourceItemChanged();
}

QRectF DQuickRadiusEffect::sourceRect() const
{
    Q_D(const DQuickRadiusEffect);

    return d->sourceRect;
}

void DQuickRadiusEffect::setSourceRect(const QRectF &rect)
{
    Q_D(DQuickRadiusEffect);

    if (rect == d->sourceRect)
        return;
    d->sourceRect = rect;
    update();
    emit sourceRectChanged();
}

QSize DQuickRadiusEffect::textureSize() const
{
    Q_D(const DQuickRadiusEffect);

    return d->textureSize;
}

void DQuickRadiusEffect::setTextureSize(const QSize &size)
{
    Q_D(DQuickRadiusEffect);

    if (size == d->textureSize)
        return;

    d->textureSize = size;
    update();
    emit textureSizeChanged();
}

DQuickRadiusEffect::Format DQuickRadiusEffect::format() const
{
    Q_D(const DQuickRadiusEffect);

    return d->format;
}

void DQuickRadiusEffect::setFormat(DQuickRadiusEffect::Format format)
{
    Q_D(DQuickRadiusEffect);

    if (format == d->format)
        return;

    d->format = format;
    update();
    emit formatChanged();
}

bool DQuickRadiusEffect::live() const
{
    Q_D(const DQuickRadiusEffect);

    return d->live;
}

void DQuickRadiusEffect::setLive(bool live)
{
    Q_D(DQuickRadiusEffect);

    if (live == d->live)
        return;

    d->live = live;
    update();
    emit liveChanged();
}

bool DQuickRadiusEffect::hideSource() const
{
    Q_D(const DQuickRadiusEffect);

    return d->hideSource;
}

void DQuickRadiusEffect::setHideSource(bool hide)
{
    Q_D(DQuickRadiusEffect);

    if (hide == d->hideSource)
        return;

    if (d->sourceItem) {
        QQuickItemPrivate::get(d->sourceItem)->refFromEffectItem(hide);
        QQuickItemPrivate::get(d->sourceItem)->derefFromEffectItem(d->hideSource);
    }
    d->hideSource = hide;
    update();
    emit hideSourceChanged();
}

bool DQuickRadiusEffect::mipmap() const
{
    Q_D(const DQuickRadiusEffect);

    return d->mipmap;
}

void DQuickRadiusEffect::setMipmap(bool enabled)
{
    Q_D(DQuickRadiusEffect);
    if (enabled == d->mipmap)
        return;

    d->mipmap = enabled;
    update();
    emit mipmapChanged();
}

bool DQuickRadiusEffect::recursive() const
{
    Q_D(const DQuickRadiusEffect);

    return d->recursive;
}

void DQuickRadiusEffect::setRecursive(bool enabled)
{
    Q_D(DQuickRadiusEffect);

    if (enabled == d->recursive)
        return;
    d->recursive = enabled;
    emit recursiveChanged();
}

DQuickRadiusEffect::TextureMirroring DQuickRadiusEffect::textureMirroring() const
{
    Q_D(const DQuickRadiusEffect);
    return DQuickRadiusEffect::TextureMirroring(d->textureMirroring);
}

void DQuickRadiusEffect::setTextureMirroring(TextureMirroring mirroring)
{
    Q_D(DQuickRadiusEffect);

    if (mirroring == DQuickRadiusEffect::TextureMirroring(d->textureMirroring))
        return;
    d->textureMirroring = mirroring;
    update();
    emit textureMirroringChanged();
}

int DQuickRadiusEffect::samples() const
{
    Q_D(const DQuickRadiusEffect);
    return d->samples;
}

void DQuickRadiusEffect::setSamples(int count)
{
    Q_D(DQuickRadiusEffect);

    if (count == d->samples)
        return;
    d->samples = count;
    update();
    emit samplesChanged();
}

void DQuickRadiusEffect::scheduleUpdate()
{
    Q_D(DQuickRadiusEffect);

    if (d->grab)
        return;
    d->grab = true;
    update();
}

static void get_wrap_mode(DQuickRadiusEffect::WrapMode mode, QSGTexture::WrapMode *hWrap, QSGTexture::WrapMode *vWrap)
{
    switch (mode) {
    case DQuickRadiusEffect::RepeatHorizontally:
        *hWrap = QSGTexture::Repeat;
        *vWrap = QSGTexture::ClampToEdge;
        break;
    case DQuickRadiusEffect::RepeatVertically:
        *vWrap = QSGTexture::Repeat;
        *hWrap = QSGTexture::ClampToEdge;
        break;
    case DQuickRadiusEffect::Repeat:
        *hWrap = *vWrap = QSGTexture::Repeat;
        break;
    default:
        *hWrap = *vWrap = QSGTexture::ClampToEdge;
        break;
    }
}


void DQuickRadiusEffect::releaseResources()
{
    Q_D(DQuickRadiusEffect);

    if (d->texture || d->provider) {
        window()->scheduleRenderJob(new DQuickRadiusEffectCleanup(d->texture, d->provider),
                                    QQuickWindow::AfterSynchronizingStage);
        d->texture = nullptr;
        d->provider = nullptr;
    }
}

class QQuickShaderSourceAttachedNode : public QObject, public QSGNode
{
    Q_OBJECT
public:
    Q_SLOT void markTextureDirty() {
        QSGNode *pn = QSGNode::parent();
        if (pn) {
            Q_ASSERT(pn->type() == QSGNode::GeometryNodeType);
            pn->markDirty(DirtyMaterial);
        }
    }
};

static QSGLayer::Format toLayerFormat(DQuickRadiusEffect::Format format)
{
    switch (format) {
    case DQuickRadiusEffect::RGBA8:
        return QSGLayer::RGBA8;
    case DQuickRadiusEffect::RGBA16F:
        return QSGLayer::RGBA16F;
    case DQuickRadiusEffect::RGBA32F:
        return QSGLayer::RGBA32F;
    default:
        return QSGLayer::RGBA8;
    }
}

QSGNode *DQuickRadiusEffect::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    Q_D(DQuickRadiusEffect);

    if (Q_UNLIKELY(!d->sourceItem || d->sourceItem->width() <= 0 || d->sourceItem->height() <= 0)) {
        if (d->texture)
            d->texture->setItem(nullptr);
        delete oldNode;
        return nullptr;
    }

    QSGRenderContext *rc = QQuickItemPrivate::get(this)->sceneGraphRenderContext();
    QSGRendererInterface *rif = rc->sceneGraphContext()->rendererInterface(rc);
    if (Q_UNLIKELY(!rif))
        return nullptr;

    ensureTexture();

    if (Q_LIKELY(!d->isQSGPlaintexture)) {
        d->texture->setLive(d->live);
        d->texture->setItem(QQuickItemPrivate::get(d->sourceItem)->itemNode());
    }
    QRectF sourceRect = d->sourceRect.width() == 0 || d->sourceRect.height() == 0
                            ? QRectF(0, 0, d->sourceItem->width(), d->sourceItem->height())
                            : d->sourceRect;
    d->texture->setRect(sourceRect);
    const float dpr = d->window->effectiveDevicePixelRatio();
    QSize textureSize = d->textureSize.isEmpty()
                            ? QSize(qCeil(qAbs(sourceRect.width())), qCeil(qAbs(sourceRect.height()))) * dpr
                            : d->textureSize;
    Q_ASSERT(!textureSize.isEmpty());

    const QSize minTextureSize = d->sceneGraphContext()->minimumFBOSize();
    while (textureSize.width() < minTextureSize.width())
        textureSize.rwidth() *= 2;
    while (textureSize.height() < minTextureSize.height())
        textureSize.rheight() *= 2;

    d->texture->setDevicePixelRatio(d->window->effectiveDevicePixelRatio());
    d->texture->setSize(textureSize);
    if (Q_LIKELY(!d->isQSGPlaintexture)) {
        d->texture->setRecursive(d->recursive);
    }
    d->texture->setFormat(toLayerFormat(d->format));
    d->texture->setHasMipmaps(d->mipmap);
    d->texture->setMirrorHorizontal(d->textureMirroring & MirrorHorizontally);
    d->texture->setMirrorVertical(d->textureMirroring & MirrorVertically);
    d->texture->setSamples(d->samples);

    if (Q_LIKELY(d->grab && !d->isQSGPlaintexture && !d->isQSGDynamicTexture))
        d->texture->scheduleUpdate();
    d->grab = false;

    QSGTexture::Filtering filtering = QQuickItemPrivate::get(this)->smooth
                                          ? QSGTexture::Linear
                                          : QSGTexture::Nearest;
    QSGTexture::Filtering mmFiltering = d->mipmap ? filtering : QSGTexture::None;
    QSGTexture::WrapMode hWrap, vWrap;
    get_wrap_mode(d->wrapMode, &hWrap, &vWrap);

    if (Q_LIKELY(d->provider)) {
        d->provider->mipmapFiltering = mmFiltering;
        d->provider->filtering = filtering;
        d->provider->horizontalWrap = hWrap;
        d->provider->verticalWrap = vWrap;
    }

    if (Q_UNLIKELY(width() <= 0 || height() <= 0)) {
        delete oldNode;
        return nullptr;
    }

    DSGRadiusDefaultImageNode *node = static_cast<DSGRadiusDefaultImageNode *>(oldNode);
    if (Q_LIKELY(!node)) {
        node = new DSGRadiusDefaultImageNode();
        node->setFlag(QSGNode::UsePreprocess);
        node->setTexture(d->texture);
        QQuickShaderSourceAttachedNode *attached = new QQuickShaderSourceAttachedNode;
        node->appendChildNode(attached);
        if (!d->isQSGPlaintexture)
            connect(d->texture, SIGNAL(updateRequested()), attached, SLOT(markTextureDirty()));
    }

    if (Q_LIKELY(d->live && d->recursive))
        node->markDirty(QSGNode::DirtyMaterial);

    node->setMipmapFiltering(mmFiltering);
    node->setFiltering(filtering);
    node->setHorizontalWrapMode(hWrap);
    node->setVerticalWrapMode(vWrap);
    node->setTargetRect(QRectF(0, 0, width(), height()));
    node->setInnerTargetRect(QRectF(0, 0, width(), height()));
    node->setRadius(d->radius);
    if (Q_LIKELY(d->extraRectangle.isAllocated())) {
        node->setTopLeftRadius(d->extraRectangle.value().topLeftRadius);
        node->setTopRightRadius(d->extraRectangle.value().topRightRadius);
        node->setBottomLeftRadius(d->extraRectangle.value().bottomLeftRadius);
        node->setBottomRightRadius(d->extraRectangle.value().bottomRightRadius);
    } else {
        node->setTopLeftRadius(-1.);
        node->setTopRightRadius(-1.);
        node->setBottomLeftRadius(-1.);
        node->setBottomRightRadius(-1.);
    }
    node->setAntialiasing(antialiasing());
    node->update();

    return node;
}

void DQuickRadiusEffect::invalidateSceneGraph()
{
    Q_D(DQuickRadiusEffect);

    if (d->texture)
        delete d->texture;
    if (d->provider)
        delete d->provider;
    d->texture = nullptr;
    d->provider = nullptr;
}

void DQuickRadiusEffect::itemChange(ItemChange change, const ItemChangeData &value)
{
    Q_D(DQuickRadiusEffect);

    if (change == QQuickItem::ItemSceneChange && d->sourceItem) {
        if (value.window)
            QQuickItemPrivate::get(d->sourceItem)->refWindow(value.window);
        else
            QQuickItemPrivate::get(d->sourceItem)->derefWindow();
    }
    QQuickItem::itemChange(change, value);
}

DQuickRadiusEffect::DQuickRadiusEffect(DQuickRadiusEffectPrivate &dd, QQuickItem *parent)
    : QQuickItem(dd, parent)
{
}

#include "dquickradiuseffect.moc"

DQUICK_END_NAMESPACE
