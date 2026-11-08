#include "core/tags.h"

namespace quadm::tags {

QString normalize(const QString &tag)
{
    QString out;
    out.reserve(tag.size());
    bool pendingSeparator = false;
    for (const QChar ch : tag.trimmed().toLower()) {
        if (ch.isSpace() || ch == QLatin1Char('_')) {
            pendingSeparator = !out.isEmpty();
        } else if (ch.isLetterOrNumber() || ch == QLatin1Char('-')) {
            if (pendingSeparator)
                out += QLatin1Char('_');
            pendingSeparator = false;
            out += ch;
        }
        // anything else is dropped without splitting the word: "o'brien" -> "obrien"
    }
    while (out.startsWith(QLatin1Char('-')))
        out.remove(0, 1);
    while (out.endsWith(QLatin1Char('-')))
        out.chop(1);
    return out;
}

QStringList normalizeAll(const QStringList &tags)
{
    QStringList result;
    for (const QString &tag : tags) {
        const QString clean = normalize(tag);
        if (!clean.isEmpty() && !result.contains(clean))
            result << clean;
    }
    return result;
}

QString join(const QStringList &tags)
{
    return tags.join(QLatin1Char(';'));
}

QStringList split(const QString &stored)
{
    return normalizeAll(stored.split(QLatin1Char(';'), Qt::SkipEmptyParts));
}

} // namespace quadm::tags
