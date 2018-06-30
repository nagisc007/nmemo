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

#include "../../cpu.h"

#include <QThread>

class CpuTest : public QObject
{
  Q_OBJECT

public:
  CpuTest();
  // members
  QScopedPointer<CPU::Core> cpu;

private Q_SLOTS:
  void init();
  void cleanup();
  void initTestCase();
  void cleanupTestCase();
  void testCase1();
};

CpuTest::CpuTest():
  cpu(new CPU::Core())
{
}

void CpuTest::initTestCase()
{
}

void CpuTest::cleanupTestCase()
{
}

void CpuTest::init()
{

}

void CpuTest::cleanup()
{

}

void CpuTest::testCase1()
{
  QVERIFY2(true, "Failure");
}

QTEST_MAIN(CpuTest)

#include "tst_cputest.moc"
