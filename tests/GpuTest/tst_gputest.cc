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
  void FromGpu(T_dev_addr addr, T_ivec iv, T_strs s, T_states st) {
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
      filetab->setCurrentIndex(iv.at(0));
    }
    if (_IsExists(addr, DEV::Addr::FILETAB_STATES)) {
      for (int i = 0; i < st.size(); ++i) {
        filetab->setTabTextColor(i, st.at(i) ? Qt::black: Qt::red);
      }
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
      booktab->setCurrentIndex(iv.at(0));
    }
    if (_IsExists(addr, DEV::Addr::BOOKTAB_STATES)) {
      for (int i = 0; i < st.size(); ++i) {
        booktab->setTabTextColor(i, st.at(i) ? Qt::black: Qt::red);
      }
    }
    if (_IsExists(addr, DEV::Addr::PAGELIST_LABELS)) {
      pagelist->clear();
      pagelist->addItems(s);
    }
    if (_IsExists(addr, DEV::Addr::PAGELIST_INDEX)) {
      pagelist->setCurrentRow(iv.at(0));
    }
    if (_IsExists(addr, DEV::Addr::PAGELIST_STATES)) {
      for (int i = 0; i < st.size(); ++i) {
        pagelist->item(i)->setForeground(QBrush(st.at(i) ? Qt::black: Qt::red));
      }
    }
    if (_IsExists(addr, DEV::Addr::EDITOR_READONLY)) {
      editor->setReadOnly(iv.at(0));
    }
    if (_IsExists(addr, DEV::Addr::EDITOR_TEXT)) {
      editor->setText(s.at(0));
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
    QCOMPARE(filetab->count(), strs.size());
    for (int i = 0; i < strs.size(); ++i) {
      QCOMPARE(filetab->tabText(i), strs.at(i));
    }
  }
  void VerifyFileIndex(T_index i) {
    QCOMPARE(filetab->currentIndex(), i);
  }
  void VerifyFileColors(T_states st) {
    for (int i = 0; i < st.size(); ++i) {
      QCOMPARE(filetab->tabTextColor(i), QColor(st.at(i) ? Qt::black: Qt::red));
    }
  }
  void VerifyBookTabLabels(T_strs strs) {
    QCOMPARE(booktab->count(), strs.size());
    for (int i = 0; i < strs.size(); ++i) {
      QCOMPARE(booktab->tabText(i), strs.at(i));
    }
  }
  void VerifyBookTabIndex(T_index i) {
    QCOMPARE(booktab->currentIndex(), i);
  }
  void VerifyBookColors(T_states st) {
    for (int i = 0; i < st.size(); ++i) {
      QCOMPARE(booktab->tabTextColor(i), QColor(st.at(i) ? Qt::black: Qt::red));
    }
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
  void VerifyPageListColors(T_states st) {
    for (int i = 0; i < st.size(); ++i) {
      QCOMPARE(pagelist->item(i)->foreground(),
               QBrush(st.at(i) ? Qt::black: Qt::red));
    }
  }
  void VerifyEditorReadOnly(bool is_ro) {
    QCOMPARE(editor->isReadOnly(), is_ro);
  }
  void VerifyEditorText(T_str text) {
    QCOMPARE(editor->toPlainText(), text);
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
  void testCaseTextModify1();
  void testCaseFileOpen1();
  void testCaseFileSave1();
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
  connect(cpu.data(), &CPU::Core::ToGpuIrq, gpu.data(), &GPU::Core::FromIrq);
  connect(gpu.data(), &GPU::Core::ToDev, this, &GpuTest::FromGpu);
  connect(cpu.data(), &CPU::Core::ToError, this, &GpuTest::FromCpuError);
  connect(gpu.data(), &GPU::Core::ToError, this, &GpuTest::FromGpuError);
}

void GpuTest::init()
{
  while (filetab->count() > 0)
    filetab->removeTab(0);
  while (booktab->count() > 0)
    booktab->removeTab(0);
  pagelist->clear();
  editor->setText("");
  statusbar->showMessage("");
  window->setTitle("");
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
  T_states st(1, true);
  VerifyFileColors(st);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseFileClose0()
{
  dev_in->CloseFile(0);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs());
  VerifyFileIndex(-1);
  T_states st(0);
  VerifyFileColors(st);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseFileClose1()
{
  dev_in->NewFile();
  dev_in->CloseFile(0);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::FILE_CLOSED);
  VerifyFileTabLabels(T_strs());
  VerifyFileIndex(-1);
  T_states st(0);
  VerifyFileColors(st);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseFileOpen1()
{
  dev_in->OpenFile("test_memo1.memo");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::FILE_OPENED);
  VerifyFileTabLabels(T_strs{"test_memo1.memo"});
  VerifyFileIndex(0);
  T_states fst(1, false);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{DEFAULT::BOOK_TITLE});
  VerifyBookTabIndex(0);
  T_states bst(1, false);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs{"test1"});
  VerifyPageListIndex(0);
  T_states pst(1, false);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseFileSave1()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->AddPage("testpage");
  dev_in->SaveAsFile(0, "test_memo");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::FILE_SAVED);
  VerifyFileTabLabels(T_strs{"test_memo.memo"});
  VerifyFileIndex(0);
  T_states fst(1, false);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  T_states bst(1, false);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs{"testpage"});
  VerifyPageListIndex(0);
  T_states pst(1, false);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseFileChange0()
{
  dev_in->ChangeFile(1);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs());
  VerifyFileIndex(-1);
  T_states fst(0);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseFileChange1()
{
  dev_in->NewFile();
  dev_in->NewFile();
  dev_in->NewFile();
  dev_in->ChangeFile(1);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::FILE_CHANGED);
  VerifyFileTabLabels(T_strs{DEFAULT::FILE_TITLE,DEFAULT::FILE_TITLE,DEFAULT::FILE_TITLE});
  VerifyFileIndex(1);
  T_states fst(3, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseFileMove0()
{
  dev_in->MoveFile(0, 1);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs());
  VerifyFileIndex(-1);
  T_states fst(0);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseFileMove1()
{
  dev_in->NewFile();
  dev_in->NewFile();
  dev_in->NewFile();
  dev_in->MoveFile(0, 1);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::FILE_MOVED);
  VerifyFileTabLabels(T_strs{DEFAULT::FILE_TITLE,DEFAULT::FILE_TITLE,DEFAULT::FILE_TITLE});
  VerifyFileIndex(1);
  T_states fst(3, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseFileRename0()
{
  dev_in->RenameFile(0, "testfile");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs());
  VerifyFileIndex(-1);
  T_states fst(0);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseFileRename1()
{
  dev_in->NewFile();
  dev_in->RenameFile(0, "testfile");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::FILE_RENAMED);
  VerifyFileTabLabels(T_strs{"testfile"});
  VerifyFileIndex(0);
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseBookAdd0()
{
  dev_in->AddBook("test1");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs());
  VerifyFileIndex(-1);
  T_states fst(0);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseBookAdd1()
{
  dev_in->NewFile();
  dev_in->AddBook("test1");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyStatusMessage(MSG::BOOK_ADDED);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"test1"});
  VerifyBookTabIndex(0);
  T_states bst(1, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseBookDelete0()
{
  dev_in->NewFile();
  dev_in->DeleteBook(0);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
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
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"test2"});
  VerifyBookTabIndex(0);
  T_states bst(1, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseBookChange0()
{
  dev_in->NewFile();
  dev_in->ChangeBook(1);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
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
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"test1", "test2", "test3"});
  VerifyBookTabIndex(1);
  T_states bst(3, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseBookMove0()
{
  dev_in->NewFile();
  dev_in->MoveBook(0, 1);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
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
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"test2", "test1", "test3"});
  VerifyBookTabIndex(1);
  T_states bst(3, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseBookRename0()
{
  dev_in->NewFile();
  dev_in->RenameBook(0, "tested");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
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
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"tested"});
  VerifyBookTabIndex(0);
  T_states bst(1, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
}

void GpuTest::testCasePageAdd0()
{
  dev_in->NewFile();
  dev_in->AddPage("test1");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs());
  VerifyBookTabIndex(-1);
  T_states bst(0);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
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
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  T_states bst(1, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs{"test1"});
  VerifyPageListIndex(0);
  T_states pst(1, true);
  VerifyPageListColors(pst);
}

