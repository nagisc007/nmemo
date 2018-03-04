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

/* values */
const int APP::Version::MAJOR = 1;
const int APP::Version::MINOR = 1;
const int APP::Version::MICRO = 0;

const QString APP::Values::AUTHORS = "N.T.Works";
const QString APP::Values::NAME = "Nmemo";
const QString APP::Values::VERSION = QString("%1.%2.%3")
    .arg(QString::number(APP::Version::MAJOR))
    .arg(QString::number(APP::Version::MINOR))
    .arg(QString::number(APP::Version::MICRO));
const QString APP::Values::DESCRIPTION = "Nmemo is a simple memo editor by Qt.";
const QString APP::Values::LICENSE = "GNU GENERAL PUBLIC LICENSE Version 3";
const QString APP::Values::COPYRIGHT = "Copyright (c) 2018";
const int MainWindow::Values::STATUS_MESSAGE_TIME = 3000;
const QString MainWindow::Values::DEFAULT_BOOK_NAME = "New Book";
const QString MainWindow::Values::GET_BOOK_TITLE = "Book Name";
const QString MainWindow::Values::GET_BOOK_CAPTION = "Input a book name: ";
const QString MainWindow::Values::LOAD_FILE_CAPTION = "Load file";
const QString MainWindow::Values::SAVE_FILE_CAPTION = "Save file";
const QString MainWindow::Values::FILE_FILTER = "Memo file (*.memo);;All Files (*)";
const QString MainWindow::Values::DEFAULT_SELECTED_FILTER = "Memo file (*.memo)";

/* class */
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  is_booklist_updating_(false),
  is_editor_updating_(false),
  is_tab_updating_(false),
  filename_(""),
  selected_(Values::DEFAULT_SELECTED_FILTER),
  booklist_(nullptr),
  editor_(nullptr),
  tab_(nullptr),
  core_(new Nmemo::Core())
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
  if (booklist_) {
    booklist_->clear();
    booklist_.reset();
  }
  if (editor_) editor_.reset();
  if (tab_) tab_.reset();
  if (core_) core_.reset();
  delete ui;
  qDebug() << "MainWindow: destruct";
}

/* methods: base */
auto MainWindow::InitWidgets() -> bool
{
  // layouts
  auto mainWidget = new QWidget(this);
  auto mainLayout = new QVBoxLayout();
  auto subLayout = new QHBoxLayout();
  // widgets
  tab_.reset(new QTabBar(this));
  booklist_.reset(new QListWidget(this));
  editor_.reset(new QTextEdit(this));
  // set props
  tab_->setTabsClosable(true);
  tab_->setMovable(true);
  editor_->setReadOnly(true);
  editor_->setText("Welcome to Nmemo pad!");
  // set layout
  mainLayout->addWidget(tab_.data());
  subLayout->addWidget(editor_.data());
  subLayout->addWidget(booklist_.data());
  setCentralWidget(mainWidget);
  mainWidget->setLayout(mainLayout);
  mainLayout->addLayout(subLayout);
  return true;
}

auto MainWindow::InitActions() -> bool
{
  connect(tab_.data(), &QTabBar::currentChanged, this, &MainWindow::ChangeTab);
  connect(tab_.data(), &QTabBar::tabBarClicked, this, &MainWindow::ChangeTab);
  connect(tab_.data(), &QTabBar::tabCloseRequested, this, &MainWindow::DeleteTab);
  connect(tab_.data(), &QTabBar::tabMoved, this, &MainWindow::MoveTab);
  connect(booklist_.data(), &QListWidget::currentRowChanged, this, &MainWindow::ChangeBook);
  connect(booklist_.data(), &QListWidget::itemDoubleClicked, this, &MainWindow::DoubleClickBook);
  connect(this, &MainWindow::updated, core_.data(), &Nmemo::Core::Update);
  connect(this, &MainWindow::loaded, core_.data(), &Nmemo::Core::LoadData);
  connect(this, &MainWindow::saved, core_.data(), &Nmemo::Core::SaveData);
  connect(core_.data(), &Nmemo::Core::tabOutputted, this, &MainWindow::outputToTab);
  connect(core_.data(), &Nmemo::Core::booksOutputted, this, &MainWindow::outputToBookList);
  connect(core_.data(), &Nmemo::Core::memoOutputted, this, &MainWindow::outputToEditor);
  connect(core_.data(), &Nmemo::Core::filenameToSaveRequested,
          this, &MainWindow::on_actSaveAs_triggered);
  connect(core_.data(), &Nmemo::Core::statusUpdated, this, &MainWindow::updateStatus);
  connect(core_.data(), &Nmemo::Core::fileUpdated, this, &MainWindow::updateFile);
  return true;
}

