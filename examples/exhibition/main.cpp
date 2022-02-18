/*
 * Copyright (C) 2022 UnionTech Technology Co., Ltd.
 *
 * Author:     JiDe Zhang <zhangjide@deepin.org>
 *
 * Maintainer: JiDe Zhang <zhangjide@deepin.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QDir>
#include <QFile>
#include <QUrl>
#include <QDebug>
#include <QQuickItem>

class Object : public QObject {
    Q_OBJECT
public:
    Q_INVOKABLE QByteArray readFile(const QUrl &url) const {
        QFile file(url.isLocalFile() ? url.toLocalFile() : (":" + url.path()));
        if (!file.open(QIODevice::ReadOnly))
            return QByteArray();
        return file.readAll();
    }
    Q_INVOKABLE void replace(QQuickItem *oldItem, QQuickItem *newItem) const {
        newItem->setParentItem(oldItem->parentItem());
        newItem->stackBefore(oldItem);
        oldItem->setParentItem(nullptr);
        QQmlEngine::setObjectOwnership(oldItem, QQmlEngine::CppOwnership);
        oldItem->deleteLater();
    }
};

int main(int argc, char **argv)
{
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QGuiApplication app(argc, argv);
    app.setApplicationName("dtk-exhibition");
    qputenv("D_POPUP_MODE", "embed");

#ifdef QT_NO_DEBUG
    QQuickStyle::setStyle("Chameleon");
#else
    QQuickStyle::setStyle(CHAMELEON_PATH"/Chameleon");
#endif
    QQmlApplicationEngine engine;

//    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::Software);
    engine.addImportPath(CHAMELEON_PATH);
    engine.rootContext()->setContextProperty("examplesFiles",
                                             QDir(":/examples").entryList({"*.qml"}));
    engine.rootContext()->setContextProperty("globalObject", new Object());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

#include "main.moc"
