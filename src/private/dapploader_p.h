/*
 * Copyright (C) 2020 ~ 2022 Deepin Technology Co., Ltd.
 *
 * Author:     Chen Bin <chenbin@uniontech.com>
 *
 * Maintainer: Chen Bin <chenbin@uniontech.com>
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

#ifndef DAPPLOADER_P_H
#define DAPPLOADER_P_H

#include <QString>
#include <DObjectPrivate>
#include <QQmlIncubationController>

#include "dapploader.h"
#include <private/qsgrenderloop_p.h>
#include <private/qquickitemchangelistener_p.h>
#include <private/qquicktransitionmanager_p_p.h>

QT_BEGIN_NAMESPACE
class QQmlApplicationEngine;
class QQuickItem;
class QQuickTransition;
QT_END_NAMESPACE

DQUICK_BEGIN_NAMESPACE

class DAppLoaderPrivate;
class DWindowOverlayTransitionManager : public QQuickTransitionManager
{
public:
    DWindowOverlayTransitionManager(DAppLoaderPrivate *loader);
    bool transitionLoaded();
protected:
    virtual void finished() override;
private:
    DAppLoaderPrivate *appLoader;
};

class DQuickAppLoaderItem;
class DQmlAppPreloadInterface;
class DQmlAppMainWindowInterface;
class DAppLoaderPrivate : public DTK_CORE_NAMESPACE::DObjectPrivate, public QQuickItemChangeListener
{
    friend class DQmlComponentIncubator;
public:
    DAppLoaderPrivate(DAppLoader *qq);
    void ensureLoadPreload();
    void ensureLoadMain();
    static QObject *ensureInstance(const QString &pluginPath);
    void destoryIncubator(QQmlIncubator *incubator);
    QQmlContext *creationContext(QQmlComponent *component, QObject *obj);
    bool createObjects(const char *propertyName);
    void createChildComponents();
    void initRootItem(QQuickItem *item);
    void updateRootItemSize(QQuickItem *item);
    virtual void itemGeometryChanged(QQuickItem *, QQuickGeometryChange, const QRectF &) override;
    void findWindowContentItem();
    void finishedLoaded();
    void createOverlay();
    void clearOverlay();
    void _q_onPreloadCreated(QObject *obj, const QUrl &);
    void _q_onMainComponentStatusChanged(QQmlComponent::Status status);
    void _q_onComponentProgressChanged();

    QString appName;
    QString mainQmlPlugin;
    QString preloadQmlPlugin;
    QQmlApplicationEngine *engine;
    QList<QQmlIncubator *> incubators;
    DQuickAppLoaderItem *appRootItem;
    QQuickItem *loadingOverlay;
    QPointer<QQuickItem> windowContentItem;
    QQuickTransition *exitedTransition;
    QQuickWindow *appWindow;
    QQmlComponent *mainComponent;
    bool overlayUsingParentSize;
    bool duringTransition;
    DWindowOverlayTransitionManager transitionManager;
    QHash<QQmlComponent *, QQmlContext *> itemContextMap;
    QScopedPointer<DQmlAppPreloadInterface> preloadInstance;
    QScopedPointer<DQmlAppMainWindowInterface> mainInstance;
    QScopedPointer<QGuiApplication> app;
private:
    D_DECLARE_PUBLIC(DAppLoader)
};

DQUICK_END_NAMESPACE

#endif //DAPPLOADER_P_H