/* methods: features */

/* slots: output */
void MainWindow::outputToTab(T_tab_i index, T_tabnames slist)
{
  mutex_.lock();
  is_tab_updating_ = true;
  Utl::TabBarToMerge()(tab_.data(), slist);
  tab_->setCurrentIndex(index);
  mutex_.unlock();

  is_tab_updating_ = false;
}

void MainWindow::outputToBookList(T_book_i index, T_booknames slist)
{
  mutex_.lock();
  is_booklist_updating_ = true;
  Utl::ListWidgetToMerge()(booklist_.data(), slist);
  booklist_->setCurrentRow(index);
  mutex_.unlock();

  is_booklist_updating_ = false;
}

void MainWindow::outputToEditor(T_stat stat, const T_text& text)
{
  mutex_.lock();
  is_editor_updating_ = true;
  editor_->setReadOnly(!stat);
  editor_->setText(text);
  mutex_.unlock();

  is_editor_updating_ = false;
}

void MainWindow::updateStatus(const QString& msg)
{
  statusBar()->showMessage(msg, Values::STATUS_MESSAGE_TIME);
}

void MainWindow::updateFile(const T_fname& fname)
{
  filename_ = fname;
  setWindowTitle(QString("%1Nmemo[%2]").arg("")
                 .arg(Utl::baseNameFetched()(fname)));
}

/* slots: tabs */
void MainWindow::AddTab()
{
  if (is_tab_updating_) return;
  updated(CmdSig::TAB_ADD, -1, editor_->toPlainText(),
          QVariant(Nmemo::Core::Values::DEFAULT_FILENAME));
}

void MainWindow::DeleteTab(int index)
{
  if (is_tab_updating_) return;
  updated(CmdSig::TAB_DELETE, index, editor_->toPlainText(), QVariant(0));
}

void MainWindow::ChangeTab(int index)
{
  if (is_tab_updating_) return;
  updated(CmdSig::TAB_CHANGE, index, editor_->toPlainText(), QVariant(0));
}

void MainWindow::MoveTab(int from, int to)
{
  if (is_tab_updating_) return;
  updated(CmdSig::TAB_MOVE, from, editor_->toPlainText(), QVariant(to));
}

void MainWindow::RenameTab()
{
  if (is_tab_updating_) return;
}

/* slots: books */
void MainWindow::AddBook()
{
  if (is_booklist_updating_) return;
  if (tab_->count() > 0) {
    auto name = Utl::NameToGet()(this, Values::GET_BOOK_TITLE,
                                 Values::GET_BOOK_CAPTION,
                                 Values::DEFAULT_BOOK_NAME);
    updated(CmdSig::BOOK_ADD, -1, editor_->toPlainText(),
            QVariant(name == "" ? Values::DEFAULT_BOOK_NAME: name));
  }
}

void MainWindow::DeleteBook(int index)
{
  if (is_booklist_updating_) return;
  updated(CmdSig::BOOK_DELETE, index, editor_->toPlainText(), QVariant(0));
}

void MainWindow::ChangeBook(int index)
{
  if (is_booklist_updating_) return;
  updated(CmdSig::BOOK_CHANGE, index, editor_->toPlainText(), QVariant(0));
}

void MainWindow::MoveBook(int, int)
{
  if (is_booklist_updating_) return;
}

void MainWindow::RenameBook()
{
  auto item = booklist_->currentItem();
  if (!item) return;
  DoubleClickBook(item);
}

