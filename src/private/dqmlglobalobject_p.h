// SPDX-FileCopyrightText: 2020 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef DQMLGLOBALOBJECT_P_H
#define DQMLGLOBALOBJECT_P_H

#include <dtkdeclarative_global.h>

#include <DPlatformThemeProxy>
#include <DWindowManagerHelper>
#include <DGuiApplicationHelper>
#include <DDciIconPalette>
#include <DObject>

#include <QQuickWindow>
#include <QQmlComponent>

DGUI_BEGIN_NAMESPACE
class DFontManager;
DGUI_END_NAMESPACE

DGUI_USE_NAMESPACE

DQUICK_BEGIN_NAMESPACE

class DColor
{
    Q_GADGET
public:
    enum Type : quint8 {
        Invalid = 0,
        Highlight,
        HighlightedText
    };
    Q_ENUM(Type)

    DColor() {}
    inline DColor(const QColor &color) {
        data.color.value = color;
    }
    DColor(Type type);
    inline DColor(const DColor &other) {
        memcpy(static_cast<void*>(&data), static_cast<const void*>(&other.data), sizeof(Data));
    }
    inline DColor(DColor &&other) {
        operator =(other);
    }

    [[nodiscard]] bool isValid() const noexcept;
    [[nodiscard]] bool isTypedColor() const noexcept;

    bool operator==(const DColor &c) const noexcept;
    bool operator!=(const DColor &c) const noexcept;
    inline DColor &operator=(const QColor &color) {
        data.color.value = color;
        return *this;
    }
    inline DColor &operator=(const DColor &other) {
        memcpy(static_cast<void*>(&data), static_cast<const void*>(&other.data), sizeof(Data));
        return *this;
    }
    inline DColor &operator=(DColor &&other) {
        data.hue = std::move(other.data.hue);
        data.saturation = std::move(other.data.saturation);
        data.lightness = std::move(other.data.lightness);
        data.opacity = std::move(other.data.opacity);
        data.color.value = std::move(other.data.color.value);
        return *this;
    }

    Q_INVOKABLE QColor toColor(const QPalette &palette) const;
    Q_INVOKABLE QColor color() const;
    Q_INVOKABLE DTK_QUICK_NAMESPACE::DColor hue(qint8 floatValue) const;
    Q_INVOKABLE DTK_QUICK_NAMESPACE::DColor saturation(qint8 floatValue) const;
    Q_INVOKABLE DTK_QUICK_NAMESPACE::DColor lightness(qint8 floatValue) const;
    Q_INVOKABLE DTK_QUICK_NAMESPACE::DColor opacity(qint8 floatValue) const;

private:
    struct Data {
        qint8 hue = 0;
        qint8 saturation = 0;
        qint8 lightness = 0;
        qint8 opacity = 0;

        union Color {
    #ifdef Q_COMPILER_UNIFORM_INIT
            Color() {} // doesn't init anything, thus can't be constexpr
    #endif
            quint8 type; // DQMLGlobalObject::TypedColor
            QColor value = QColor();
        } color;
    } data;
};

class DQuickDciIcon;
class FloatingMessageContainer;
class DQMLGlobalObjectPrivate;
class DQMLGlobalObject : public QObject, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool hasBlurWindow READ hasBlurWindow NOTIFY hasBlurWindowChanged)
    Q_PROPERTY(bool hasComposite READ hasComposite NOTIFY hasCompositeChanged)
    Q_PROPERTY(bool hasNoTitlebar READ hasNoTitlebar NOTIFY hasNoTitlebarChanged)
    Q_PROPERTY(bool isSoftwareRender READ isSoftwareRender FINAL CONSTANT)
    Q_PROPERTY(DTK_GUI_NAMESPACE::DWindowManagerHelper::WMName windowManagerName READ windowManagerName CONSTANT)
    Q_PROPERTY(DTK_GUI_NAMESPACE::DGuiApplicationHelper::ColorType themeType READ themeType NOTIFY themeTypeChanged)
    Q_PROPERTY(QString windowManagerNameString READ windowManagerNameString CONSTANT)
    Q_PROPERTY(DPlatformThemeProxy *platformTheme READ platformTheme FINAL CONSTANT)
    Q_PROPERTY(DTK_GUI_NAMESPACE::DFontManager *fontManager READ fontManager FINAL CONSTANT)
    Q_PROPERTY(QPalette palette READ palette NOTIFY paletteChanged)
    Q_PROPERTY(QPalette inactivePalette READ inactivePalette NOTIFY inactivePaletteChanged)
    Q_PROPERTY(QString deepinDistributionOrgLogo READ deepinDistributionOrgLogo CONSTANT)
    Q_PROPERTY(QString deepinWebsiteName READ deepinWebsiteName CONSTANT)
    Q_PROPERTY(QString deepinWebsiteLink READ deepinWebsiteLink CONSTANT)

