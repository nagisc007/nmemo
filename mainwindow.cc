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
  // class
  ui(new Ui::MainWindow),
  tabbar(new QTabBar(this)),
  pagelist(new QListWidget(this)),
  editor(new QTextEdit(this)),
  sys(new Core::System()),
  // register
  r_titlebar_updated(true),
  r_statusbar_updated(true),
  r_tabbar_updated(true),
  r_pagelist_updated(true),
  r_editor_updated(true),
  r_filter_selected(Nmemo::VALUE::DEFAULT_SELECTED_FILTER),
  r_statusshow_time(Nmemo::VALUE::STATUS_MESSAGE_TIME),
  r_dirname(QDir::currentPath())
  // utils
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
  if (sys) sys.reset();
  delete ui;
  qDebug() << "MainWindow: destruct";
}

/* methods: base */
auto MainWindow::InitWidgets() -> bool
{
  // props
  tabbar->setTabsClosable(true);
  tabbar->setMovable(true);
  pagelist->setMaximumWidth(Nmemo::PROP::LISTVIEW_MAX_WIDTH);
  editor->setTabStopDistance(Nmemo::PROP::EDIT_TAB_DISTANCE);
  editor->setReadOnly(true);

  // layout
  auto mainLayout = new QVBoxLayout();
  auto subLayout = new QHBoxLayout();
  auto mainWidget = new QWidget(this);
  subLayout->addWidget(editor.data());
  subLayout->addWidget(pagelist.data());
  mainLayout->addWidget(tabbar.data());
  mainLayout->addLayout(subLayout);
  mainWidget->setLayout(mainLayout);
  setCentralWidget(mainWidget);

  return true;
}

auto MainWindow::InitActions() -> bool
{
  connect(tabbar.data(), &QTabBar::currentChanged, this, &MainWindow::OnTabCurrentChanged);
  connect(tabbar.data(), &QTabBar::tabCloseRequested, this, &MainWindow::OnTabCloseRequested);
  connect(tabbar.data(), &QTabBar::tabMoved, this, &MainWindow::OnTabMoved);
  connect(pagelist.data(), &QListWidget::currentRowChanged,
          this, &MainWindow::OnListCurrentRowChanged);
  connect(pagelist.data(), &QListWidget::itemDoubleClicked,
          this, &MainWindow::OnListItemDoubleClicked);
  connect(editor.data(), &QTextEdit::textChanged, this, &MainWindow::OnEditorTextChanged);
  connect(this, &MainWindow::asSystemData, sys.data(), &Core::System::ToSystemData);
  connect(sys.data(), &Core::System::asTabBarData, this, &MainWindow::ToTabBar);
  connect(sys.data(), &Core::System::asPageListData, this, &MainWindow::ToPageList);
  connect(sys.data(), &Core::System::asEditorData, this, &MainWindow::ToEditor);
  connect(sys.data(), &Core::System::asStatusBarData, this, &MainWindow::ToStatusBar);
  connect(sys.data(), &Core::System::asTitleBarData, this, &MainWindow::ToTitleBar);
  connect(sys.data(), &Core::System::asFileNameRequest,
          this, &MainWindow::on_actSaveAs_triggered);
  return true;
}

/* methods: features */
auto MainWindow::CheckUnsaved() -> bool
{
  return false;
}

auto MainWindow::UpdateNote() -> void
{
  if (!r_editor_updated || editor->isReadOnly()) return;

  emit asSystemData(Cmd::NOTE_EDIT, QVariant(UIP::Editor::Text::Fetch(editor.data())),
                    QVariant(0), QVariant(0));
}

/* slots: Devices */
void MainWindow::ToTabBar(const T_cmd cmd, const T_arg arg0, const T_arg arg1,
                          const T_arg arg2)
{
  u_mutex.lock();
  r_tabbar_updated = false;
  UIP::TabBar::ToUpdate(cmd, tabbar.data(), arg0, arg1, arg2);
  u_mutex.unlock();

  r_tabbar_updated = true;
}

void MainWindow::ToPageList(const T_cmd cmd, const T_arg arg0, const T_arg arg1)
{
  u_mutex.lock();
  r_pagelist_updated = false;
  UIP::PageList::ToUpdate(cmd, pagelist.data(), arg0, arg1);
  u_mutex.unlock();

  r_pagelist_updated = true;
}

