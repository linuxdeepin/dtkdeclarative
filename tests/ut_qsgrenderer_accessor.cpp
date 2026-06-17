// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <cstring>

#include <QTest>
#include <private/qsgrenderer_p.h>

#include "../src/util/dprivateaccessor_p.h"

D_DECLARE_PRIVATE_BITFIELD(Test_QSGRendererBFTag,
                           QSGRenderer, m_nodes_dont_preprocess,
                           QSet<QSGNode *>, uint);

static constexpr unsigned kChangedEmittedBit = 0;
static constexpr unsigned kIsRenderingBit = 1;

static_assert(sizeof(QSGRenderer) == 432,
              "QSGRenderer layout changed — update the accessor test");

class ut_QSGRendererAccessor : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void init()
    {
        std::memset(buffer, 0, sizeof(buffer));
        renderer = reinterpret_cast<QSGRenderer *>(buffer);
    }

    void setIsRenderingVisibleViaDirectAccess()
    {
        D_PRIVATE_BF_SET(*renderer, Test_QSGRendererBFTag, kIsRenderingBit, true);

        QCOMPARE(renderer->m_is_rendering, 1U);
        QCOMPARE(renderer->m_changed_emitted, 0U);
    }

    void setChangedEmittedVisibleViaDirectAccess()
    {
        D_PRIVATE_BF_SET(*renderer, Test_QSGRendererBFTag, kChangedEmittedBit, true);

        QCOMPARE(renderer->m_changed_emitted, 1U);
        QCOMPARE(renderer->m_is_rendering, 0U);
    }

    void readIsRenderingViaAccessor()
    {
        renderer->m_is_rendering = true;

        QVERIFY(D_PRIVATE_BF_GET(*renderer, Test_QSGRendererBFTag, kIsRenderingBit));
        QVERIFY(!D_PRIVATE_BF_GET(*renderer, Test_QSGRendererBFTag, kChangedEmittedBit));
    }

    void readChangedEmittedViaAccessor()
    {
        renderer->m_changed_emitted = true;

        QVERIFY(D_PRIVATE_BF_GET(*renderer, Test_QSGRendererBFTag, kChangedEmittedBit));
        QVERIFY(!D_PRIVATE_BF_GET(*renderer, Test_QSGRendererBFTag, kIsRenderingBit));
    }

    void clearIsRenderingViaAccessor()
    {
        renderer->m_is_rendering = true;
        renderer->m_changed_emitted = true;

        D_PRIVATE_BF_SET(*renderer, Test_QSGRendererBFTag, kIsRenderingBit, false);

        QCOMPARE(renderer->m_is_rendering, 0U);
        QCOMPARE(renderer->m_changed_emitted, 1U);
    }

private:
    alignas(QSGRenderer) unsigned char buffer[sizeof(QSGRenderer)];
    QSGRenderer *renderer = nullptr;
};

QTEST_MAIN(ut_QSGRendererAccessor)

#include "ut_qsgrenderer_accessor.moc"
