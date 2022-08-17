// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QPluginLoader>
#include <QQmlComponent>
#include <QScopedPointer>
#include <QUrl>
#include <QQuickWindow>
#include <QQuickItem>
#include <QQmlApplicationEngine>
#include <QQmlIncubator>

#include <private/qquickitem_p.h>
#include <private/qquicktransition_p.h>

#include "dapploader.h"
#include "dquickwindow.h"
#include "private/dquickapploaderitem_p.h"
#include "private/dqmlglobalobject_p.h"
#include "dqmlapppreloadinterface.h"
#include "dqmlappmainwindowinterface.h"
#include "private/dapploader_p.h"

DQUICK_BEGIN_NAMESPACE

#define APPLICATIONWINDOW_CONTENTDATA "contentData"
#define DIALOGWINDOW_CONTENTDATA "content"
#define WINDOW_CONTENTDATA "data"

#ifndef QT_DEBUG
Q_LOGGING_CATEGORY(appLoaderLog, "dtk.quick.apploader" , QtInfoMsg);
#else
Q_LOGGING_CATEGORY(appLoaderLog, "dtk.quick.apploader");
#endif

static const QQuickItemPrivate::ChangeTypes changedTypes = QQuickItemPrivate::Geometry;
DAppLoader *DAppLoader::self = nullptr;

class DQmlComponentIncubator : public QQmlIncubator
{
public:
    DQmlComponentIncubator(QQmlComponent *component, DAppLoaderPrivate *d, IncubationMode mode = AsynchronousIfNested);

protected:
    virtual void statusChanged(Status) override;
    virtual void setInitialState(QObject *) override;

private:
    DAppLoaderPrivate *appLoader;
    QQmlComponent *compnent;
};

DQmlComponentIncubator::DQmlComponentIncubator(QQmlComponent *component, DAppLoaderPrivate *appLoader, QQmlIncubator::IncubationMode mode)
    : QQmlIncubator(mode)
    , appLoader(appLoader)
    , compnent(component)
{

}

void DQmlComponentIncubator::statusChanged(QQmlIncubator::Status status)
{
    if (status != QQmlIncubator::Ready) {
        if (status == QQmlIncubator::Error)
            qCWarning(appLoaderLog) << "DAppLoader: " << this->errors();
        return;
    }

    QObject *obj = object();
    QQuickItem *item = qmlobject_cast<QQuickItem *>(obj);
    if (item)
        item->setParentItem(appLoader->appRootItem);
    appLoader->_q_onComponentProgressChanged();
    appLoader->destoryIncubator(this);
}

void DQmlComponentIncubator::setInitialState(QObject *o)
{
    if (!appLoader->appRootItem)
        return;

    if (o) {
        QQmlContext *context = appLoader->itemContextMap.value(compnent);
        if (context)
            QQml_setParent_noEvent(context, o);
        QQml_setParent_noEvent(o, appLoader->appRootItem);
    }
}

DWindowOverlayTransitionManager::DWindowOverlayTransitionManager(DAppLoaderPrivate *loader)
    : appLoader(loader)
{

}

bool DWindowOverlayTransitionManager::transitionLoaded()
{
    if (!appLoader->loadingOverlay || !appLoader->exitedTransition)
        return false;
    transition(QList<QQuickStateAction>(), appLoader->exitedTransition, appLoader->loadingOverlay);
    return true;
}

void DWindowOverlayTransitionManager::finished()
{
    appLoader->clearOverlay();
    appLoader->appRootItem->setFocus(true);
    appLoader->exitedTransition->deleteLater();
    appLoader->exitedTransition = nullptr;
    appLoader->duringTransition = false;
}

DAppLoaderPrivate::DAppLoaderPrivate(DAppLoader *qq)
    : DTK_CORE_NAMESPACE::DObjectPrivate(qq)
    , engine (nullptr)
    , appRootItem(nullptr)
    , loadingOverlay(nullptr)
    , windowContentItem(nullptr)
    , exitedTransition(nullptr)
    , appWindow(nullptr)
    , mainComponent(nullptr)
    , overlayUsingParentSize(false)
    , duringTransition(false)
    , transitionManager(this)
{
    DAppLoader::self = qq;
}

