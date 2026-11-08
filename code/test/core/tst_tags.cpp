#include "core/tags.h"

#include <QtTest>

using namespace quadm;

class TagsTest : public QObject
{
    Q_OBJECT
private slots:
    void normalize_data()
    {
        QTest::addColumn<QString>("input");
        QTest::addColumn<QString>("expected");
        QTest::newRow("plain") << "forest" << "forest";
        QTest::newRow("upper") << "Forest Zone 1" << "forest_zone_1";
        QTest::newRow("trim") << "  night shot  " << "night_shot";
        QTest::newRow("spaces collapse") << "a    b\t c" << "a_b_c";
        QTest::newRow("underscores collapse") << "a___b" << "a_b";
        QTest::newRow("punctuation dropped") << "o'brien!" << "obrien";
        QTest::newRow("keeps dash") << "take-2" << "take-2";
        QTest::newRow("edge dashes") << "--take-2--" << "take-2";
        QTest::newRow("edge underscores") << "_x_" << "x";
        QTest::newRow("unicode letters kept") << "Café Noir" << "café_noir";
        QTest::newRow("only junk") << "!!!" << "";
        QTest::newRow("empty") << "" << "";
    }
    void normalize()
    {
        QFETCH(QString, input);
        QFETCH(QString, expected);
        QCOMPARE(tags::normalize(input), expected);
    }

    void normalizeAllDropsInvalidAndDuplicates()
    {
        const QStringList in{"Rain", "rain", "  ", "Night Shot", "!!!", "night_shot", "b-roll"};
        QCOMPARE(tags::normalizeAll(in), (QStringList{"rain", "night_shot", "b-roll"}));
    }

    void joinAndSplitRoundTrip()
    {
        const QStringList list{"a", "b_c", "d-e"};
        QCOMPARE(tags::split(tags::join(list)), list);
    }

    void splitIgnoresEmptyPartsAndCleansEntries()
    {
        QCOMPARE(tags::split(";A;;B Two;"), (QStringList{"a", "b_two"}));
        QVERIFY(tags::split("").isEmpty());
    }
};

QTEST_APPLESS_MAIN(TagsTest)
#include "tst_tags.moc"
