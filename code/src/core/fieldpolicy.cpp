#include "core/fieldpolicy.h"

namespace quadm {

FieldPolicy::FieldPolicy(QSet<QString> ignoredGroups, QSet<QString> ignoredKeys,
                         QSet<QString> readOnlyKeys)
    : m_ignoredGroups(std::move(ignoredGroups)),
      m_ignoredKeys(std::move(ignoredKeys)),
      m_readOnlyKeys(std::move(readOnlyKeys))
{
}

const FieldPolicy &FieldPolicy::standard()
{
    static const FieldPolicy policy(
        // Whole groups. ICC_Profile is deliberately not listed: designers need the color profile.
        {"ExifTool", "System", "MakerNotes"},
        // Single keys.
        {"SourceFile", "File:Directory", "File:FilePermissions", "File:FileAccessDate",
         "File:FileInodeChangeDate", "File:FileTypeExtension", "File:ID3Size", "EXIF:ThumbnailImage",
         "EXIF:PrintIM"},
        // Shown, not editable. Gamma, sRGB and pixel density ARE writable, so they are not here.
        {"PNG:ImageWidth", "PNG:ImageHeight", "PNG:BitDepth", "PNG:ColorType", "PNG:Compression",
         "PNG:Filter", "PNG:Interlace", "File:FileName", "File:FileSize", "File:FileType",
         "File:MIMEType", "File:FileModifyDate", "File:FileCreateDate", "Composite:ImageSize",
         "Composite:Megapixels"});
    return policy;
}

bool FieldPolicy::isIgnored(const QString &key) const
{
    return m_ignoredKeys.contains(key) || m_ignoredGroups.contains(groupOf(key));
}

bool FieldPolicy::isReadOnly(const QString &key) const
{
    return m_readOnlyKeys.contains(key);
}

QString FieldPolicy::groupOf(const QString &key)
{
    const int colon = key.indexOf(QLatin1Char(':'));
    return colon < 0 ? QString() : key.left(colon);
}

QString FieldPolicy::nameOf(const QString &key)
{
    const int colon = key.indexOf(QLatin1Char(':'));
    return colon < 0 ? key : key.mid(colon + 1);
}

} // namespace quadm
