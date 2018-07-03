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
  void testCaseFileClose0();
  void testCaseFileClose1();
  void testCaseFileChange0();
  void testCaseFileChange1();
  void testCaseFileMove0();
  void testCaseFileMove1();
  void testCaseFileRename0();
  void testCaseFileRename1();
  void testCaseBookAdd0();
  void testCaseBookAdd1();
  void testCaseBookDelete0();
  void testCaseBookDelete1();
  void testCaseBookChange0();
  void testCaseBookChange1();
  void testCaseBookMove0();
  void testCaseBookMove1();
  void testCaseBookRename0();
  void testCaseBookRename1();
  void testCasePageAdd0();
  void testCasePageAdd1();
  void testCasePageDelete0();
  void testCasePageDelete1();
  void testCasePageChange0();
  void testCasePageChange1();
  void testCasePageMove0();
  void testCasePageMove1();
  void testCasePageRename0();
  void testCasePageRename1();
  void testCaseFileOpen1();
  void testCaseFileSave1();
  void testCaseFileOpen2();
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
  dev_in->OpenFile("test_memo1.memo");
  VerifyFileIndex(0);
  VerifyFileLabels(T_strs{"test_memo1.memo"});
  VerifyFileSize(1);
  T_states st(1, false);
  VerifyFileStates(st);
}

void CpuTest::testCaseFileOpen2()
{
  dev_in->OpenFile("test_memo.memo");
  VerifyFileIndex(0);
  VerifyFileLabels(T_strs{"test_memo.memo"});
  VerifyFileSize(1);
  T_states st(1, false);
  VerifyFileStates(st);
  VerifyBookIndex(0);
  VerifyBookLabels(T_strs{"testbook"});
  VerifyBookSize(1);
  VerifyBookStates(st);
  VerifyPageIndex(0);
  VerifyPageLabels(T_strs{"testpage"});
  VerifyPageSize(1);
  VerifyPageStates(st);
}

void CpuTest::testCaseFileSave1()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->AddPage("testpage");
  dev_in->SaveAsFile("test_memo");
}