public:
    explicit DQMLGlobalObject(QObject *parent = nullptr);
    ~DQMLGlobalObject() override;

    enum ControlState {
        NormalState,
        HoveredState,
        PressedState,
        DisabledState,
        InactiveState
    };
    Q_ENUM(ControlState)

    enum ZOrder {
        BottomOrder = -99,
        BelowOrder = -1,
        NormalOrder = 0,
        AboveOrder = 1,
        TopOrder = 99
    };
    Q_ENUM(ZOrder)

    enum PopupMode {
        AutoMode,
        WindowMode,
        EmbedMode
    };
    Q_ENUM(PopupMode)

    bool hasBlurWindow() const;
    bool hasComposite() const;
    bool hasNoTitlebar() const;
    bool isSoftwareRender() const;

    DWindowManagerHelper::WMName windowManagerName() const;
    QString windowManagerNameString() const;

    DGuiApplicationHelper::ColorType themeType() const;

    DPlatformThemeProxy *platformTheme() const;
    DFontManager *fontManager() const;

    QPalette palette() const;
    QPalette inactivePalette() const;

    Q_INVOKABLE QColor blendColor(const QColor &substrate, const QColor &superstratum);
    Q_INVOKABLE DTK_QUICK_NAMESPACE::DColor makeColor(DTK_QUICK_NAMESPACE::DColor::Type type);
    Q_INVOKABLE DTK_QUICK_NAMESPACE::DColor makeColor(const QColor &color);
    Q_INVOKABLE QUrl makeShadowImageUrl(qreal boxSize, qreal cornerRadius, qreal shadowBlur,
                                        QColor color, qreal xOffset, qreal yOffset, qreal spread,
                                        bool hollow, bool inner);

    Q_INVOKABLE DTK_GUI_NAMESPACE::DGuiApplicationHelper::ColorType toColorType(const QColor &color);
    Q_INVOKABLE QColor selectColor(const QColor &windowColor, const QColor &light, const QColor &dark);

    QString deepinWebsiteName() const;
    QString deepinWebsiteLink() const;
    QString deepinDistributionOrgLogo() const;

    Q_INVOKABLE QPoint cursorPosition() const;

    Q_INVOKABLE DTK_QUICK_NAMESPACE::DQuickDciIcon makeIcon(const QJSValue &qicon, const QJSValue &iconExtra);
    Q_INVOKABLE DTK_GUI_NAMESPACE::DDciIconPalette makeIconPalette(const QPalette &palette);

    Q_INVOKABLE bool sendMessage(QObject *target, const QString &content, const QString &iconName = QString(), int duration = 4000, const QString &msgId = QString());
    Q_INVOKABLE bool sendMessage(QObject *target, QQmlComponent *delegate, const QVariant &message, int duration = 4000, const QString &msgId = QString());
    Q_INVOKABLE void closeMessage(DTK_QUICK_NAMESPACE::FloatingMessageContainer *message);
    Q_INVOKABLE void closeMessage(QObject *target, const QString &msgId);
    Q_INVOKABLE void sendSystemMessage(const QString &summary, const QString &body = QString(),
                                       const QString &appIcon = QString(), const QStringList &actions = QStringList(),
                                       const QVariantMap hints = QVariantMap(), const int timeout = 3000, const uint replaceId = 0);

    static void setPopupMode(const PopupMode mode);

Q_SIGNALS:
    void hasBlurWindowChanged();
    void hasCompositeChanged();
    void hasNoTitlebarChanged();
    void paletteChanged();
    void inactivePaletteChanged();
    void themeTypeChanged(DGuiApplicationHelper::ColorType themeType);

private:
    D_DECLARE_PRIVATE(DQMLGlobalObject)
    D_PRIVATE_SLOT(void _q_onPaletteChanged())
};

DQUICK_END_NAMESPACE
Q_DECLARE_METATYPE(DTK_QUICK_NAMESPACE::DColor)

#endif // DQMLGLOBALOBJECT_P_H
