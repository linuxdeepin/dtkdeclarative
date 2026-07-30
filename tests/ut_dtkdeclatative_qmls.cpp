// SPDX-FileCopyrightText: 2023 - 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <gtest/gtest.h>

#include "test_helper.hpp"
#include "dpopupwindowhandle_p.h"
#include <QQmlModuleRegistration>
#include <QQuickItem>
#include <QQuickWindow>
#if QT_VERSION_MAJOR >= 6
#include <qpa/qwindowsysteminterface.h>
#endif

DQUICK_USE_NAMESPACE

class ut_dtkDeclarativeQmls : public ::testing::TestWithParam<std::string> {
public:
    static std::list<std::string> qmls()
    {
        return std::list<std::string>{
            "SearchEdit.qml",
            "LineEdit.qml",
//            "ControlBackground.qml", // don't expose
            "PasswordEdit.qml",
            "ButtonBox.qml",
            "FloatingButton.qml",
            "IconButton.qml",
            "RecommandButton.qml",
            "ToolButton.qml",
            "WarningButton.qml",
            "FocusBoxBorder.qml",
            "CicleSpreadAnimation.qml",
            "TitleBar.qml",
            "DialogTitleBar.qml",
            "WindowButton.qml",
            "WindowButtonGroup.qml",
            "DialogWindow.qml",
            "AboutDialog.qml",
            "HelpAction.qml",
            "AboutAction.qml",
            "QuitAction.qml",
            "ThemeMenu.qml",
            "RectangularShadow.qml",
            "AlertToolTip.qml",
            "FlowStyle.qml",
            "Button.qml",
            "Control.qml",
            "PlusMinusSpinBox.qml",
//            "EditPanel.qml", // don't expose
            "TextArea.qml",
            "TextField.qml",
            "SpinBox.qml",
//            "SpinBoxIndicator.qml", // don't expose
            "ApplicationWindow.qml",
            "BoxShadow.qml",
            "FloatingPanel.qml",
            "FloatingMessage.qml",
            "Menu.qml",
            "MenuItem.qml",
            "MenuSeparator.qml",
            "HighlightPanel.qml",
            "ArrowListView.qml",
            "ScrollIndicator.qml",
            "StyledBehindWindowBlur.qml",
            "Popup.qml",
            "PopupWindow.qml",
            "Switch.qml",
            "Slider.qml",
//            "SliderHandle.qml", // don't expose
            "ScrollBar.qml",
            "ComboBox.qml",
            "ActionButton.qml",
            "ItemDelegate.qml",
            "CheckDelegate.qml",
            "TipsSlider.qml",
            "SliderTipItem.qml",
            "BoxInsetShadow.qml",
            "CheckBox.qml",
            "IpV4LineEdit.qml",
            "OutsideBoxBorder.qml",
            "InsideBoxBorder.qml",
            "SortFilterModel.qml",
//            "ArrowShapePopupWindow.qml", // don't expose
//            "StyledArrowShapeWindow.qml", // error
            "ArrowShapePopup.qml",
            "Action.qml",
            "ActionGroup.qml",
            "Label.qml",
            "Container.qml",
            "ButtonGroup.qml",
            "ScrollView.qml",
            "MenuBar.qml",
            "AbstractButton.qml",
            "TabBar.qml",
            "SwipeDelegate.qml",
            "SwipeView.qml",
            "Drawer.qml",
            "Dial.qml",
            "Dialog.qml",
            "DelayButton.qml",
            "RoundButton.qml",
            "RadioButton.qml",
            "ToolTip.qml",
            "DialogButtonBox.qml",
            "Frame.qml",
            "GroupBox.qml",
            "PageIndicator.qml",
            "StackView.qml",
            "BusyIndicator.qml",
            "ProgressBar.qml",
            "BoxPanel.qml",
            "Pane.qml",
            "KeySequenceEdit.qml",
            "ButtonIndicator.qml",
            "EmbeddedProgressBar.qml",
            "WaterProgressBar.qml",
//            "PlaceholderText.qml" // don't expose
        };
    }
    ControlHelper<> helper;
};

class ut_dtkDeclarativeQmlPrivates : public ut_dtkDeclarativeQmls
{
public:
    static std::list<std::string> qmls()
    {
        return std::list<std::string>{
//            "ProgressBarImpl.qml", // don't expose
//            "ProgressBarPanel.qml", // don't expose
//            "ColorOverlay.qml", // don't expose
//            "SoftwareColorOverlay.qml", // don't expose
//            "ArrowListViewButton.qml", // don't expose
            "ButtonPanel.qml",
//            "KeySequenceLabel.qml" // don't expose
        };
    }
};

class ut_dtkDeclarativeQmlSettings : public ut_dtkDeclarativeQmls
{
public:
    static std::list<std::string> qmls()
    {
        return std::list<std::string>{
            "SettingsDialog.qml",
            "OptionDelegate.qml",
            "CheckBox.qml",
            "LineEdit.qml",
            "ComboBox.qml",
            "NavigationTitle.qml",
            "ContentTitle.qml",
            "ContentBackground.qml"
        };
    }
};

