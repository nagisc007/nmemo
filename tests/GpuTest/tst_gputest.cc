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
#include "../../gpu.h"
#include "../CpuTest/mock/input_device.h"

#include <QTabBar>
#include <QStatusBar>
#include <QWindow>
#include <QListWidget>
#include <QTextEdit>

namespace {

bool _IsExists(T_dev_addr addr, T_dev_addr target)
{
  return (static_cast<int>(addr) & static_cast<int>(target)) != 0;
}

}  // inner global
class GpuTest : public QObject
{
  Q_OBJECT

public:
  GpuTest();
  // members
  QScopedPointer<CPU::Core> cpu;
  QScopedPointer<GPU::Core> gpu;
  QScopedPointer<DEV::Input> dev_in;
  QScopedPointer<QTabBar> filetab;
  QScopedPointer<QTabBar> booktab;
  QScopedPointer<QListWidget> pagelist;
  QScopedPointer<QTextEdit> editor;
  QScopedPointer<QStatusBar> statusbar;
  QScopedPointer<QWindow> window;
public slots:
  void FromGpu(T_dev_addr addr, T_ivec i, T_strs s, T_states st) {
    if (_IsExists(addr, DEV::Addr::WINDOW_TITLE)) {
      window->setTitle(s.at(0));
    }
    if (_IsExists(addr, DEV::Addr::STATUS_MESSAGE)) {
      statusbar->showMessage(s.at(0));
    }
    if (_IsExists(addr, DEV::Addr::FILETAB_LABELS)) {
      while (filetab->count() > 0) {
        filetab->removeTab(0);
      }
      for (int i = 0; i < s.size(); ++i) {
        filetab->addTab(s.at(i));
      }
    }
    if (_IsExists(addr, DEV::Addr::FILETAB_INDEX)) {
      filetab->setCurrentIndex(i.at(0));
    }
    if (_IsExists(addr, DEV::Addr::BOOKTAB_LABELS)) {
      while (booktab->count() > 0) {
        booktab->removeTab(0);
      }
      for (int i = 0; i < s.size(); ++i) {
        booktab->addTab(s.at(i));
      }
    }
    if (_IsExists(addr, DEV::Addr::BOOKTAB_INDEX)) {
      booktab->setCurrentIndex(i.at(0));
    }
    if (_IsExists(addr, DEV::Addr::PAGELIST_LABELS)) {
      pagelist->clear();
      pagelist->addItems(s);
    }
    if (_IsExists(addr, DEV::Addr::PAGELIST_INDEX)) {
      pagelist->setCurrentRow(i.at(0));
    }
  }
  void FromCpuError(T_cpu_result r) {
    qDebug() << "Cpu Error: " << static_cast<int>(r);
  }
  void FromGpuError(T_gpu_result r) {
    qDebug() << "Gpu Error: " << static_cast<int>(r);
  }
  void VerifyWindowTitle(T_str title) {
    QCOMPARE(window->title(), title);
  }
  void VerifyStatusMessage(T_str msg) {
    QCOMPARE(statusbar->currentMessage(), msg);
  }
  void VerifyFileTabLabels(T_strs strs) {
    for (int i = 0; i < strs.size(); ++i) {
      QCOMPARE(filetab->tabText(i), strs.at(i));
    }
  }
  void VerifyFileIndex(T_index i) {
    QCOMPARE(filetab->currentIndex(), i);
  }
  void VerifyBookTabLabels(T_strs strs) {
    for (int i = 0; i < strs.size(); ++i) {
      QCOMPARE(booktab->tabText(i), strs.at(i));
    }
  }
  void VerifyBookTabIndex(T_index i) {
    QCOMPARE(booktab->currentIndex(), i);
  }
  void VerifyPageListLabels(T_strs strs) {
    QCOMPARE(pagelist->count(), strs.size());
    for (int i = 0; i < strs.size(); ++i) {
      QCOMPARE(pagelist->item(i)->text(), strs.at(i));
    }
  }
  void VerifyPageListIndex(T_index i) {
    QCOMPARE(pagelist->currentRow(), i);
  }
private Q_SLOTS:
  void init();
  void cleanup();
  void initTestCase();
  void cleanupTestCase();
  void testCaseFileNew1();
  void testCaseBookAdd1();
  void testCaseBookDelete1();
  void testCaseBookChange1();
  void testCaseBookMove1();
  void testCaseBookRename1();
  void testCasePageAdd1();
  void testCasePageDelete1();
  void testCasePageChange1();
  void testCasePageMove1();
  void testCasePageRename1();
};

