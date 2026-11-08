#pragma once

#include <QSet>
#include <QString>

namespace quadm {

// Decides which metadata keys ("Group:Tag", ExifTool style) are shown and which may be edited.
// It is a plain value type so tests (and future settings) can build their own policy.
class FieldPolicy
{
public:
    FieldPolicy(QSet<QString> ignoredGroups, QSet<QString> ignoredKeys, QSet<QString> readOnlyKeys);

    // The policy the application ships with.
    static const FieldPolicy &standard();

    // Never shown: ExifTool internals, system noise, thumbnails, values that change on every read.
    bool isIgnored(const QString &key) const;
    // Shown but not editable: header values and computed values ExifTool cannot write.
    bool isReadOnly(const QString &key) const;
    // Editable = shown and not read-only.
    bool isEditable(const QString &key) const { return !isIgnored(key) && !isReadOnly(key); }

    // "PNG:ImageWidth" -> "PNG"; a key without a group returns an empty string.
    static QString groupOf(const QString &key);
    // "PNG:ImageWidth" -> "ImageWidth"
    static QString nameOf(const QString &key);

private:
    QSet<QString> m_ignoredGroups;
    QSet<QString> m_ignoredKeys;
    QSet<QString> m_readOnlyKeys;
};

} // namespace quadm
