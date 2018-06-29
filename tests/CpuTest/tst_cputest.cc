/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include <QString>
#include <QtTest>
#include <QCoreApplication>

class CpuTest : public QObject
{
  Q_OBJECT

public:
  CpuTest();

private Q_SLOTS:
  void initTestCase();
  void cleanupTestCase();
  void testCase1();
};

CpuTest::CpuTest()
{
}

void CpuTest::initTestCase()
{
}

void CpuTest::cleanupTestCase()
{
}

void CpuTest::testCase1()
{
  QVERIFY2(true, "Failure");
}

QTEST_MAIN(CpuTest)

#include "tst_cputest.moc"
