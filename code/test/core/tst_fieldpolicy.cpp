#include "core/fieldpolicy.h"

#include <QtTest>

using namespace quadm;

class FieldPolicyTest : public QObject
{
    Q_OBJECT
private slots:
    void ignoredByGroup()
    {
        const auto &p = FieldPolicy::standard();
        QVERIFY(p.isIgnored("System:FileSize"));
        QVERIFY(p.isIgnored("ExifTool:ExifToolVersion"));
        QVERIFY(p.isIgnored("MakerNotes:Anything"));
    }

    void ignoredByKey()
    {
        const auto &p = FieldPolicy::standard();
        QVERIFY(p.isIgnored("SourceFile"));
        QVERIFY(p.isIgnored("File:FileAccessDate"));
        QVERIFY(!p.isIgnored("File:FileName"));
    }

    void colorProfileIsKept()
    {
        QVERIFY(!FieldPolicy::standard().isIgnored("ICC_Profile:ProfileDescription"));
    }

    void readOnlyKeysAreShownButNotEditable()
    {
        const auto &p = FieldPolicy::standard();
        QVERIFY(!p.isIgnored("PNG:ImageWidth"));
        QVERIFY(p.isReadOnly("PNG:ImageWidth"));
        QVERIFY(!p.isEditable("PNG:ImageWidth"));
    }

    void unknownKeysAreEditable()
    {
        QVERIFY(FieldPolicy::standard().isEditable("XMP:Title"));
        QVERIFY(FieldPolicy::standard().isEditable("ID3:Artist"));
    }

    void ignoredKeysAreNotEditable()
    {
        QVERIFY(!FieldPolicy::standard().isEditable("System:FileSize"));
    }

    void customPolicy()
    {
        FieldPolicy p({"A"}, {"B:x"}, {"C:y"});
        QVERIFY(p.isIgnored("A:z"));
        QVERIFY(p.isIgnored("B:x"));
        QVERIFY(p.isReadOnly("C:y"));
        QVERIFY(p.isEditable("D:w"));
    }

    void groupAndName()
    {
        QCOMPARE(FieldPolicy::groupOf("PNG:ImageWidth"), QString("PNG"));
        QCOMPARE(FieldPolicy::nameOf("PNG:ImageWidth"), QString("ImageWidth"));
        QCOMPARE(FieldPolicy::groupOf("SourceFile"), QString());
        QCOMPARE(FieldPolicy::nameOf("SourceFile"), QString("SourceFile"));
    }
};

QTEST_APPLESS_MAIN(FieldPolicyTest)
#include "tst_fieldpolicy.moc"