void DAppLoaderPrivate::ensureLoadPreload()
{
    if (preloadInstance)
        return;
    preloadInstance.reset(qobject_cast<DQmlAppPreloadInterface *>(ensureInstance(preloadQmlPlugin)));
    if (preloadInstance.isNull())
        qFatal("Preload plugin for \"%s\" is not found!", qPrintable(appName));
}

void DAppLoaderPrivate::ensureLoadMain()
{
    if (mainInstance)
        return;
    mainInstance.reset(qobject_cast<DQmlAppMainWindowInterface *>(ensureInstance(mainQmlPlugin)));
    if (mainInstance.isNull())
        qFatal("Main plugin for \"%s\" is not found!", qPrintable(appName));}

QObject *DAppLoaderPrivate::ensureInstance(const QString &pluginPath)
{
    QPluginLoader loader(pluginPath);
    QObject *instance = loader.instance();
    if (!instance) {
        qFatal("Can't load \"%s\", error message: %s", qPrintable(pluginPath), qPrintable(loader.errorString()));
    }

    return instance;
}

void DAppLoaderPrivate::destoryIncubator(QQmlIncubator *incubator)
{
    incubators.removeAll(incubator);
    if (incubators.isEmpty()) {
        finishedLoaded();
        appRootItem->setLoaded(true);
    }
    incubator->clear();
    delete incubator;
}

QQmlContext *DAppLoaderPrivate::creationContext(QQmlComponent *component, QObject *obj)
{
    QQmlContext *creationContext = component->creationContext();
    if (!creationContext)
        creationContext = qmlContext(obj);
    QQmlContext *context = new QQmlContext(creationContext);
    context->setContextObject(obj);
    this->itemContextMap.insert(component, context);
    return context;
}

bool DAppLoaderPrivate::createObjects(const char *propertyName)
{
    Q_ASSERT(mainComponent);
    QQmlListReference pros(appWindow, propertyName);
    if (!pros.isValid())
        return false;

    appRootItem = qobject_cast<DQuickAppLoaderItem *>(mainComponent->beginCreate(creationContext(mainComponent, appWindow)));
    Q_ASSERT_X(appRootItem, "DAppLoader", "Must use the AppLoader item in main component.");
    appRootItem->setWindow(appWindow);
    mainComponent->completeCreate();
    DQuickWindowAttached *attached = qobject_cast<DQuickWindowAttached*>(qmlAttachedPropertiesObject<DQuickWindow>(appWindow, false));
    if (attached) {
        attached->setAppLoader(appRootItem);
    }
    pros.append(appRootItem);
    initRootItem(windowContentItem);
    createChildComponents();
    return true;
}

void DAppLoaderPrivate::createChildComponents()
{
    auto components = appRootItem->findChildren<QQmlComponent *>(QStringLiteral(""), Qt::FindDirectChildrenOnly);
    for (auto childCom : qAsConst(components)) {
        QObject::connect(childCom, SIGNAL(progressChanged(qreal)), q_func(), SLOT(_q_onComponentProgressChanged()));
        auto asyn = appRootItem->asynchronous() ? DQmlComponentIncubator::Asynchronous : DQmlComponentIncubator::AsynchronousIfNested;
        DQmlComponentIncubator *incubator = new DQmlComponentIncubator(childCom, this, asyn);
        this->incubators.append(incubator);
        childCom->create(*incubator, creationContext(childCom, appRootItem));
    }
}

void DAppLoaderPrivate::initRootItem(QQuickItem *item)
{
    QQuickItemPrivate *wp = QQuickItemPrivate::get(item);
    wp->addItemChangeListener(this, changedTypes);
    // The appRootItem always fills the window.
    updateRootItemSize(item);
}

void DAppLoaderPrivate::updateRootItemSize(QQuickItem *item)
{
    if (!appRootItem)
        return;

    QQuickItemPrivate *ip = QQuickItemPrivate::get(item);
    if (ip->widthValid) {
        appRootItem->setWidth(item->width());
    } else {
        appRootItem->setWidth(appWindow->width());
    }

    if (ip->heightValid) {
        appRootItem->setHeight(item->height());
    } else {
        appRootItem->setWidth(appWindow->height());
    }
}

