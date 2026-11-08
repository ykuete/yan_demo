#include "core/result.h"

#include <QtTest>

using namespace quadm;

class ResultTest : public QObject
{
    Q_OBJECT
private slots:
    void statusDefaultsToSuccess()
    {
        Status s;
        QVERIFY(s.isOk());
        QVERIFY(s);
        QVERIFY(Status::success().isOk());
    }

    void statusFailureCarriesCodeAndMessage()
    {
        const Status s = Status::failure(ErrorCode::ReadOnly, "locked");
        QVERIFY(!s);
        QCOMPARE(s.error().code, ErrorCode::ReadOnly);
        QCOMPARE(s.message(), QString("locked"));
    }

    void resultHoldsAValue()
    {
        Result<int> r = 42;
        QVERIFY(r.isOk());
        QCOMPARE(r.value(), 42);
        QCOMPARE(r.valueOr(7), 42);
        QVERIFY(r.status().isOk());
    }

    void resultHoldsAnError()
    {
        Result<QString> r = Result<QString>::failure(ErrorCode::NotFound, "gone");
        QVERIFY(!r.isOk());
        QCOMPARE(r.error().code, ErrorCode::NotFound);
        QCOMPARE(r.valueOr("fallback"), QString("fallback"));
        const Status s = r.status();
        QVERIFY(!s);
        QCOMPARE(s.message(), QString("gone"));
    }

    void takeValueMovesOut()
    {
        Result<QString> r = QString("hello");
        QCOMPARE(r.takeValue(), QString("hello"));
    }
};

QTEST_APPLESS_MAIN(ResultTest)
#include "tst_result.moc"