void MainWindow::ToEditor(const T_cmd cmd, const T_arg arg0, const T_arg arg1)
{
  u_mutex.lock();
  r_editor_updated = false;
  UIP::Editor::ToUpdate(cmd, editor.data(), arg0, arg1);
  u_mutex.unlock();

  r_editor_updated = true;
}

void MainWindow::ToStatusBar(const T_cmd cmd, const T_arg arg0, const T_arg arg1)
{
  u_mutex.lock();
  r_statusbar_updated = false;
  UIP::StatusBar::ToUpdate(cmd, this->statusBar(), arg0, arg1, &r_statusshow_time);
  u_mutex.unlock();

  r_statusbar_updated = true;
}

void MainWindow::ToTitleBar(const T_cmd cmd, const T_arg arg0)
{
  u_mutex.lock();
  r_titlebar_updated = false;
  UIP::TitleBar::ToUpdate(cmd, this, arg0);
  u_mutex.unlock();

  r_titlebar_updated = true;
}

/* slots: TabBar */
void MainWindow::OnTabCurrentChanged(const T_index index)
{
  if (!r_tabbar_updated) return;

  UpdateNote();
  emit asSystemData(Cmd::BOOK_CHANGE, QVariant(index), QVariant(0), QVariant(0));
}

void MainWindow::OnTabCloseRequested(const T_index index)
{
  if (!r_tabbar_updated) return;

  UpdateNote();
  emit asSystemData(Cmd::BOOK_DELETE, QVariant(index), QVariant(0), QVariant(0));
}

void MainWindow::OnTabMoved(const T_index from, const T_index to)
{
  if (!r_tabbar_updated) return;

  emit asSystemData(Cmd::BOOK_MOVE, QVariant(from), QVariant(to), QVariant(0));
}

/* slots: PageList */
void MainWindow::OnListCurrentRowChanged(const T_index index)
{
  if (!r_pagelist_updated) return;

  UpdateNote();
  emit asSystemData(Cmd::PAGE_CHANGE, QVariant(index), QVariant(0), QVariant(0));
}

void MainWindow::OnListItemDoubleClicked(const T_item* item)
{
  if (!r_pagelist_updated) return;

  auto index = pagelist->row(item);
  auto name = Utl::Name::Input(this, Nmemo::VALUE::GET_PAGE_TITLE,
                               Nmemo::VALUE::GET_PAGE_CAPTION, item->text());

  emit asSystemData(Cmd::PAGE_EDIT, QVariant(index), QVariant(name), QVariant(0));
}

/* slots: Editor */
void MainWindow::OnEditorTextChanged()
{
  if (!r_editor_updated || editor->isReadOnly()) return;

  emit asSystemData(Cmd::NOTE_MODIFY, QVariant(0), QVariant(0), QVariant(0));
}

/* slots: menus - File */
void MainWindow::on_actNew_triggered()
{
  if (!r_tabbar_updated) return;

  UpdateNote();
  emit asSystemData(Cmd::BOOK_ADD, QVariant(Nmemo::VALUE::DEFAULT_FILENAME),
                    QVariant(0), QVariant(0));
}

void MainWindow::on_actOpen_triggered()
{
  if (!r_tabbar_updated) return;

  auto path = Utl::Path::Load::Input(this, Nmemo::VALUE::LOAD_FILE_CAPTION, r_dirname,
                                    Nmemo::VALUE::FILE_FILTER, &r_filter_selected);
  if (path.isEmpty()) return;
  r_dirname = QDir::currentPath();
  UpdateNote();
  emit asSystemData(Cmd::FILE_LOAD, QVariant(path), QVariant(0), QVariant(0));
}

void MainWindow::on_actClose_triggered()
{
  if (!r_tabbar_updated) return;

  OnTabCloseRequested(UIP::TabBar::Index::Fetch(tabbar.data()));
}

void MainWindow::on_actSave_triggered()
{
  if (!r_tabbar_updated) return;

  UpdateNote();
  emit asSystemData(Cmd::FILE_SAVE, QVariant(0), QVariant(0), QVariant(0));
}

void MainWindow::on_actSaveAs_triggered()
{
  if (!r_tabbar_updated) return;

  auto path = Utl::Path::Save::Input(this, Nmemo::VALUE::SAVE_FILE_CAPTION,
                                    r_dirname,
                                    Nmemo::VALUE::FILE_FILTER, &r_filter_selected);
  if (path.isEmpty()) return;
  r_dirname = QDir::currentPath();
  UpdateNote();
  emit asSystemData(Cmd::FILE_SAVEAS, QVariant(path), QVariant(0), QVariant(0));
}