void DAppLoaderPrivate::itemGeometryChanged(QQuickItem *item, QQuickGeometryChange change, const QRectF &oldGeometry)
{
    if (loadingOverlay && !duringTransition) {
        if (overlayUsingParentSize)
            loadingOverlay->setSize(item->size());
    }
    updateRootItemSize(item);
    QQuickItemChangeListener::itemGeometryChanged(item, change, oldGeometry);
}

void DAppLoaderPrivate::finishedLoaded()
{
    D_Q(DAppLoader);
    Q_EMIT q->loadFinished();
    duringTransition = transitionManager.transitionLoaded();
    if (!duringTransition) {
        clearOverlay();
        appRootItem->setFocus(true);
    }
    mainInstance->finishedLoading(engine);
}

void DAppLoaderPrivate::createOverlay()
{
    if (loadingOverlay)
        return;
    DQuickWindowAttached *attached = qobject_cast<DQuickWindowAttached*>(qmlAttachedPropertiesObject<DQuickWindow>(appWindow, false));
    if (!attached)
        return;
    QQmlComponent *loComponent = attached->loadingOverlay();
    if (!loComponent)
        return;
    QQmlContext *context = creationContext(loComponent, windowContentItem);
    loadingOverlay = qobject_cast<QQuickItem *>(loComponent->beginCreate(context));
    if (!loadingOverlay)
        return;
    loadingOverlay->setParentItem(windowContentItem);
    loadingOverlay->setZ(DQMLGlobalObject::TopOrder);
    exitedTransition = attached->overlayExited();
    if (exitedTransition)
        engine->setObjectOwnership(exitedTransition, QQmlEngine::CppOwnership);
    loComponent->completeCreate();
    QQuickItemPrivate *overlayPrivate = QQuickItemPrivate::get(loadingOverlay);
    if (!overlayPrivate->widthValid || !overlayPrivate->heightValid) {
        overlayUsingParentSize = true;
        if (qFuzzyCompare(windowContentItem->width(), 0) || qFuzzyCompare(windowContentItem->height(), 0)) {
            loadingOverlay->setSize(appWindow->size());
        } else {
            loadingOverlay->setSize(windowContentItem->size());
        }
    }
    engine->setObjectOwnership(loadingOverlay, QQmlEngine::CppOwnership);
    return;
}

void DAppLoaderPrivate::clearOverlay()
{
    if (!loadingOverlay)
        return;
    loadingOverlay->setParentItem(nullptr);
    loadingOverlay->setVisible(false);
    loadingOverlay->deleteLater();
    loadingOverlay = nullptr;
    DQuickWindowAttached *attached = qobject_cast<DQuickWindowAttached*>(qmlAttachedPropertiesObject<DQuickWindow>(appWindow, false));
    if (attached) {
        QQmlContext *context = itemContextMap.take(attached->loadingOverlay());
        delete context;
    }
}

void DAppLoaderPrivate::findWindowContentItem()
{
    if (!appWindow)
        return;

    do {
        QVariant dataVariant = appWindow->property(APPLICATIONWINDOW_CONTENTDATA);
        if (dataVariant.isValid()) {
            // ApplicationWindow
            auto objectsData = dataVariant.value<QQmlListProperty<QObject>>();
            windowContentItem = qmlobject_cast<QQuickItem *>(objectsData.object);
            break;
        }

        dataVariant = appWindow->property(DIALOGWINDOW_CONTENTDATA);
        if (dataVariant.isValid()) {
            // DialogWindow
            auto itemsData = dataVariant.value<QQmlListProperty<QQuickItem>>();
            windowContentItem = qmlobject_cast<QQuickItem *>(itemsData.object);
            break;
        }
    } while (false);

    // Window
    if (!windowContentItem)
        windowContentItem = appWindow->contentItem();
}