#if QT_VERSION_MAJOR >= 6
TEST(ut_DPopupWindowHandle, popupWindowCreatesHandle)
{
    ControlHelper<> helper;
    const QByteArray data(R"(
        import QtQuick
        import QtQuick.Controls
        import org.deepin.dtk 1.0 as D

        Item {
            property alias popup: popup

            D.Popup {
                id: popup
                popupType: Popup.Window
            }
        }
    )");

    ASSERT_TRUE(helper.setData(data));
    QObject *popup = qvariant_cast<QObject *>(helper.object->property("popup"));
    ASSERT_TRUE(popup);
    EXPECT_TRUE(qmlAttachedPropertiesObject<DPopupWindowHandle>(popup, false));
}

TEST(ut_DPopupWindowHandle, comboBoxKeepsPopupOpenWhenFocusMovesToPopupWindow)
{
    ControlHelper<QQuickWindow> helper;
    const QByteArray data(R"(
        import QtQuick
        import QtQuick.Templates as T
        import org.deepin.dtk 1.0 as D

        Window {
            width: 400
            height: 300
            property alias control: control

            T.ComboBox {
                id: control
                width: 200
                model: 3
                popup: D.Popup {
                    width: 200
                    height: 160
                    popupType: T.Popup.Window
                    focus: false
                    closeOnInactive: false
                    contentItem: Item { }
                }
            }
        }
    )");

    ASSERT_TRUE(helper.setData(data));
    helper.requestExposed();

    auto *control = qvariant_cast<QQuickItem *>(helper.object->property("control"));
    ASSERT_TRUE(control);
    auto *popup = qvariant_cast<QObject *>(control->property("popup"));
    ASSERT_TRUE(popup);

    QWindowSystemInterface::handleFocusWindowChanged(helper.object, Qt::OtherFocusReason);
    QCoreApplication::processEvents();
    control->forceActiveFocus(Qt::OtherFocusReason);
    ASSERT_TRUE(control->hasActiveFocus());

    ASSERT_TRUE(QMetaObject::invokeMethod(popup, "open", Qt::DirectConnection));
    QCoreApplication::processEvents();
    ASSERT_TRUE(popup->property("visible").toBool());

    auto *contentItem = qvariant_cast<QQuickItem *>(popup->property("contentItem"));
    ASSERT_TRUE(contentItem);
    QQuickWindow *popupWindow = contentItem->window();
    ASSERT_TRUE(popupWindow);
    ASSERT_NE(popupWindow, helper.object);

    QWindowSystemInterface::handleFocusWindowChanged(popupWindow, Qt::PopupFocusReason);
    QCoreApplication::processEvents();

    EXPECT_TRUE(popup->property("visible").toBool());

    QWindowSystemInterface::handleFocusWindowChanged(helper.object, Qt::PopupFocusReason);
    QMetaObject::invokeMethod(popup, "close", Qt::DirectConnection);
    QCoreApplication::processEvents();
}
#endif

TEST_P(ut_dtkDeclarativeQmls, load)
{
    QString path = QString::fromStdString(GetParam());
    QString qmlType = path.chopped(4);

    const QString data(QString(u8R"(
                    import QtQuick.Window 2.11
                    import org.deepin.dtk 1.0 as D
                    Window {
                        D.%1 {
                        }
                    })").arg(qmlType));

    EXPECT_TRUE(helper.setData(data.toLocal8Bit()));
}
INSTANTIATE_TEST_SUITE_P(loadQml, ut_dtkDeclarativeQmls, ::testing::ValuesIn(ut_dtkDeclarativeQmls::qmls()));

TEST_P(ut_dtkDeclarativeQmlPrivates, load)
{
    QString path = QString::fromStdString(GetParam());
    QString qmlType = path.chopped(4);

    const QString data(QString(u8R"(
                    import QtQuick.Window 2.11
                    import org.deepin.dtk.private 1.0 as P
                    Window {
                        P.%1 {
                        }
                    })").arg(qmlType));

    EXPECT_TRUE(helper.setData(data.toLocal8Bit()));
}
INSTANTIATE_TEST_SUITE_P(load, ut_dtkDeclarativeQmlPrivates, ::testing::ValuesIn(ut_dtkDeclarativeQmlPrivates::qmls()));

TEST_P(ut_dtkDeclarativeQmlSettings, load)
{
    QString path = QString::fromStdString(GetParam());
    QString qmlType = path.chopped(4);

    const QString data(QString(u8R"(
                    import QtQuick.Window 2.11
                    import org.deepin.dtk.settings 1.0 as Settings
                    Window {
                        Settings.%1 {
                        }
                    })").arg(qmlType));

    EXPECT_TRUE(helper.setData(data.toLocal8Bit()));
}
INSTANTIATE_TEST_SUITE_P(load, ut_dtkDeclarativeQmlSettings, ::testing::ValuesIn(ut_dtkDeclarativeQmlSettings::qmls()));
