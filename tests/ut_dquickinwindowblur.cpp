// SPDX-FileCopyrightText: 2023 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "test_helper.hpp"

#include <QQuickItem>

#include "dquickinwindowblur_p.h"
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
// 回归用例需要访问 BackdropBlitter（Qt6 下 InWindowBlur 的内部实现）
#  include "dquickbackdropblitter_p.h"
#endif
#include <QSignalSpy>
DQUICK_USE_NAMESPACE

TEST(ut_DQuickInWindowBlur, properties)
{
    TEST_QRHI_SKIP();

    QuickViewHelper<> helper("qrc:/qml/DQuickInWindowBlur.qml");
    ASSERT_TRUE(helper.object);

    DQuickInWindowBlur *target = helper.object->findChild<DQuickInWindowBlur *>();
    ASSERT_TRUE(target);

    EXPECT_EQ(target->radius(), 10);

    QImage img = helper.view->grabWindow();
    // blured area
    EXPECT_TRUE(TestUtil::equalBlurColor(img.pixelColor(QPoint(50, 50)), TestUtil::simpleImage->pixelColor(QPoint(50, 50))));
    // don't be blured area
    EXPECT_NE(img.pixelColor(QPoint(90, 90)), TestUtil::simpleImage->pixelColor(QPoint(90, 90)));
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
// 回归：窗口销毁时 Renderer 的生命周期必须早于 QSGRenderContext（及其子对象
// ShaderManager）的销毁。修复前 RhiManager 在 QObject 子对象清理阶段才 delete
// renderer，~Renderer 访问已释放的 ShaderManager 必然 SIGSEGV（QSGBatchRenderer
// ::destroyGraphicsResources → ShaderManager::clearCachedRendererData，见
// dbackdropnode.cpp RhiManager::~RhiManager）。本用例在 RHI 渲染路径上先渲染一帧、
// 再随窗口销毁触发 invalidated()，修复前进程崩溃，修复后正常退出。
// 仅 Qt6 有效：Qt5 的 InWindowBlur 走 DQuickInWindowBlur C++ 类，不创建
// RhiManager，本用例场景在 Qt5 不存在。
TEST(ut_DQuickInWindowBlur, destroyWithBackdropNodeOnRhi)
{
    TEST_OFFSCREEN_SKIP();

    // D.InWindowBlur 在 Qt6 解析为 overridable QML（内含 D.BackdropBlitter），
    // 该元素在 updatePaintNode 中创建 DBackdropNode（RhiNode）并建立 RhiManager。
    // 必须确认 node 确实被创建、且启用 InWindowBlur 场景（非软件渲染、非被
    // DTK_DISABLE_INWINDOWBLUR 关闭），否则本用例什么都没测到。
    if (QQuickWindow::sceneGraphBackend() == QLatin1String("software"))
        GTEST_SKIP_("RhiManager 只在 RHI 渲染路径创建，软件渲染不适用本用例");

    QuickViewHelper<> helper("qrc:/qml/DQuickInWindowBlur.qml");
    ASSERT_TRUE(helper.object);

    // Qt6 下 D.InWindowBlur 是 overridable QML 类型（内含 D.BackdropBlitter），
    // 不再是 C++ 的 DQuickInWindowBlur；直接找 BackdropBlitter 实例。
    auto blurItem = helper.object->findChild<DQuickBackdropBlitter *>();
    if (!blurItem)
        GTEST_SKIP_("Qt6 InWindowBlur 未解析到 BackdropBlitter，环境不支持");
    if (!blurItem->blitterEnabled())
        GTEST_SKIP_("InWindowBlur 被禁用（DTK_DISABLE_INWINDOWBLUR 或 DConfig）");

    // 等真实渲染帧而非固定延时：RhiManager/Renderer 在渲染线程同步时建立，
    // 以 afterRendering（每帧从渲染线程发出、跨线程投递到主线程）作为"至少
    // 完成一帧"的信号。固定 qWait 在慢机器/高负载下可能首帧未完成就销毁，
    // 测试静默"假通过"（未建立 RhiManager 就销毁，什么都没测到）。
    QQuickWindow *window = helper.view.data();
    QSignalSpy renderSpy(window, &QQuickWindow::afterRendering);
    window->update(); // 若首帧已错过，强制再出一帧
    if (!renderSpy.wait(3000))
        GTEST_SKIP_("3s 内未等到渲染帧，无法确认 RhiManager 已建立，跳过本用例");

    // 销毁窗口：走 QQuickWindow::~QQuickWindow → windowDestroyed → context
    // invalidate() → delete；随后 QObject::deleteChildren 才轮到 RhiManager。
    // 修复前此行即 SIGSEGV。
    helper.clear();
}
#endif // QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