GpuTest::GpuTest():
  cpu(new CPU::Core()),
  gpu(new GPU::Core()),
  dev_in(new DEV::Input()),
  filetab(new QTabBar()),
  booktab(new QTabBar()),
  pagelist(new QListWidget()),
  editor(new QTextEdit()),
  statusbar(new QStatusBar()),
  window(new QWindow())
{
  // init connections
  connect(dev_in.data(), &DEV::Input::ToCpu, cpu.data(), &CPU::Core::FromDev);
  connect(dev_in.data(), &DEV::Input::ToCpuIrq, cpu.data(), &CPU::Core::FromIrq);
  connect(cpu.data(), &CPU::Core::ToGpu, gpu.data(), &GPU::Core::FromCpu);
  connect(dev_in.data(), &DEV::Input::ToGpuIrq, gpu.data(), &GPU::Core::FromIrq);
  connect(gpu.data(), &GPU::Core::ToDev, this, &GpuTest::FromGpu);
  connect(cpu.data(), &CPU::Core::ToError, this, &GpuTest::FromCpuError);
  connect(gpu.data(), &GPU::Core::ToError, this, &GpuTest::FromGpuError);
}

void GpuTest::init()
{
  dev_in->Reset();
}

void GpuTest::cleanup()
{

}

void GpuTest::initTestCase()
{
}

void GpuTest::cleanupTestCase()
{
}

void GpuTest::testCaseFileNew1()
{
  dev_in->NewFile();
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::FILE_CREATED);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
}

void GpuTest::testCaseBookAdd1()
{
  dev_in->NewFile();
  dev_in->AddBook("test1");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::BOOK_ADDED);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  VerifyBookTabLabels(T_strs{"test1"});
  VerifyBookTabIndex(0);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
}

void GpuTest::testCaseBookDelete1()
{
  dev_in->NewFile();
  dev_in->AddBook("test1");
  dev_in->AddBook("test2");
  dev_in->DeleteBook(0);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::BOOK_DELETED);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  VerifyBookTabLabels(T_strs{"test2"});
  VerifyBookTabIndex(0);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
}

void GpuTest::testCaseBookChange1()
{
  dev_in->NewFile();
  dev_in->AddBook("test1");
  dev_in->AddBook("test2");
  dev_in->AddBook("test3");
  dev_in->ChangeBook(1);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::BOOK_CHANGED);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  VerifyBookTabLabels(T_strs{"test1", "test2", "test3"});
  VerifyBookTabIndex(1);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
}

void GpuTest::testCaseBookMove1()
{
  dev_in->NewFile();
  dev_in->AddBook("test1");
  dev_in->AddBook("test2");
  dev_in->AddBook("test3");
  dev_in->MoveBook(0, 1);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::BOOK_MOVED);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  VerifyBookTabLabels(T_strs{"test2", "test1", "test3"});
  VerifyBookTabIndex(1);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
}

void GpuTest::testCaseBookRename1()
{
  dev_in->NewFile();
  dev_in->AddBook("test1");
  dev_in->RenameBook(0, "tested");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::BOOK_RENAMED);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  VerifyBookTabLabels(T_strs{"tested"});
  VerifyBookTabIndex(0);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
}

void GpuTest::testCasePageAdd1()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->AddPage("test1");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::PAGE_ADDED);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  VerifyPageListLabels(T_strs{"test1"});
  VerifyPageListIndex(0);
}

void GpuTest::testCasePageDelete1()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->AddPage("test1");
  dev_in->AddPage("test2");
  dev_in->DeletePage(0);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::PAGE_DELETED);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  VerifyPageListLabels(T_strs{"test2"});
  VerifyPageListIndex(0);
}

void GpuTest::testCasePageChange1()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->AddPage("test1");
  dev_in->AddPage("test2");
  dev_in->AddPage("test3");
  dev_in->ChangePage(1);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::PAGE_CHANGED);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  VerifyPageListLabels(T_strs{"test1", "test2", "test3"});
  VerifyPageListIndex(1);
}

void GpuTest::testCasePageMove1()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->AddPage("test1");
  dev_in->AddPage("test2");
  dev_in->AddPage("test3");
  dev_in->MovePage(0, 1);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::PAGE_MOVED);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  VerifyPageListLabels(T_strs{"test2", "test1", "test3"});
  VerifyPageListIndex(1);
}

void GpuTest::testCasePageRename1()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->AddPage("test1");
  dev_in->RenamePage(0, "tested");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::PAGE_RENAMED);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  VerifyPageListLabels(T_strs{"tested"});
  VerifyPageListIndex(0);
}

QTEST_MAIN(GpuTest)

#include "tst_gputest.moc"
