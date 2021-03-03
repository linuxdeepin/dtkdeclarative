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

#include "dquickbusyindicator_p.h"

#include <QtCore/qmath.h>
#include <QtQuick/private/qquickitem_p.h>

DQUICK_BEGIN_NAMESPACE

static const int CircleCount = 3;
static const QRgb TransparentColor = 0x00000000;

static QList<QColor> createDefaultIndicatorColorList(QColor color)
{
    QList<QColor> colors;
    QList<int> opacitys;
    // indicator 的阴影使用降低透明度的方式显示
    opacitys << 100 << 30 << 15 << 10 << 5 << 4 << 3 << 2 << 1;
    for (int i = 0; i < opacitys.count(); ++i) {
        color.setAlpha(255 * opacitys.value(i) / 100);
        colors << color;
    }

    return colors;
}

DQuickBusyIndicatorNode::DQuickBusyIndicatorNode(DQuickBusyIndicator *item)
    : m_spinning(false)
    , m_rotation(0)
    , m_center(0, 0)
    , m_window(item->window())
{
    connect(item->window(), &QQuickWindow::beforeRendering, this, &DQuickBusyIndicatorNode::maybeRotate, Qt::DirectConnection);
    connect(item->window(), &QQuickWindow::frameSwapped, this, &DQuickBusyIndicatorNode::maybeUpdate, Qt::DirectConnection);

    int totalCircleCount = 0;
    // 获取每个 indicator 阴影的颜色值
    for (int i = 0; i < CircleCount; i++) {
        m_indicatorColors << createDefaultIndicatorColorList(item->fill());
        totalCircleCount += m_indicatorColors.value(i).count();
    }

    // 为每个 indicator 创建一个 QSGTransformNode
    for (int i = 0; i < totalCircleCount; ++i) {
        QSGTransformNode *transformNode = new QSGTransformNode;
        appendChildNode(transformNode);

        QQuickItemPrivate *d = QQuickItemPrivate::get(item);
        QSGInternalRectangleNode *rectNode = d->sceneGraphContext()->createInternalRectangleNode();
        rectNode->setAntialiasing(true);
        transformNode->appendChildNode(rectNode);
    }
}

void DQuickBusyIndicatorNode::setSpinning(bool spinning)
{
    m_spinning = spinning;
}

void DQuickBusyIndicatorNode::sync(QQuickItem *item)
{
    // 准备绘制 indicator 所需的坐标数据
    const qreal w = item->width();
    const qreal h = item->height();
    m_center = QPointF(w / 2, h / 2);
    auto radius = qMin(w, h) / 2;
    auto indicatorRadius = radius / 2 / 2 * 1.1;
    double degreeCurrent = 0;
    double indicatorShadowOffset = 10;
    auto indicatorDegreeDelta = 360 / m_indicatorColors.count();

    // 先绘制出所有的 indicator, 然后围绕整个图形的中心点旋转
    // 绘制方式参考了 DSpinner::paintEvent() 内容
    // 外层循环表示 indicator 的个数，内层循环表示 indicator 的阴影的个数
    QSGTransformNode *transformNode = static_cast<QSGTransformNode *>(firstChild());
    for (int i = 0; i < CircleCount; i++) {
        auto colors = m_indicatorColors.value(i);
        for (int j = 0; j < colors.count(); j++) {
            Q_ASSERT(transformNode->type() == QSGNode::TransformNodeType);

            QSGInternalRectangleNode *rectNode = static_cast<QSGInternalRectangleNode *>(transformNode->firstChild());
            Q_ASSERT(rectNode->type() == QSGNode::GeometryNodeType);

            degreeCurrent = (i * indicatorDegreeDelta) - (j * indicatorShadowOffset);
            auto x = (radius - indicatorRadius) * qCos(qDegreesToRadians(degreeCurrent));
            auto y = (radius - indicatorRadius) * qSin(qDegreesToRadians(degreeCurrent));

            x = m_center.x() + x;
            y = m_center.y() + y;
            auto tl = QPointF(x - indicatorRadius, y - indicatorRadius);
            QRectF rf(tl.x(), tl.y(), indicatorRadius * 2, indicatorRadius * 2);

            rectNode->setRect(rf);
            rectNode->setRadius(indicatorRadius);
            rectNode->setColor(colors.value(j));
            rectNode->update();

            transformNode = static_cast<QSGTransformNode *>(transformNode->nextSibling());
        }
    }

    maybeUpdate();
}

void DQuickBusyIndicatorNode::maybeRotate()
{
    if (m_spinning) {
        m_rotation += static_cast<float>(360 / m_window->screen()->refreshRate());
        // 先把 z 轴平移到 DQuickBusyIndicator 中心点，然后 DQuickBusyIndicatorNode 整个图形围绕 z 轴旋转
        auto cX = static_cast<float>(m_center.x());
        auto cY = static_cast<float>(m_center.y());
        QMatrix4x4 matrix;
        matrix.translate(cX, cY);
        matrix.rotate(m_rotation, 0, 0, 1);
        matrix.translate(-cX, -cY);
        setMatrix(matrix);

        // If we're inside a QQuickWidget, this call is necessary to ensure the widget gets updated.
        m_window->update();
    }
}

void DQuickBusyIndicatorNode::maybeUpdate()
{
    if (m_spinning)
        m_window->update();
}

DQuickBusyIndicator::DQuickBusyIndicator(QQuickItem *parent)
    : QQuickItem(parent)
    , m_fill(QColor::fromRgb(TransparentColor))
    , m_isRunning(false)
{
    setFlag(ItemHasContents);
}

QColor DQuickBusyIndicator::fill() const
{
    return m_fill;
}

void DQuickBusyIndicator::setFill(const QColor &fill)
{
    if (fill == m_fill)
        return;

    m_fill = fill;
    update();
}

bool DQuickBusyIndicator::isRunning() const
{
    return m_isRunning;
}

void DQuickBusyIndicator::setRunning(bool running)
{
    m_isRunning = running;
    update();
}

void DQuickBusyIndicator::itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &data)
{
    QQuickItem::itemChange(change, data);
    switch (change) {
    case ItemOpacityHasChanged:
        if (qFuzzyIsNull(data.realValue))
            setVisible(false);
        break;
    case ItemVisibleHasChanged:
        update();
        break;
    default:
        break;
    }
}

QSGNode *DQuickBusyIndicator::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    DQuickBusyIndicatorNode *node = static_cast<DQuickBusyIndicatorNode *>(oldNode);
    if (width() > 0 && height() > 0) {
        if (!node) {
            node = new DQuickBusyIndicatorNode(this);
        }
        node->setSpinning(m_isRunning);
        node->sync(this);
    } else {
        delete node;
        node = nullptr;
    }
    return node;
}

DQUICK_END_NAMESPACE
