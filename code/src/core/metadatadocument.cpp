#include "core/metadatadocument.h"

#include "core/tags.h"

#include <QSet>

namespace quadm {

namespace {
QSet<QString> asSet(const QStringList &list)
{
    return QSet<QString>(list.begin(), list.end());
}
} // namespace

MetadataDocument::MetadataDocument(QString path, QString fileType, QMap<QString, QString> fields,
                                   QStringList tagList)
    : m_path(std::move(path)),
      m_fileType(std::move(fileType)),
      m_fields(std::move(fields)),
      m_original(m_fields),
      m_tags(tags::normalizeAll(tagList)),
      m_originalTags(m_tags)
{
}

bool MetadataDocument::setField(const QString &key, const QString &value)
{
    if (value.isEmpty())
        return removeField(key);
    auto it = m_fields.find(key);
    if (it != m_fields.end() && it.value() == value)
        return false;
    m_fields.insert(key, value);
    return true;
}

bool MetadataDocument::removeField(const QString &key)
{
    return m_fields.remove(key) > 0;
}

bool MetadataDocument::isFieldModified(const QString &key) const
{
    const bool now = m_fields.contains(key);
    const bool before = m_original.contains(key);
    if (now != before)
        return true;
    return now && m_fields.value(key) != m_original.value(key);
}

bool MetadataDocument::revertField(const QString &key)
{
    if (!isFieldModified(key))
        return false;
    if (m_original.contains(key))
        m_fields.insert(key, m_original.value(key));
    else
        m_fields.remove(key);
    return true;
}

FieldChanges MetadataDocument::changes() const
{
    FieldChanges result;
    for (auto it = m_fields.begin(); it != m_fields.end(); ++it) {
        if (!m_original.contains(it.key()) || m_original.value(it.key()) != it.value())
            result.upserts.insert(it.key(), it.value());
    }
    for (auto it = m_original.begin(); it != m_original.end(); ++it) {
        if (!m_fields.contains(it.key()))
            result.removals << it.key();
    }
    return result;
}

bool MetadataDocument::addTag(const QString &tag)
{
    const QString clean = tags::normalize(tag);
    if (clean.isEmpty() || m_tags.contains(clean))
        return false;
    m_tags << clean;
    return true;
}

bool MetadataDocument::removeTag(const QString &tag)
{
    return m_tags.removeAll(tags::normalize(tag)) > 0;
}

bool MetadataDocument::hasTag(const QString &tag) const
{
    return m_tags.contains(tags::normalize(tag));
}

bool MetadataDocument::setTags(const QStringList &newTags)
{
    const QStringList clean = tags::normalizeAll(newTags);
    const bool changed = asSet(clean) != asSet(m_tags);
    m_tags = clean;
    return changed;
}

bool MetadataDocument::tagsChanged() const
{
    return asSet(m_tags) != asSet(m_originalTags);
}

void MetadataDocument::markSaved()
{
    m_original = m_fields;
    m_originalTags = m_tags;
}

void MetadataDocument::revert()
{
    m_fields = m_original;
    m_tags = m_originalTags;
}

} // namespace quadm