void DAppLoaderPrivate::_q_onPreloadCreated(QObject *obj, const QUrl &)
{
    QQuickWindow *window = qmlobject_cast<QQuickWindow *>(obj);
    if (!window)
        return;

    appWindow = window;
    findWindowContentItem();
    Q_ASSERT(appWindow);
    Q_ASSERT(windowContentItem);
    // ensure the overlay shown and start the loading animation.
    createOverlay();

    ensureLoadMain();
    mainInstance->initialize(engine);
    // Insert component into preload
    const QUrl &mainComponentPath = mainInstance->mainComponentPath();
    mainComponent = new QQmlComponent(engine, mainComponentPath, QQmlComponent::Asynchronous);
    if (mainComponent->isLoading()) {
        QObject::connect(mainComponent, SIGNAL(statusChanged(QQmlComponent::Status)), q_func(), SLOT(_q_onMainComponentStatusChanged(QQmlComponent::Status)));
    } else {
        _q_onMainComponentStatusChanged(mainComponent->status());
    }
}


void DAppLoaderPrivate::_q_onMainComponentStatusChanged(QQmlComponent::Status status)
{
    if (status != QQmlComponent::Ready) {
        if (status == QQmlComponent::Error)
            qCWarning(appLoaderLog) << "DAppLoader: " << mainComponent->errors() << " " << mainComponent->errorString();
        return;
    }

    do {
        // For ApplicationWindow.
        if (createObjects(APPLICATIONWINDOW_CONTENTDATA))
            break;

        // For DialogWindow.
        if (createObjects(DIALOGWINDOW_CONTENTDATA))
            break;

        // For Window.
        if (createObjects(WINDOW_CONTENTDATA))
            break;
    } while (false);

    mainComponent->deleteLater();
    mainComponent = nullptr;
}

void DAppLoaderPrivate::_q_onComponentProgressChanged()
{
    qreal progress = 0;
    auto components = appRootItem->findChildren<QQmlComponent *>();
    for (auto childCom : qAsConst(components)) {
        progress += childCom->progress();
    }

    appRootItem->setProgress(progress / components.count());
}

/*!
 * \~chinese \brief DAppLoader::DAppLoader　用于加载DTk QML应用插件
 * \~chinese \param appName　　应用插件的名字
 * \~chinese \param appPath　　应用插件的安装目录
 */
DAppLoader::DAppLoader(const QString &appName, const QString &appPath, QObject *parent)
    : QObject (parent)
    , DTK_CORE_NAMESPACE::DObject(*new DAppLoaderPrivate(this))
{
    D_D(DAppLoader);
    QString path = appPath;

#ifdef DTK_QML_APP_PLUGIN_PATH
    if (path.isEmpty()) {
        path = DTK_QML_APP_PLUGIN_PATH;
    }
#endif
    d->appName = appName;
    d->mainQmlPlugin = path + "/libdtkqml-" + d->appName + ".so";
    d->preloadQmlPlugin = path + "/libdtkqml-" + d->appName + "-Preload" + ".so";
}

DAppLoader::~DAppLoader()
{
    D_D(DAppLoader);
    qDeleteAll(d->itemContextMap.values());
    delete d->engine;
    self = nullptr;
}

// it will enter the eventloop directly.
int DAppLoader::exec(int &argc, char **argv)
{
    D_D(DAppLoader);
    d->ensureLoadPreload();
    d->app.reset(d->preloadInstance->creatApplication(argc, argv));

    if (!d->app)
        return -1;
    if (!d->engine)
        d->engine = new QQmlApplicationEngine(this);
    auto graphics = d->preloadInstance->graphicsApi();
    if (graphics == QSGRendererInterface::Unknown) {
        const char *renderName = reinterpret_cast<const char *>(glGetString(GL_RENDER));
        if (renderName) {
            if (renderName != QByteArrayLiteral("LLVMPIPE")
                    || renderName != QByteArrayLiteral("SWRAST"))
                QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);
        }
    } else {
        QQuickWindow::setSceneGraphBackend(graphics);
    }
    d->preloadInstance->aboutToPreload(d->engine);
    QObject::connect(d->engine, SIGNAL(objectCreated(QObject *, const QUrl &)), this, SLOT(_q_onPreloadCreated(QObject *, const QUrl &)));
    d->engine->load(d->preloadInstance->preloadComponentPath());
    if (d->engine->rootObjects().isEmpty())
        return -1;
    return d->app->exec();
}

DQUICK_END_NAMESPACE
#include "moc_dapploader.cpp"
