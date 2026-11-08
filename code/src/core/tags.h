#pragma once

#include <QString>
#include <QStringList>

// Tags are the short labels users attach to files ("forest_zone_1") so they can find them again.
namespace quadm::tags {

// Cleans one tag so every backend stores the same spelling:
//   trims, lower-cases, turns runs of whitespace into '_', drops everything except letters,
//   digits, '_' and '-', collapses repeated '_' and strips leading/trailing '_' and '-'.
//   "  Forest Zone 1! " -> "forest_zone_1".   Returns an empty string if nothing is left.
QString normalize(const QString &tag);

// Normalizes every entry, drops invalid ones and duplicates, keeps the first-seen order.
QStringList normalizeAll(const QStringList &tags);

// ID3 (MP3/WAV) has no list field, so tags are stored in one text frame separated by ';'.
QString join(const QStringList &tags);
QStringList split(const QString &stored);

// Where the tags live inside a file.
//   ExifTool formats (PNG, JPG, MP4, ...): the XMP keyword list (dc:Subject)
//   MP3 and WAV: one custom ID3v2 frame called QuadM_Tags
inline const QString kXmpReadKey = QStringLiteral("XMP:Subject");      // name when reading with -G
inline const QString kXmpWriteTag = QStringLiteral("XMP-dc:Subject");  // name when writing
inline const QString kId3FrameName = QStringLiteral("QuadM_Tags");
inline const QString kId3ReadKey = QStringLiteral("ID3:QuadM_Tags");

} // namespace quadm::tags
