#include "core/version.h"

#include <QRegularExpression>
#include <QtTest>

class VersionTest : public QObject
{
    Q_OBJECT
private slots:
    void looksLikeSemanticVersion()
    {
        const QRegularExpression semver(QStringLiteral(R"(^\d+\.\d+\.\d+$)"));
        QVERIFY2(semver.match(quadm::versionString()).hasMatch(),
                 qPrintable(quadm::versionString()));
    }
};

QTEST_APPLESS_MAIN(VersionTest)
#include "tst_version.moc"
