#include "core/metadatadocument.h"

#include <QtTest>

using namespace quadm;

class MetadataDocumentTest : public QObject
{
    Q_OBJECT

    static MetadataDocument sample()
    {
        return MetadataDocument("/x/a.png", "PNG",
                                {{"XMP:Title", "Old"}, {"EXIF:Make", "Canon"}}, {"rain", "night"});
    }

private slots:
    void freshDocumentHasNoChanges()
    {
        const auto doc = sample();
        QVERIFY(!doc.hasChanges());
        QVERIFY(doc.changes().isEmpty());
        QCOMPARE(doc.path(), QString("/x/a.png"));
        QCOMPARE(doc.fileType(), QString("PNG"));
        QCOMPARE(doc.value("XMP:Title"), QString("Old"));
    }

    void editingAFieldIsReported()
    {
        auto doc = sample();
        QVERIFY(doc.setField("XMP:Title", "New"));
        QVERIFY(doc.isFieldModified("XMP:Title"));
        QVERIFY(!doc.isFieldModified("EXIF:Make"));
        const auto ch = doc.changes();
        QCOMPARE(ch.upserts.value("XMP:Title"), QString("New"));
        QVERIFY(ch.removals.isEmpty());
        QVERIFY(doc.hasChanges());
    }

    void settingTheSameValueIsNotAChange()
    {
        auto doc = sample();
        QVERIFY(!doc.setField("XMP:Title", "Old"));
        QVERIFY(!doc.hasChanges());
    }

    void addingAFieldIsReported()
    {
        auto doc = sample();
        QVERIFY(doc.setField("XMP:Creator", "Yan"));
        QCOMPARE(doc.changes().upserts.value("XMP:Creator"), QString("Yan"));
        QVERIFY(doc.isFieldModified("XMP:Creator"));
    }

    void emptyValueRemovesTheField()
    {
        auto doc = sample();
        QVERIFY(doc.setField("EXIF:Make", ""));
        QVERIFY(!doc.contains("EXIF:Make"));
        QCOMPARE(doc.changes().removals, QStringList{"EXIF:Make"});
        QVERIFY(doc.isFieldModified("EXIF:Make"));
    }

    void addingThenRemovingANewFieldLeavesNoChange()
    {
        auto doc = sample();
        doc.setField("XMP:Creator", "Yan");
        doc.removeField("XMP:Creator");
        QVERIFY(!doc.hasChanges());
    }

    void revertFieldRestoresTheLoadedValue()
    {
        auto doc = sample();
        doc.setField("XMP:Title", "New");
        QVERIFY(doc.revertField("XMP:Title"));
        QCOMPARE(doc.value("XMP:Title"), QString("Old"));
        doc.removeField("EXIF:Make");
        QVERIFY(doc.revertField("EXIF:Make"));
        QCOMPARE(doc.value("EXIF:Make"), QString("Canon"));
        QVERIFY(!doc.revertField("EXIF:Make"));
    }

    void tagsAreNormalizedOnAdd()
    {
        auto doc = sample();
        QVERIFY(doc.addTag("Forest Zone 1"));
        QVERIFY(doc.hasTag("forest_zone_1"));
        QVERIFY(doc.hasTag("FOREST ZONE 1"));
        QVERIFY(!doc.addTag("forest zone 1")); // duplicate
        QVERIFY(!doc.addTag("!!!"));           // invalid
        QVERIFY(doc.tagsChanged());
        QVERIFY(doc.hasChanges());
    }

    void removingATag()
    {
        auto doc = sample();
        QVERIFY(doc.removeTag("Rain"));
        QVERIFY(!doc.removeTag("rain"));
        QCOMPARE(doc.tags(), QStringList{"night"});
        QVERIFY(doc.tagsChanged());
    }

    void tagOrderDoesNotMatter()
    {
        auto doc = sample();
        QVERIFY(!doc.setTags({"night", "rain"}));
        QVERIFY(!doc.tagsChanged());
        QVERIFY(doc.setTags({"night"}));
        QVERIFY(doc.tagsChanged());
    }

    void addingAndRemovingSameTagIsNoChange()
    {
        auto doc = sample();
        doc.addTag("new");
        doc.removeTag("new");
        QVERIFY(!doc.tagsChanged());
    }

    void markSavedForgetsEdits()
    {
        auto doc = sample();
        doc.setField("XMP:Title", "New");
        doc.addTag("x");
        doc.markSaved();
        QVERIFY(!doc.hasChanges());
        QCOMPARE(doc.value("XMP:Title"), QString("New"));
        QVERIFY(doc.hasTag("x"));
    }

    void revertDiscardsEdits()
    {
        auto doc = sample();
        doc.setField("XMP:Title", "New");
        doc.removeField("EXIF:Make");
        doc.addTag("x");
        doc.revert();
        QVERIFY(!doc.hasChanges());
        QCOMPARE(doc.value("XMP:Title"), QString("Old"));
        QVERIFY(doc.contains("EXIF:Make"));
        QVERIFY(!doc.hasTag("x"));
    }

    void loadedTagsAreNormalized()
    {
        MetadataDocument doc("/x", "MP3", {}, {"Rain Day", "rain_day", "!!"});
        QCOMPARE(doc.tags(), QStringList{"rain_day"});
        QVERIFY(!doc.hasChanges());
    }
};

QTEST_APPLESS_MAIN(MetadataDocumentTest)
#include "tst_metadatadocument.moc"
