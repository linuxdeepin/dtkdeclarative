// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include <QGuiApplication>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    // gerrit编译时没有显示器，需要指定环境变量
    qputenv("QT_QPA_PLATFORM", "offscreen");

#ifdef QT_NO_DEBUG
    QQuickStyle::setStyle("Chameleon");
#else
    QQuickStyle::setStyle(CHAMELEON_PATH"/Chameleon");
#endif

    QGuiApplication app(argc, argv);
    app.setApplicationName(BIN_NAME);
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