void MainWindow::on_actQuit_triggered()
{
  if (!r_tabbar_updated) return;

  bool is_unsaved = false;
  for (int i = 0, size = tabbar->count(); i < size; ++i) {
    is_unsaved |= CheckUnsaved();
  }
  if (!is_unsaved) close();
}

/* slots: menus - Edit */
void MainWindow::on_actUndo_triggered()
{
  if (r_editor_updated) UIP::Editor::Act::Undo(editor.data());
}

void MainWindow::on_actRedo_triggered()
{
  if (r_editor_updated) UIP::Editor::Act::Redo(editor.data());
}

void MainWindow::on_actCut_triggered()
{
  if (r_editor_updated) UIP::Editor::Act::Cut(editor.data());
}

void MainWindow::on_actCopy_triggered()
{
  if (r_editor_updated) UIP::Editor::Act::Copy(editor.data());
}

void MainWindow::on_actPaste_triggered()
{
  if (r_editor_updated) UIP::Editor::Act::Paste(editor.data());
}

void MainWindow::on_actErase_triggered()
{
  if (r_editor_updated) UIP::Editor::Act::Erase(editor.data());
}

void MainWindow::on_actSelectAll_triggered()
{
  if (r_editor_updated) UIP::Editor::Act::SelectAll(editor.data());
}

/* slots: menus - Book */
void MainWindow::on_actAddItem_triggered()
{
  if (!r_pagelist_updated) return;

  auto name = Utl::Name::Input(this, Nmemo::VALUE::GET_PAGE_TITLE,
                               Nmemo::VALUE::GET_PAGE_CAPTION,
                               Nmemo::VALUE::DEFAULT_LISTITEM_NAME);
  if (name.isEmpty()) return;

  UpdateNote();
  emit asSystemData(Cmd::PAGE_ADD, QVariant(name), QVariant(0), QVariant(0));
}

void MainWindow::on_actDeleteItem_triggered()
{
  if (!r_pagelist_updated) return;

  UpdateNote();
  emit asSystemData(Cmd::PAGE_DELETE, QVariant(0), QVariant(0), QVariant(0));
}

void MainWindow::on_actRenameItem_triggered()
{
  if (!r_pagelist_updated) return;

  OnListItemDoubleClicked(UIP::PageList::Item::Fetch(pagelist.data()));
}

void MainWindow::on_actMoveNext_triggered()
{
  if (!r_pagelist_updated) return;

  auto index = UIP::PageList::Index::Fetch(pagelist.data());
  emit asSystemData(Cmd::PAGE_MOVE, QVariant(index), QVariant(index+1), QVariant(0));
}

void MainWindow::on_actMovePrevious_triggered()
{
  if (!r_pagelist_updated) return;

  auto index = UIP::PageList::Index::Fetch(pagelist.data());
  emit asSystemData(Cmd::PAGE_MOVE, QVariant(index), QVariant(index-1), QVariant(0));
}

void MainWindow::on_actSort_AtoZ_triggered()
{
  if (!r_pagelist_updated) return;

  emit asSystemData(Cmd::PAGE_SORT, QVariant(Qt::AscendingOrder), QVariant(0), QVariant(0));
}

void MainWindow::on_actSort_ZtoA_triggered()
{
  if (!r_pagelist_updated) return;

  emit asSystemData(Cmd::PAGE_SORT, QVariant(Qt::DescendingOrder), QVariant(0), QVariant(0));
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
  if (!r_tabbar_updated) return;

  OnTabCurrentChanged(UIP::TabBar::Index::Fetch(tabbar.data()) + 1);
}

void MainWindow::on_actPreviousTab_triggered()
{
  if (!r_tabbar_updated) return;

  OnTabCurrentChanged(UIP::TabBar::Index::Fetch(tabbar.data()) - 1);
}

void MainWindow::on_actNextItem_triggered()
{
  if (!r_pagelist_updated) return;

  OnListCurrentRowChanged(UIP::PageList::Index::Fetch(pagelist.data()) + 1);
}