void MainWindow::DoubleClickBook(QListWidgetItem* item)
{
  if (is_booklist_updating_) return;
  if (tab_->count() > 0 && booklist_->count() > 0) {
    auto name = Utl::NameToGet()(this, Values::GET_BOOK_TITLE,
                                 Values::GET_BOOK_CAPTION,
                                 item->text());
    updated(CmdSig::BOOK_RENAME, booklist_->currentRow(), editor_->toPlainText(),
            QVariant(name == "" ? item->text(): name));
  }
}

/* slots: menus - File */
void MainWindow::on_actNew_triggered()
{
  AddTab();
}

void MainWindow::on_actOpen_triggered()
{
  auto dir = QDir::currentPath();
  if (!filename_.isEmpty()) dir = QFileInfo(filename_).absoluteFilePath();
  auto fname = Utl::LoadNameToGet()(this, Values::LOAD_FILE_CAPTION, dir,
                                    Values::FILE_FILTER, &selected_);
  if (fname.isEmpty()) return;
  emit loaded(fname, tab_->currentIndex(), editor_->toPlainText());
}

void MainWindow::on_actClose_triggered()
{
  DeleteTab(tab_->currentIndex());
}

void MainWindow::on_actSave_triggered()
{
  emit saved(QString(""), tab_->currentIndex(), editor_->toPlainText());
}

void MainWindow::on_actSaveAs_triggered()
{
  auto fname = Utl::SaveNameToGet()(this, Values::SAVE_FILE_CAPTION,
                                    filename_,
                                    Values::FILE_FILTER, &selected_);
  if (fname.isEmpty()) return;
  emit saved(fname, tab_->currentIndex(), editor_->toPlainText());
}

void MainWindow::on_actQuit_triggered()
{
  close();
}

/* slots: menus - Edit */
void MainWindow::on_actUndo_triggered()
{
  if (is_editor_updating_ || editor_->isReadOnly()) return;

  editor_->undo();
}

void MainWindow::on_actRedo_triggered()
{
  if (is_editor_updating_ || editor_->isReadOnly()) return;

  editor_->redo();
}

void MainWindow::on_actCut_triggered()
{
  if (is_editor_updating_ || editor_->isReadOnly()) return;

  editor_->cut();
}

void MainWindow::on_actCopy_triggered()
{
  if (is_editor_updating_ || editor_->isReadOnly()) return;

  editor_->copy();
}

void MainWindow::on_actPaste_triggered()
{
  if (is_editor_updating_ || editor_->isReadOnly()) return;

  editor_->paste();
}

void MainWindow::on_actErase_triggered()
{
  if (is_editor_updating_ || editor_->isReadOnly()) return;

  editor_->textCursor().removeSelectedText();
}

void MainWindow::on_actSelectAll_triggered()
{
  if (is_editor_updating_ || editor_->isReadOnly()) return;

  editor_->selectAll();
}

/* slots: menus - Book */
void MainWindow::on_actAddBook_triggered()
{
  AddBook();
}

void MainWindow::on_actDeleteBook_triggered()
{
  DeleteBook(booklist_->currentRow());
}

void MainWindow::on_actRenameBook_triggered()
{
  RenameBook();
}

void MainWindow::on_actMoveNext_triggered()
{
  MoveBook(booklist_->currentRow(), booklist_->currentRow() + 1);
}

void MainWindow::on_actMovePrevious_triggered()
{
  MoveBook(booklist_->currentRow(), booklist_->currentRow() - 1);
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
  ChangeTab(tab_->currentIndex() + 1);
}

void MainWindow::on_actPreviousTab_triggered()
{
  ChangeTab(tab_->currentIndex() - 1);
}

void MainWindow::on_actNextBook_triggered()
{
  ChangeBook(booklist_->currentRow() + 1);
}

void MainWindow::on_actPreviousBook_triggered()
{
  ChangeBook(booklist_->currentRow() - 1);
}

/* slots: menus - Help */
void MainWindow::on_actAboutQt_triggered()
{
  QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::on_actAboutApp_triggered()
{
  QMessageBox::about(this, "About Nmemo", "Nmemo is a simple memo pad.");
}
