#pragma once

#include <QMap>
#include <QString>
#include <QStringList>

namespace quadm {

// What differs between a document and the file it was loaded from.
struct FieldChanges {
    QMap<QString, QString> upserts; // added or changed fields (never an empty value)
    QStringList removals;           // fields that existed in the file and were deleted

    bool isEmpty() const { return upserts.isEmpty() && removals.isEmpty(); }
};

// The metadata of ONE file as the application sees it: a dictionary of "Group:Tag" -> text, a list
// of tags (labels) and the file's type. It remembers what it was loaded with, so it can tell
// exactly which fields and tags were edited. It knows nothing about ExifTool, TagLib or Qt widgets.
class MetadataDocument
{
public:
    MetadataDocument() = default;
    MetadataDocument(QString path, QString fileType, QMap<QString, QString> fields,
                     QStringList tags);

    const QString &path() const { return m_path; }
    const QString &fileType() const { return m_fileType; }

    // --- Fields ---
    const QMap<QString, QString> &fields() const { return m_fields; }
    bool contains(const QString &key) const { return m_fields.contains(key); }
    QString value(const QString &key) const { return m_fields.value(key); }

    // Sets a value. An empty value removes the field. Returns true if the document changed.
    bool setField(const QString &key, const QString &value);
    bool removeField(const QString &key);
    // True if this field differs from what was loaded (edited, added or removed).
    bool isFieldModified(const QString &key) const;
    // Puts one field back to its loaded value. Returns true if it was modified.
    bool revertField(const QString &key);
    FieldChanges changes() const;

    // --- Tags ---
    const QStringList &tags() const { return m_tags; }
    // Normalizes the tag first. Returns false if it is invalid or already present.
    bool addTag(const QString &tag);
    // Compared after normalizing. Returns false if the tag was not there.
    bool removeTag(const QString &tag);
    bool hasTag(const QString &tag) const;
    // Replaces the whole list (normalized, de-duplicated). Returns true if the set of tags changed.
    bool setTags(const QStringList &tags);
    // Order does not matter: ["a","b"] equals ["b","a"].
    bool tagsChanged() const;

    // --- Whole document ---
    bool hasChanges() const { return !changes().isEmpty() || tagsChanged(); }
    // Call after the changes were written to the file successfully.
    void markSaved();
    // Throws away every edit.
    void revert();

private:
    QString m_path;
    QString m_fileType;
    QMap<QString, QString> m_fields;
    QMap<QString, QString> m_original;
    QStringList m_tags;
    QStringList m_originalTags;
};

} // namespace quadm