void MainWindow::on_actPreviousItem_triggered()
{
  if (!r_pagelist_updated) return;

  OnListCurrentRowChanged(UIP::PageList::Index::Fetch(pagelist.data()) - 1);
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

/* process: UI Process */
namespace UIP {

namespace TitleBar {

auto ToUpdate(const T_cmd cmd, MainWindow* m, const T_arg arg) -> bool
{
  bool is_updated = false;
  if (Utl::Cmd::Exists(cmd, Cmd::TITLE)) {
    m->setWindowTitle(arg.toString());
    is_updated = true;
  }
  return is_updated;
}

}  // ns UIP::TitleBar

namespace StatusBar {

auto ToUpdate(const T_cmd cmd, QStatusBar* sbar, const T_arg arg_txt,
              const T_arg arg_time, T_msgtime* time) -> bool
{
  bool is_updated = false;
  if (Utl::Cmd::Exists(cmd, Cmd::STATUS_TIME)) {
    (*time) = arg_time.toInt();
    is_updated = true;
  }
  if (Utl::Cmd::Exists(cmd, Cmd::STATUS_TEXT)) {
    sbar->showMessage(arg_txt.toString(), *time);
    is_updated = true;
  }
  return is_updated;
}

}  // ns UIP::StatusBar

namespace TabBar {

namespace Index {

auto Fetch(const QTabBar* tabbar) -> T_index
{
  return tabbar->currentIndex();
}

}  // ns UIP::TabBar::Index

auto ToUpdate(const T_cmd cmd, QTabBar* tabbar, const T_arg arg0,
              const T_arg arg1, const T_arg arg2) -> bool
{
  if (Utl::Cmd::Exists(cmd, Cmd::TAB_NAME)) {
    Utl::Widget::Names::Merge<QTabBar>(tabbar, arg1.toStringList());
  }
  if (Utl::Cmd::Exists(cmd, Cmd::TAB_INDEX)) {
    tabbar->setCurrentIndex(arg0.toInt());
  }
  if (Utl::Cmd::Exists(cmd, Cmd::TAB_STATUS)) {
    auto stats = arg2.toList();
    for (int i = 0, size = tabbar->count(); i < size; ++i) {
      auto stat = stats.at(i).toBool();
      tabbar->setTabTextColor(i, stat ? Nmemo::VALUE::TAB_MODIFIED_COLOR:
                                        Nmemo::VALUE::TAB_UNMODIFIED_COLOR);
    }
  }
  return true;
}

}  // ns UIP::TabBar

namespace PageList {

namespace Index {

auto Fetch(const QListWidget* list) -> T_index
{
  return list->currentRow();
}

}  // ns UIP::PageList::Index

namespace Item {

constexpr auto Fetch(const QListWidget* list) -> T_item*
{
  return list->currentItem();
}

}  // ns UIP::PageList::Item

auto ToUpdate(const T_cmd cmd, QListWidget* list, const T_arg arg0, const T_arg arg1) -> bool
{
  if (Utl::Cmd::Exists(cmd, Cmd::LIST_NAME)) {
    Utl::Widget::Names::Merge<QListWidget>(list, arg1.toStringList());
  }
  if (Utl::Cmd::Exists(cmd, Cmd::LIST_INDEX)) {
    list->setCurrentRow(arg0.toInt());
  }
  return true;
}

}  // ns UIP::PageList

namespace Editor {

namespace Text {

auto Fetch(const QTextEdit* e) -> T_text
{
  return e->toPlainText();
}

}  // ns UIP::Editor::Text

namespace Act {

auto Undo(QTextEdit* e) -> void
{
  if (!e->isReadOnly()) e->undo();
}

auto Redo(QTextEdit* e) -> void
{
  if (!e->isReadOnly()) e->redo();
}

auto Cut(QTextEdit* e) -> void
{
  if (!e->isReadOnly()) e->cut();
}

auto Copy(QTextEdit* e) -> void
{
  if (!e->isReadOnly()) e->copy();
}

auto Paste(QTextEdit* e) -> void
{
  if (!e->isReadOnly()) e->paste();
}

auto Erase(QTextEdit* e) -> void
{
  if (!e->isReadOnly()) e->textCursor().removeSelectedText();
}

auto SelectAll(QTextEdit* e) -> void
{
  if (!e->isReadOnly()) e->selectAll();
}

}  // ns CP::Editor::Act

auto ToUpdate(const T_cmd cmd, QTextEdit* editor, const T_arg arg0, const T_arg arg1) -> bool
{
  if (Utl::Cmd::Exists(cmd, Cmd::EDITOR_TEXT)) {
    editor->setText(arg0.toString());
  }
  if (Utl::Cmd::Exists(cmd, Cmd::EDITOR_STATE)) {
    editor->setReadOnly(!arg1.toBool());
  }
  return true;
}

}  // ns UIP::Editor

}  // ns UIP
