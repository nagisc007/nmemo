/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

/* class */
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  m_tab_updated(true),
  m_booklist_updated(true),
  m_memo_updated(true),
  m_status_updated(true),
  m_title_updated(true),
  m_selected(Nmemo::VALUE::DEFAULT_SELECTED_FILTER),
  m_dirname(QDir::currentPath()),
  m_time(Nmemo::VALUE::STATUS_MESSAGE_TIME),
  memo(new Nmemo::MemoWidget()),
  core(new Nmemo::Core())
{
  ui->setupUi(this);
  if (!InitWidgets()) {
    qWarning() << "MainWindow: cannot initialize widgets!";
    exit(EXIT_FAILURE);
  }
  if (!InitActions()) {
    qWarning() << "MainWindow: cannot initialize actions!";
    exit(EXIT_FAILURE);
  }
  qDebug() << "MainWindow: construct";
}

MainWindow::~MainWindow()
{
  if (memo) memo.reset();
  if (core) core.reset();
  delete ui;
  qDebug() << "MainWindow: destruct";
}

/* methods: base */
auto MainWindow::InitWidgets() -> bool
{
  setCentralWidget(memo.data());
  return true;
}

auto MainWindow::InitActions() -> bool
{
  connect(memo->tabbar.data(), &QTabBar::currentChanged, this, &MainWindow::OnChangeTab);
  connect(memo->tabbar.data(), &QTabBar::tabCloseRequested, this, &MainWindow::OnDeleteTab);
  connect(memo->tabbar.data(), &QTabBar::tabMoved, this, &MainWindow::OnMoveTab);
  //connect(memo->booklist.data(), &QListWidget::currentRowChanged,
  //        this, &MainWindow::OnChangeBook);
  //connect(memo->booklist.data(), &QListWidget::itemDoubleClicked,
  //        this, &MainWindow::OnRenameBook);
  //connect(memo->editor.data(), &QTextEdit::textChanged, this, &MainWindow::ChangeTextInMemo);
  connect(this, &MainWindow::asTabData, core.data(), &Nmemo::Core::ToTabData);
  //connect(this, &MainWindow::asBookData, core.data(), &Nmemo::Core::ToBookData);
  //connect(this, &MainWindow::asMemoData, core.data(), &Nmemo::Core::ToMemoData);
  //connect(this, &MainWindow::asFileData, core.data(), &Nmemo::Core::ToFileData);
  connect(core.data(), &Nmemo::Core::asTabBarData, this, &MainWindow::ToTabBar);
  //connect(core.data(), &Nmemo::Core::asBookListData, this, &MainWindow::ToBookList);
  //connect(core.data(), &Nmemo::Core::asEditorData, this, &MainWindow::ToEditor);
  //connect(core.data(), &Nmemo::Core::asFileData,
  //        this, &MainWindow::on_actSaveAs_triggered);
  //connect(core.data(), &Nmemo::Core::asStatusData, this, &MainWindow::ToStatusBar);
  //connect(core.data(), &Nmemo::Core::asTitleData, this, &MainWindow::ToTitleBar);
  return true;
}

/* methods: features */

/* slots: output */
void MainWindow::ToTabBar(T_cmd cmd, T_tab_i tab_i, T_tabnames tabnames,
                          T_stats stats)
{
  qDebug() << "catch tab";
  mutex.lock();
  m_tab_updated = false;
  if (Utl::Cmd::Exists(cmd, Cmd::NAMES)) {
    qDebug() << "update names" << tab_i;
    Nmemo::TabBar::Names::Merge(memo.data(), tabnames);
    Nmemo::TabBar::State::Merge(memo.data(), stats);
  }
  if (Utl::Cmd::Exists(cmd, Cmd::INDEX)) {
    Nmemo::TabBar::Index::Merge(memo.data(), tab_i);
  }
  mutex.unlock();

  m_tab_updated = true;
}

void MainWindow::ToBookList(T_cmd cmd, T_book_i book_i, T_booknames booknames)
{
  mutex.lock();
  m_booklist_updated = false;
  if (Utl::Cmd::Exists(cmd, Cmd::NAMES)) {
    Nmemo::BookList::Names::Merge(memo.data(), booknames);
  }
  if (Utl::Cmd::Exists(cmd, Cmd::INDEX)) {
    Nmemo::BookList::Index::Merge(memo.data(), book_i);
  }
  mutex.unlock();

  m_booklist_updated = true;
}

void MainWindow::ToEditor(T_cmd cmd, T_stat stat, const T_text& text)
{
  Q_UNUSED(cmd);

  mutex.lock();
  m_memo_updated = false;
  Nmemo::Editor::State::Merge(memo.data(), stat);
  Nmemo::Editor::Text::Merge(memo.data(), text);
  mutex.unlock();

  m_memo_updated = true;
}

