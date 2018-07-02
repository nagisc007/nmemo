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
#include "mock/input_device.h"

#include <QThread>

class CpuTest : public QObject
{
  Q_OBJECT

public:
  CpuTest();
  // members
  QScopedPointer<CPU::Core> cpu;
  QScopedPointer<DEV::Input> dev_in;
  // test methods
  void VerifyFileSize(int i) {
    QCOMPARE(cpu->ram.file_ids.size(), i);
  }
  void VerifyBookSize(int i) {
    auto fid = CPU::currentFileId(&cpu->ram);
    if (fid >= 0) {
      QCOMPARE(cpu->ram.files.at(CPU::currentFileId(&cpu->ram))->book_ids.size(), i);
    }
  }
  void VerifyPageSize(int i) {
    auto bid = CPU::currentBookId(&cpu->ram);
    if (bid >= 0) {
      QCOMPARE(cpu->ram.books.at(CPU::currentBookId(&cpu->ram))->page_ids.size(), i);
    }
  }
  void VerifyFileIndex(int i) {
    QCOMPARE(CPU::fileIndexOf(&cpu->ram, CPU::currentFileId(&cpu->ram)), i);
  }
  void VerifyBookIndex(int i) {
    QCOMPARE(CPU::bookIndexOf(&cpu->ram, CPU::currentFileId(&cpu->ram),
                              CPU::currentBookId(&cpu->ram)), i);
  }
  void VerifyPageIndex(int i) {
    QCOMPARE(CPU::pageIndexOf(&cpu->ram, CPU::currentBookId(&cpu->ram),
                              CPU::currentPageId(&cpu->ram)), i);
  }
  void VerifyFileLabels(T_strs labels) {
    QCOMPARE(CPU::fileLabelsOf(&cpu->ram), labels);
  }
  void VerifyBookLabels(T_strs labels) {
    QCOMPARE(CPU::bookLabelsOf(&cpu->ram, CPU::currentFileId(&cpu->ram)), labels);
  }
  void VerifyPageLabels(T_strs labels) {
    QCOMPARE(CPU::pageLabelsOf(&cpu->ram, CPU::currentBookId(&cpu->ram)), labels);
  }
  void VerifyFileStates(T_states states) {
    QCOMPARE(CPU::fileStatesOf(&cpu->ram), states);
  }
  void VerifyBookStates(T_states states) {
    QCOMPARE(CPU::bookStatesOf(&cpu->ram, CPU::currentFileId(&cpu->ram)), states);
  }
  void VerifyPageStates(T_states states) {
    QCOMPARE(CPU::pageStatesOf(&cpu->ram, CPU::currentBookId(&cpu->ram)), states);
  }

public slots:
  void FromCpuError(T_cpu_result r) {
    qDebug() << "CPU error: " << static_cast<int>(r);
  }
private Q_SLOTS:
  void init();
  void cleanup();
  void initTestCase();
  void cleanupTestCase();
  void testCaseFileNew1();
  void testCaseFileOpen1();
  void testCaseFileClose1();
  void testCaseFileChange1();
  void testCaseFileMove0();
  void testCaseFileMove1();
  void testCaseFileRename1();
  void testCaseBookAdd0();
  void testCaseBookAdd1();
  void testCaseBookDelete1();
};

CpuTest::CpuTest():
  cpu(new CPU::Core()),
  dev_in(new DEV::Input())
{
  // init connection
  connect(dev_in.data(), &DEV::Input::ToCpu, cpu.data(), &CPU::Core::FromDev);
  connect(dev_in.data(), &DEV::Input::ToCpuIrq, cpu.data(), &CPU::Core::FromIrq);
  connect(cpu.data(), &CPU::Core::ToError, this, &CpuTest::FromCpuError);
}

void CpuTest::initTestCase()
{
}

void CpuTest::cleanupTestCase()
{
}

void CpuTest::init()
{
  dev_in->Reset();
}

void CpuTest::cleanup()
{
}

void CpuTest::testCaseFileNew1()
{
  dev_in->NewFile();
  VerifyFileSize(1);
  VerifyFileIndex(0);
  VerifyFileLabels(T_strs{DEFAULT::FILE_TITLE});
  T_states states(1);
  states[0] = true;
  VerifyFileStates(states);
}

void CpuTest::testCaseFileOpen1()
{
  QSKIP("currently skipped");
}

void CpuTest::testCaseFileClose1()
{
  dev_in->NewFile();
  dev_in->CloseFile(0);
  VerifyFileSize(0);
  VerifyFileIndex(-1);
  VerifyFileLabels(T_strs());
  T_states states(0);
  VerifyFileStates(states);
}

void CpuTest::testCaseFileChange1()
{
  dev_in->NewFile();
  dev_in->NewFile();
  dev_in->ChangeFile(0);
  VerifyFileSize(2);
  VerifyFileIndex(0);
}

void CpuTest::testCaseFileMove0()
{
  dev_in->MoveFile(0, 1);
}

void CpuTest::testCaseFileMove1()
{
  dev_in->NewFile();
  dev_in->NewFile();
  dev_in->NewFile();
  dev_in->MoveFile(2, 1);
  VerifyFileIndex(1);
  VerifyFileSize(3);
  VerifyFileLabels(T_strs{DEFAULT::FILE_TITLE,DEFAULT::FILE_TITLE,DEFAULT::FILE_TITLE});
  T_states states(3);
  states.fill(true);
  VerifyFileStates(states);
}

void CpuTest::testCaseFileRename1()
{
  dev_in->NewFile();
  dev_in->RenameFile(0, "test1");
  VerifyFileIndex(0);
  VerifyFileSize(1);
  VerifyFileLabels(T_strs{"test1"});
}

void CpuTest::testCaseBookAdd0()
{
  dev_in->AddBook("test0");
  VerifyBookIndex(-1);
  VerifyBookLabels(T_strs());
  //VerifyBookSize(0);
  //VerifyBookStates(T_states());
}

void CpuTest::testCaseBookAdd1()
{
  dev_in->NewFile();
  dev_in->AddBook("test1");
  VerifyBookIndex(0);
  VerifyBookLabels(T_strs{"test1"});
  VerifyBookSize(1);
  T_states st(1, true);
  VerifyBookStates(st);
}

void CpuTest::testCaseBookDelete1()
{
  dev_in->NewFile();
  dev_in->AddBook("test1");
  dev_in->AddBook("test2");
  dev_in->DeleteBook(0);
  VerifyBookIndex(0);
  VerifyBookLabels(T_strs{"test2"});
  VerifyBookSize(1);
  T_states st(1, true);
  VerifyBookStates(st);
}

QTEST_MAIN(CpuTest)

#include "tst_cputest.moc"