void CpuTest::testCaseFileClose0()
{
  dev_in->CloseFile(0);
  VerifyFileIndex(-1);
  VerifyFileLabels(T_strs());
  VerifyFileSize(0);
  VerifyFileStates(T_states());
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

void CpuTest::testCaseFileChange0()
{
  dev_in->ChangeFile(10);
  VerifyFileIndex(-1);
  VerifyFileSize(0);
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

void CpuTest::testCaseFileRename0()
{
  dev_in->RenameFile(10, "test10");
  VerifyFileIndex(-1);
  VerifyFileLabels(T_strs());
  VerifyFileSize(0);
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

void CpuTest::testCaseBookDelete0()
{
  dev_in->DeleteBook(0);
  VerifyBookIndex(-1);
  VerifyBookLabels(T_strs());
  VerifyBookSize(0);
  VerifyBookStates(T_states());
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

void CpuTest::testCaseBookChange0()
{
  dev_in->ChangeBook(0);
  VerifyBookIndex(-1);
  VerifyBookSize(0);
  dev_in->NewFile();
  dev_in->AddBook("test1");
  dev_in->ChangeBook(100);
  VerifyBookIndex(0);
  VerifyBookSize(1);
}

void CpuTest::testCaseBookChange1()
{
  dev_in->NewFile();
  dev_in->AddBook("test1");
  dev_in->AddBook("test2");
  VerifyBookIndex(1);
  VerifyBookLabels(T_strs{"test1", "test2"});
  VerifyBookSize(2);
  T_states st(2, true);
  VerifyBookStates(st);
  dev_in->ChangeBook(0);
  VerifyBookIndex(0);
}

void CpuTest::testCaseBookMove0()
{
  dev_in->MoveBook(0, 1);
  VerifyBookIndex(-1);
  VerifyBookSize(0);
  dev_in->NewFile();
  dev_in->AddBook("test1");
  dev_in->MoveBook(100, 10);
  VerifyBookIndex(0);
  VerifyBookSize(1);
}

void CpuTest::testCaseBookMove1()
{
  dev_in->NewFile();
  dev_in->AddBook("test1");
  dev_in->AddBook("test2");
  dev_in->AddBook("test3");
  dev_in->MoveBook(0, 1);
  VerifyBookIndex(1);
  VerifyBookLabels(T_strs{"test2", "test1", "test3"});
  VerifyBookSize(3);
  T_states st(3, true);
  VerifyBookStates(st);
}

void CpuTest::testCaseBookRename0()
{
  dev_in->RenameBook(0, "tested");
  VerifyBookIndex(-1);
  VerifyBookLabels(T_strs());
  VerifyBookSize(0);
  dev_in->NewFile();
  dev_in->AddBook("test1");
  dev_in->RenameBook(100, "tested");
  VerifyBookIndex(0);
  VerifyBookLabels(T_strs{"test1"});
  VerifyBookSize(1);
}

void CpuTest::testCaseBookRename1()
{
  dev_in->NewFile();
  dev_in->AddBook("test1");
  dev_in->RenameBook(0, "tested");
  VerifyBookIndex(0);
  VerifyBookLabels(T_strs{"tested"});
  VerifyBookSize(1);
  T_states st(1, true);
  VerifyBookStates(st);
}

void CpuTest::testCasePageAdd0()
{
  dev_in->AddPage("test1");
  VerifyPageIndex(-1);
  VerifyPageSize(0);
}

void CpuTest::testCasePageAdd1()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->AddPage("test1");
  VerifyPageIndex(0);
  VerifyPageLabels(T_strs{"test1"});
  VerifyPageSize(1);
  T_states st(1, true);
  VerifyPageStates(st);
}

void CpuTest::testCasePageDelete0()
{
  dev_in->DeletePage(0);
  VerifyPageIndex(-1);
  VerifyPageSize(0);
}

void CpuTest::testCasePageDelete1()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->AddPage("test1");
  dev_in->AddPage("test2");
  dev_in->DeletePage(0);
  VerifyPageIndex(-1);
  VerifyPageLabels(T_strs{"test2"});
  VerifyPageSize(1);
  T_states st(1, true);
  VerifyPageStates(st);
}

void CpuTest::testCasePageChange0()
{
  dev_in->ChangePage(0);
  VerifyPageIndex(-1);
  VerifyPageSize(0);
}

void CpuTest::testCasePageChange1()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->AddPage("test1");
  dev_in->AddPage("test2");
  dev_in->ChangePage(0);
  VerifyPageIndex(0);
  VerifyPageLabels(T_strs{"test1", "test2"});
  T_states st(2, true);
  VerifyPageStates(st);
}

void CpuTest::testCasePageMove0()
{
  dev_in->MovePage(10, 100);
  VerifyPageIndex(-1);
  VerifyPageSize(0);
}

void CpuTest::testCasePageMove1()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->AddPage("test1");
  dev_in->AddPage("test2");
  dev_in->AddPage("test3");
  dev_in->MovePage(1, 0);
  VerifyPageIndex(0);
  VerifyPageLabels(T_strs{"test2", "test1", "test3"});
  VerifyPageSize(3);
  T_states st(3, true);
  VerifyPageStates(st);
}

void CpuTest::testCasePageRename0()
{
  dev_in->RenamePage(0, "tested");
  VerifyPageIndex(-1);
  VerifyPageLabels(T_strs());
  VerifyPageSize(0);
}

void CpuTest::testCasePageRename1()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->AddPage("test1");
  dev_in->RenamePage(0, "tested");
  VerifyPageIndex(0);
  VerifyPageLabels(T_strs{"tested"});
  VerifyPageSize(1);
  T_states st(1, true);
  VerifyPageStates(st);
}

QTEST_MAIN(CpuTest)

#include "tst_cputest.moc"