void MainWindow::ToStatusBar(const QString& msg)
{
  mutex.lock();
  m_status_updated = false;
  StatusBar::Message::Merge(this, msg);
  mutex.unlock();

  m_status_updated = true;
}

void MainWindow::ToTitleBar(const T_title& title)
{
  mutex.lock();
  m_title_updated = false;
  TitleBar::Merge(this, title);
  mutex.unlock();

  m_title_updated = true;
}

/* slots: tabs */
void MainWindow::OnAddTab()
{
  if (!m_tab_updated) return;
  emit asTabData(Cmd::TAB_ADD, -1, QVariant(Nmemo::VALUE::DEFAULT_FILENAME));
}

void MainWindow::OnDeleteTab(int index)
{
  if (!m_tab_updated) return;
  emit asTabData(Cmd::TAB_DELETE, index, QVariant(0));
}

void MainWindow::OnChangeTab(int index)
{
  if (!m_tab_updated) return;
  emit asTabData(Cmd::TAB_CHANGE, index, QVariant(0));
}

void MainWindow::OnMoveTab(int from, int to)
{
  if (!m_tab_updated) return;
  emit asTabData(Cmd::TAB_MOVE, from, QVariant(to));
}

/* slots: books */
void MainWindow::OnAddBook()
{
  if (!m_booklist_updated || !Nmemo::TabBar::Exists(memo.data())) return;
  auto name = Utl::Name::Input(this, Nmemo::VALUE::GET_BOOK_TITLE,
                                 Nmemo::VALUE::GET_BOOK_CAPTION,
                                 Nmemo::VALUE::DEFAULT_BOOK_NAME);
  emit asBookData(Cmd::BOOK_ADD, -1,
                  QVariant(name == "" ? Nmemo::VALUE::DEFAULT_BOOK_NAME: name));
}

void MainWindow::OnDeleteBook(int index)
{
  if (!m_booklist_updated || !Nmemo::TabBar::Exists(memo.data())) return;
  emit asBookData(Cmd::BOOK_DELETE, index, QVariant(0));
}

void MainWindow::OnChangeBook(int index)
{
  if (!m_booklist_updated || !Nmemo::TabBar::Exists(memo.data())) return;
  emit asBookData(Cmd::BOOK_CHANGE, index, QVariant(0));
}

void MainWindow::OnMoveBook(int from, int to)
{
  if (!m_booklist_updated || !Nmemo::TabBar::Exists(memo.data())) return;
  emit asBookData(Cmd::BOOK_MOVE, from ,QVariant(0));
}

void MainWindow::OnRenameBook(const QListWidgetItem* item)
{
  if (!m_booklist_updated || !Nmemo::TabBar::Exists(memo.data())) return;
  if (!item) return;
  auto name = Utl::Name::Input(this, Nmemo::VALUE::GET_BOOK_TITLE,
                                 Nmemo::VALUE::GET_BOOK_CAPTION,
                                 item->text());
  emit asBookData(Cmd::BOOK_RENAME, Nmemo::BookList::Index::Fetch(memo.data()),
                  QVariant(name == "" ? item->text(): name));
}

void MainWindow::OnSortBook(T_order order)
{
  if (!m_booklist_updated || !Nmemo::TabBar::Exists(memo.data())) return;
  if (memo->booklist->count() < 2) return;
  emit asBookData(Cmd::BOOK_SORT, Nmemo::BookList::Index::Fetch(memo.data()),
                  QVariant(order));
}

/* slots: memos */

/* slots: menus - File */
void MainWindow::on_actNew_triggered()
{
  OnAddTab();
}

void MainWindow::on_actOpen_triggered()
{
  auto dir = QDir::currentPath();
  auto path = Utl::Path::Load::Input(this, Nmemo::VALUE::LOAD_FILE_CAPTION, m_dirname,
                                    Nmemo::VALUE::FILE_FILTER, &m_selected);
  if (path.isEmpty()) return;
  m_dirname = QDir::currentPath();
  emit asFileData(Cmd::FILE_LOAD, path);
}

void MainWindow::on_actClose_triggered()
{
  OnDeleteTab(Nmemo::TabBar::Index::Fetch(memo.data()));
}

void MainWindow::on_actSave_triggered()
{
  emit asFileData(Cmd::FILE_SAVE, QString(""));
}

void MainWindow::on_actSaveAs_triggered()
{
  auto path = Utl::Path::Save::Input(this, Nmemo::VALUE::SAVE_FILE_CAPTION,
                                    m_dirname,
                                    Nmemo::VALUE::FILE_FILTER, &m_selected);
  if (path.isEmpty()) return;
  emit asFileData(Cmd::FILE_SAVEAS, path);
}

void MainWindow::on_actQuit_triggered()
{
  close();
}

