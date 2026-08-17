// SPDX-FileCopyrightText: 2023 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "dquickdciiconimage_p.h"

#include <QQuickItem>
#include <QPointF>
#include <QtMath>

DQUICK_USE_NAMESPACE

TEST(ut_DQuickDciIconImage, properties)
{
    ControlHelper<> helper("qrc:/qml/DciIcon.qml");
    ASSERT_TRUE(helper.object);

    DQuickDciIconImage *target = helper.object->findChild<DQuickDciIconImage *>();
    EXPECT_EQ(target->name(), "switch_button");
    EXPECT_EQ(target->mode(), DTK_QUICK_NAMESPACE::DQMLGlobalObject::NormalState);
    EXPECT_EQ(target->theme(), DGuiApplicationHelper::LightType);
    EXPECT_EQ(target->sourceSize(), QSize(0, 0));
    EXPECT_EQ(target->mirror(), false);
    EXPECT_EQ(target->fallbackToQIcon(), true);
    QTest::qWait(1); // TODO: Resolve crash
    EXPECT_EQ(target->isNull("switch_button"), false);
}

TEST(ut_DQuickDciIconImage, image)
{
    QuickViewHelper<> helper("qrc:/qml/DciIcon.qml");
    ASSERT_TRUE(helper.object);

    auto windowImange = helper.object->window()->grabWindow();
    EXPECT_EQ(windowImange.pixelColor(QPoint(50, 50)), QColor(16, 131, 245)); // NOTICE: color ?
}

TEST(ut_DQuickDciIconImage, keepsImageCenteredWhenParentTransformChanges)
{
    QuickViewHelper<> helper("qrc:/qml/DciIconScaledParent.qml");
    ASSERT_TRUE(helper.object);

    auto root = qobject_cast<QQuickItem *>(helper.object);
    ASSERT_NE(root, nullptr);
    auto icon = root->findChild<DQuickDciIconImage *>("scaledDciIcon");
    ASSERT_NE(icon, nullptr);
    ASSERT_NE(icon->imageItem(), nullptr);

    auto expectedCenter = [icon]() {
        return QPointF((icon->width() - icon->imageItem()->width()) / 2.0,
                       (icon->height() - icon->imageItem()->height()) / 2.0);
    };

    QTRY_VERIFY(icon->imageItem()->width() > 0);
    QTRY_VERIFY(icon->imageItem()->height() > 0);

    // Ensure a layout triggered by the image size has settled while the parent
    // is still scaled, then finish the scale animation without another icon
    // geometry change.
    QTest::qWait(100);
    auto scaledParent = root->property("scaledParent").value<QQuickItem *>();
    ASSERT_NE(scaledParent, nullptr);
    scaledParent->setScale(1.0);

    // The ancestor transform change must invalidate the pixel-aligned local
    // offset created while the parent was still scaled.
    QTest::qWait(100);

    // The final position must stay visually centered and land on a physical
    // pixel. A sub-logical-pixel local offset is intentional for DPR alignment.
    qreal dpr = icon->window()->effectiveDevicePixelRatio();
    EXPECT_LE(qAbs(icon->imageItem()->x() - expectedCenter().x()), 1.0 / dpr);
    EXPECT_LE(qAbs(icon->imageItem()->y() - expectedCenter().y()), 1.0 / dpr);

    QPointF scenePos = icon->mapToScene(icon->imageItem()->position());
    EXPECT_DOUBLE_EQ(std::round(scenePos.x() * dpr), scenePos.x() * dpr);
    EXPECT_DOUBLE_EQ(std::round(scenePos.y() * dpr), scenePos.y() * dpr);
}
