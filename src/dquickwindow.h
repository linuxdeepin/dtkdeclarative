/*
 * Copyright (C) 2020 ~ 2020 Deepin Technology Co., Ltd.
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
#ifndef DWINDOW_H
#define DWINDOW_H
#include <QQuickWindow>
#include <DObject>

#include <dtkdeclarative_global.h>
#include <DWindowManagerHelper>

DQUICK_BEGIN_NAMESPACE

class DQuickWindowPrivate;
class DQuickWindow : public QQuickWindow, public DTK_CORE_NAMESPACE::DObject
{
    Q_OBJECT

    Q_PROPERTY(bool isValid READ isValid)
    Q_PROPERTY(int windowRadius READ windowRadius WRITE setWindowRadius NOTIFY windowRadiusChanged)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(int shadowRadius READ shadowRadius WRITE setShadowRadius NOTIFY shadowRadiusChanged)
    Q_PROPERTY(QPoint shadowOffset READ shadowOffset WRITE setShadowOffset NOTIFY shadowOffsetChanged)
    Q_PROPERTY(QColor shadowColor READ shadowColor WRITE setShadowColor NOTIFY shadowColorChanged)
    Q_PROPERTY(bool translucentBackground READ translucentBackground WRITE setTranslucentBackground NOTIFY translucentBackgroundChanged)
    Q_PROPERTY(bool enableSystemResize READ enableSystemResize WRITE setEnableSystemResize NOTIFY enableSystemResizeChanged)
    Q_PROPERTY(bool enableSystemMove READ enableSystemMove WRITE setEnableSystemMove NOTIFY enableSystemMoveChanged)
    Q_PROPERTY(bool enableBlurWindow READ enableBlurWindow WRITE setEnableBlurWindow NOTIFY enableBlurWindowChanged)
    Q_PROPERTY(DTK_GUI_NAMESPACE::DWindowManagerHelper::WmWindowTypes wmWindowTypes READ wmWindowTypes WRITE setWmWindowTypes NOTIFY wmWindowTypesChanged)

public:
    explicit DQuickWindow(QWindow *parent = nullptr);
    ~DQuickWindow() override;

    bool isValid() const;

    int windowRadius() const;

    int borderWidth() const;
    QColor borderColor() const;

    int shadowRadius() const;
    QPoint shadowOffset() const;
    QColor shadowColor() const;

    QRegion frameMask() const;

    bool translucentBackground() const;
    bool enableSystemResize() const;
    bool enableSystemMove() const;
    bool enableBlurWindow() const;

    DGUI_NAMESPACE::DWindowManagerHelper::WmWindowTypes wmWindowTypes() const;

public Q_SLOTS:
    void setWindowRadius(int windowRadius);

    void setBorderWidth(int borderWidth);
    void setBorderColor(const QColor &borderColor);

    void setShadowRadius(int shadowRadius);
    void setShadowOffset(const QPoint &shadowOffset);
    void setShadowColor(const QColor &shadowColor);

    void setTranslucentBackground(bool translucentBackground);
    void setEnableSystemResize(bool enableSystemResize);
    void setEnableSystemMove(bool enableSystemMove);
    void setEnableBlurWindow(bool enableBlurWindow);

    void setWmWindowTypes(DGUI_NAMESPACE::DWindowManagerHelper::WmWindowTypes wmWindowTypes);

Q_SIGNALS:
    void windowRadiusChanged();
    void borderWidthChanged();
    void borderColorChanged();
    void shadowRadiusChanged();
    void shadowOffsetChanged();
    void shadowColorChanged();
    void translucentBackgroundChanged();
    void enableSystemResizeChanged();
    void enableSystemMoveChanged();
    void enableBlurWindowChanged();
    void wmWindowTypesChanged(DGUI_NAMESPACE::DWindowManagerHelper::WmWindowTypes wmWindowTypes);

private:
    D_DECLARE_PRIVATE(DQuickWindow)
};

DQUICK_END_NAMESPACE

#endif // DWINDOW_H
