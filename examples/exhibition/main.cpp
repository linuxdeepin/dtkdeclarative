// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
        oldItem->setParentItem(nullptr);
        QQmlEngine::setObjectOwnership(oldItem, QQmlEngine::CppOwnership);
        oldItem->deleteLater();
    }
};

int main(int argc, char **argv)
{
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    QGuiApplication app(argc, argv);
    app.setOrganizationName("deepin");
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