void GpuTest::testCasePageDelete0()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->DeletePage(0);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  T_states bst(1, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
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
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  T_states bst(1, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs{"test2"});
  VerifyPageListIndex(0);
  T_states pst(1, true);
  VerifyPageListColors(pst);
}

void GpuTest::testCasePageChange0()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->ChangePage(1);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  T_states bst(1, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
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
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  T_states bst(1, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs{"test1", "test2", "test3"});
  VerifyPageListIndex(1);
  T_states pst(3, true);
  VerifyPageListColors(pst);
}

void GpuTest::testCasePageMove0()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->MovePage(0, 1);
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  T_states bst(1, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
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
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  T_states bst(1, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs{"test2", "test1", "test3"});
  VerifyPageListIndex(1);
  T_states pst(3, true);
  VerifyPageListColors(pst);
}

void GpuTest::testCasePageRename0()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->RenamePage(0, "tested");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  T_states bst(1, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs());
  VerifyPageListIndex(-1);
  T_states pst(0);
  VerifyPageListColors(pst);
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
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  T_states bst(1, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs{"tested"});
  VerifyPageListIndex(0);
  T_states pst(1, true);
  VerifyPageListColors(pst);
}

void GpuTest::testCaseTextModify1()
{
  dev_in->NewFile();
  dev_in->AddBook("testbook");
  dev_in->AddPage("testpage");
  dev_in->ModifyText("testtext");
  VerifyWindowTitle(DEFAULT::WINDOW_TITLE);
  VerifyFileTabLabels(T_strs{"NewFile"});
  VerifyFileIndex(0);
  T_states fst(1, true);
  VerifyFileColors(fst);
  VerifyBookTabLabels(T_strs{"testbook"});
  VerifyBookTabIndex(0);
  T_states bst(1, true);
  VerifyBookColors(bst);
  VerifyPageListLabels(T_strs{"testpage"});
  VerifyPageListIndex(0);
  T_states pst(1, true);
  VerifyPageListColors(pst);
  VerifyEditorReadOnly(false);
  VerifyEditorText("testtext");
}

QTEST_MAIN(GpuTest)

#include "tst_gputest.moc"
