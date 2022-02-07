/*
 * Copyright (C) 2020 ~ 2021 Deepin Technology Co., Ltd.
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

#include <gtest/gtest.h>

#include <QPointer>
#include <QPalette>

#include <DPalette>
#include <DFontManager>
#include <DWindowManagerHelper>
#include <DGuiApplicationHelper>

#include <private/dqmlglobalobject_p.h>

DQUICK_USE_NAMESPACE
DGUI_USE_NAMESPACE

class ut_DQMLGlobalObject : public ::testing::Test
{
public:
    virtual void SetUp();
    virtual void TearDown();

    QPointer<DQMLGlobalObject> pObj;
    QPointer<DWindowManagerHelper> pWMIns;
    QPointer<DGuiApplicationHelper> pAppIns;
};

void ut_DQMLGlobalObject::SetUp()
{
    pObj = new DQMLGlobalObject();
    pWMIns = DWindowManagerHelper::instance();
    pAppIns = DGuiApplicationHelper::instance();
}

void ut_DQMLGlobalObject::TearDown()
{
    if (!pObj.isNull())
        delete pObj.data();
}

TEST_F(ut_DQMLGlobalObject, hasBlurWindow)
{
    ASSERT_EQ(pWMIns->hasBlurWindow(), pObj->hasBlurWindow());
}

TEST_F(ut_DQMLGlobalObject, hasComposite)
{
    ASSERT_EQ(pWMIns->hasComposite(), pObj->hasComposite());
}

TEST_F(ut_DQMLGlobalObject, hasNoTitlebar)
{
    ASSERT_EQ(pWMIns->hasNoTitlebar(), pObj->hasNoTitlebar());
}

TEST_F(ut_DQMLGlobalObject, windowManagerName)
{
    ASSERT_EQ(pWMIns->windowManagerName(), pObj->windowManagerName());
}

TEST_F(ut_DQMLGlobalObject, themeType)
{
    ASSERT_EQ(pAppIns->themeType(), pObj->themeType());
}

TEST_F(ut_DQMLGlobalObject, windowManagerNameString)
{
    ASSERT_EQ(pWMIns->windowManagerNameString(), pObj->windowManagerNameString());
}

TEST_F(ut_DQMLGlobalObject, platformTheme)
{
    ASSERT_TRUE(pObj->platformTheme());
}

TEST_F(ut_DQMLGlobalObject, fontManager)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    qmlRegisterType<DFontManager>();
#else
    qmlRegisterAnonymousType<DFontManager>("", 1);
#endif
    ASSERT_TRUE(pObj->fontManager());
}

TEST_F(ut_DQMLGlobalObject, palette)
{
    DPalette expected = pAppIns->applicationPalette();
    DPalette actual = pObj->palette();
    for (int i = 0; i < QPalette::NColorRoles; ++i) {
        QPalette::ColorRole role = static_cast<QPalette::ColorRole>(i);
        ASSERT_EQ(expected.color(QPalette::Active, role), actual.color(QPalette::Active, role));
    }
}

TEST_F(ut_DQMLGlobalObject, inactivePalette)
{
    DPalette expected = pAppIns->applicationPalette();
    DPalette actual = pObj->inactivePalette();
    for (int i = 0; i < QPalette::NColorRoles; ++i) {
        QPalette::ColorRole role = static_cast<QPalette::ColorRole>(i);
        ASSERT_EQ(expected.color(QPalette::Inactive, role), actual.color(QPalette::Active, role));
    }
}