/* slots: menus - Edit */
void MainWindow::on_actUndo_triggered()
{
  if (!m_memo_updated) return;

  Nmemo::Editor::Act::Undo(memo.data());
}

void MainWindow::on_actRedo_triggered()
{
  if (!m_memo_updated) return;

  Nmemo::Editor::Act::Redo(memo.data());
}

void MainWindow::on_actCut_triggered()
{
  if (!m_memo_updated) return;

  Nmemo::Editor::Act::Cut(memo.data());
}

void MainWindow::on_actCopy_triggered()
{
  if (!m_memo_updated) return;

  Nmemo::Editor::Act::Copy(memo.data());
}

void MainWindow::on_actPaste_triggered()
{
  if (!m_memo_updated) return;

  Nmemo::Editor::Act::Paste(memo.data());
}

void MainWindow::on_actErase_triggered()
{
  if (!m_memo_updated) return;

  Nmemo::Editor::Act::Erase(memo.data());
}

void MainWindow::on_actSelectAll_triggered()
{
  if (!m_memo_updated) return;

  Nmemo::Editor::Act::SelectAll(memo.data());
}

/* slots: menus - Book */
void MainWindow::on_actAddBook_triggered()
{
  OnAddBook();
}

void MainWindow::on_actDeleteBook_triggered()
{
  OnDeleteBook(Nmemo::BookList::Index::Fetch(memo.data()));
}

void MainWindow::on_actRenameBook_triggered()
{
  OnRenameBook(Nmemo::BookList::Item::Fetch(memo.data()));
}

void MainWindow::on_actMoveNext_triggered()
{
  auto index = Nmemo::BookList::Index::Fetch(memo.data());
  OnMoveBook(index, index + 1);
}

void MainWindow::on_actMovePrevious_triggered()
{
  auto index = Nmemo::BookList::Index::Fetch(memo.data());
  OnMoveBook(index, index - 1);
}

void MainWindow::on_actSort_AtoZ_triggered()
{
  OnSortBook(Qt::AscendingOrder);
}

void MainWindow::on_actSort_ZtoA_triggered()
{
  OnSortBook(Qt::DescendingOrder);
}

/* slots: menus - View */
void MainWindow::on_actFullscreen_triggered()
{
  if (isFullScreen()) {
    ui->actFullscreen->setChecked(false);
    showNormal();
  } else {
    ui->actFullscreen->setChecked(true);
    showFullScreen();
  }
}

void MainWindow::on_actNextTab_triggered()
{
  OnChangeTab(Nmemo::TabBar::Index::Fetch(memo.data()) + 1);
}

void MainWindow::on_actPreviousTab_triggered()
{
  OnChangeTab(Nmemo::TabBar::Index::Fetch(memo.data()) - 1);
}

void MainWindow::on_actNextBook_triggered()
{
  OnChangeBook(Nmemo::BookList::Index::Fetch(memo.data()) + 1);
}

void MainWindow::on_actPreviousBook_triggered()
{
  OnChangeBook(Nmemo::BookList::Index::Fetch(memo.data()) - 1);
}

/* slots: menus - Help */
void MainWindow::on_actAboutQt_triggered()
{
  QMessageBox::aboutQt(this);
}

void MainWindow::on_actAboutApp_triggered()
{
  auto title = QString("About %1")
      .arg(APP::VALUE::NAME);
  auto msg = QString("<h3>About %1 %2</h3><p>%3<br>Licensed by %4.<br>%5</p><p>%6</p>")
      .arg(APP::VALUE::NAME)
      .arg(APP::VALUE::VERSION)
      .arg(APP::VALUE::DESCRIPTION)
      .arg(APP::VALUE::LICENSE)
      .arg(APP::VALUE::COPYRIGHT)
      .arg(APP::VALUE::AUTHORS);
  QMessageBox::about(this, title, msg);
}

namespace TitleBar {

auto Fetch(const MainWindow* m) -> T_title
{
  return m->windowTitle();
}

auto Merge(MainWindow* m, const T_title& title) -> bool
{
  m->setWindowTitle(title);
  return true;
}

}  // ns TitleBar

namespace StatusBar {
namespace Message {

auto Fetch(const MainWindow* m) -> T_msg
{
  return T_msg(m->statusBar()->currentMessage());
}

auto Merge(MainWindow* m, const T_msg& msg) -> bool
{
  m->statusBar()->showMessage(msg, m->m_time);
  return true;
}

}  // ns StatusBar::Message
namespace Time {

auto Fetch(const MainWindow* m) -> T_msgtime
{
  return m->m_time;
}

auto Merge(MainWindow* m, const T_msgtime time) -> bool
{
  if (m->m_time == time) return false;
  m->m_time = time;
  return true;
}

}  // ns StatusBar::Time
}  // ns StatusBar
