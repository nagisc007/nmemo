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
  r_ui_updated(true),
  r_filter_selected(Nmemo::VALUE::DEFAULT_SELECTED_FILTER),
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
  return ToInitWidgetsProperties() &&
      ToInitWidgetsLayouts();
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

auto MainWindow::ToInitWidgetsLayouts() -> bool
{
  // layout
  auto mainLayout = new QVBoxLayout();
  auto subLayout = new QHBoxLayout();
  auto mainWidget = new QWidget();
  subLayout->addWidget(editor.data());
  subLayout->addWidget(pagelist.data());
  mainLayout->addWidget(tabbar.data());
  mainLayout->addLayout(subLayout);
  mainWidget->setLayout(mainLayout);
  setCentralWidget(mainWidget);

  return true;
}

auto MainWindow::ToInitWidgetsProperties() -> bool
{
  // props
  tabbar->setTabsClosable(true);
  tabbar->setMovable(true);
  pagelist->setMaximumWidth(Nmemo::PROP::LISTVIEW_MAX_WIDTH);
  editor->setTabStopDistance(Nmemo::PROP::EDIT_TAB_DISTANCE);
  editor->setReadOnly(true);

  return true;
}

/* methods: features */
auto MainWindow::ExistsUnsavedAll() -> bool
{
  for (int i = 0, size = tabbar->count(); i < size; ++i) {
    if (isUnsaved(i)) {
      return true;
    }
  }
  return false;
}

auto MainWindow::UpdateNote() -> void
{
  if (!r_ui_updated || editor->isReadOnly()) return;

  emit asSystemData(OpCode::NOTE_CACHE, QVariant(UIP::Editor::textFetch(editor.data())),
                    QVariant(0), QVariant(0));
}

auto MainWindow::isDeletedBook() -> bool
{
  auto result = QMessageBox::question(this, "Delete book",
                                      "The book deleted?",
                                      QMessageBox::Ok, QMessageBox::No);
  return (result == QMessageBox::Ok);
}

auto MainWindow::isDeletedPage() -> bool
{
  auto result = QMessageBox::question(this, "Delete page",
                                      "The page deleted?",
                                      QMessageBox::Ok, QMessageBox::No);
  return (result == QMessageBox::Ok);
}

/* slots: Devices */
void MainWindow::ToTabBar(const T_sig sig, const T_arg arg0, const T_arg arg1,
                          const T_arg arg2)
{
  u_mutex.lock();
  r_ui_updated = false;
  UIP::TabBar::ToUpdate(sig, tabbar.data(), arg0, arg1, arg2);
  u_mutex.unlock();

  r_ui_updated = true;
}

void MainWindow::ToPageList(const T_sig sig, const T_arg arg0, const T_arg arg1)
{
  u_mutex.lock();
  r_ui_updated = false;
  UIP::PageList::ToUpdate(sig, pagelist.data(), arg0, arg1);
  u_mutex.unlock();

  r_ui_updated = true;
}

void MainWindow::ToEditor(const T_sig sig, const T_arg arg0, const T_arg arg1,
                          const T_arg arg2, const T_arg arg3)
{
  u_mutex.lock();
  r_ui_updated = false;
  UIP::Editor::ToUpdate(sig, editor.data(), arg0, arg1, arg2, arg3);
  u_mutex.unlock();

  r_ui_updated = true;
}

void MainWindow::ToStatusBar(const T_sig sig, const T_arg arg0, const T_arg arg1)
{
  u_mutex.lock();
  r_ui_updated = false;
  UIP::StatusBar::ToUpdate(sig, this->statusBar(), arg0, arg1);
  u_mutex.unlock();

  r_ui_updated = true;
}

void MainWindow::ToTitleBar(const T_sig sig, const T_arg arg0)
{
  u_mutex.lock();
  r_ui_updated = false;
  UIP::TitleBar::ToUpdate(sig, this, arg0);
  u_mutex.unlock();

  r_ui_updated = true;
}

/* slots: TabBar */
void MainWindow::OnTabCurrentChanged(const T_index index)
{
  if (!isUiUpdated()) return;

  UpdateNote();
  emit asSystemData(OpCode::BOOK_CHANGE, QVariant(index), QVariant(0), QVariant(0));
}

void MainWindow::OnTabCloseRequested(const T_index index)
{
  if (!isUiUpdated()) return;

  if (isUnsaved(index)) {
    if (!isDeletedBook()) {
      on_actSave_triggered();
      return;
    }
  }

  UpdateNote();
  emit asSystemData(OpCode::BOOK_DELETE, QVariant(index), QVariant(0), QVariant(0));
}

void MainWindow::OnTabMoved(const T_index from, const T_index to)
{
  if (!isUiUpdated()) return;

  emit asSystemData(OpCode::BOOK_MOVE, QVariant(from), QVariant(to), QVariant(0));
}

/* slots: PageList */
void MainWindow::OnListCurrentRowChanged(const T_index index)
{
  if (!isUiUpdated()) return;

  UpdateNote();
  emit asSystemData(OpCode::PAGE_CHANGE, QVariant(index), QVariant(0), QVariant(0));
}

void MainWindow::OnListItemDoubleClicked(const T_item* item)
{
  if (!isUiUpdated()) return;

  auto index = pagelist->row(item);
  auto name = Utl::Name::Input(this, Nmemo::VALUE::GET_PAGE_TITLE,
                               Nmemo::VALUE::GET_PAGE_CAPTION, item->text());

  emit asSystemData(OpCode::PAGE_RENAME, QVariant(index), QVariant(name), QVariant(0));
}

/* slots: Editor */
void MainWindow::OnEditorTextChanged()
{
  if (!isUiUpdated() || editor->isReadOnly()) return;

  emit asSystemData(OpCode::NOTE_MODIFY, QVariant(0), QVariant(0), QVariant(0));
}

/* slots: menus - File */
void MainWindow::on_actNew_triggered()
{
  if (!isUiUpdated()) return;

  UpdateNote();
  emit asSystemData(OpCode::BOOK_ADD, QVariant(Nmemo::VALUE::DEFAULT_FILENAME),
                    QVariant(0), QVariant(0));
}

void MainWindow::on_actOpen_triggered()
{
  if (!isUiUpdated()) return;

  auto path = Utl::Path::Load::Input(this, Nmemo::VALUE::LOAD_FILE_CAPTION, r_dirname,
                                    Nmemo::VALUE::FILE_FILTER, &r_filter_selected);
  if (path.isEmpty()) return;
  r_dirname = QDir(path).absolutePath();
  UpdateNote();
  emit asSystemData(OpCode::FILE_LOAD, QVariant(path), QVariant(0), QVariant(0));
}

void MainWindow::on_actClose_triggered()
{
  if (!isUiUpdated()) return;

  OnTabCloseRequested(UIP::TabBar::indexFetch(tabbar.data()));
}

void MainWindow::on_actSave_triggered()
{
  if (!isUiUpdated()) return;

  UpdateNote();
  emit asSystemData(OpCode::FILE_SAVE, QVariant(0), QVariant(0), QVariant(0));
}

void MainWindow::on_actSaveAs_triggered()
{
  if (!isUiUpdated()) return;

  auto path = Utl::Path::Save::Input(this, Nmemo::VALUE::SAVE_FILE_CAPTION,
                                    r_dirname,
                                    Nmemo::VALUE::FILE_FILTER, &r_filter_selected);
  if (path.isEmpty()) return;
  r_dirname = QDir(path).absolutePath();
  UpdateNote();
  emit asSystemData(OpCode::FILE_SAVEAS, QVariant(path), QVariant(0), QVariant(0));
}

void MainWindow::on_actQuit_triggered()
{
  if (!isUiUpdated()) return;

  if (ExistsUnsavedAll()) {
    auto result = QMessageBox::question(this, "Exists unsaved file",
                                        "Shutdown app to ignore unsaved files?",
                                        QMessageBox::Ok, QMessageBox::No);
    if (result != QMessageBox::Ok) {
      return;
    }
  }

  close();
}

/* slots: menus - Edit */
void MainWindow::on_actUndo_triggered()
{
  if (isUiUpdated()) UIP::Editor::Act::Undo(editor.data());
}

void MainWindow::on_actRedo_triggered()
{
  if (isUiUpdated()) UIP::Editor::Act::Redo(editor.data());
}

void MainWindow::on_actCut_triggered()
{
  if (isUiUpdated()) UIP::Editor::Act::Cut(editor.data());
}

void MainWindow::on_actCopy_triggered()
{
  if (isUiUpdated()) UIP::Editor::Act::Copy(editor.data());
}

void MainWindow::on_actPaste_triggered()
{
  if (isUiUpdated()) UIP::Editor::Act::Paste(editor.data());
}

void MainWindow::on_actErase_triggered()
{
  if (isUiUpdated()) UIP::Editor::Act::Erase(editor.data());
}

void MainWindow::on_actSelectAll_triggered()
{
  if (isUiUpdated()) UIP::Editor::Act::SelectAll(editor.data());
}

/* slots: menus - Book */
void MainWindow::on_actAddItem_triggered()
{
  if (!isUiUpdated()) return;

  auto name = Utl::Name::Input(this, Nmemo::VALUE::GET_PAGE_TITLE,
                               Nmemo::VALUE::GET_PAGE_CAPTION,
                               Nmemo::VALUE::DEFAULT_LISTITEM_NAME);
  if (name.isEmpty()) return;

  UpdateNote();
  emit asSystemData(OpCode::PAGE_ADD, QVariant(name), QVariant(0), QVariant(0));
}

void MainWindow::on_actDeleteItem_triggered()
{
  if (!isUiUpdated()) return;

  if (!isDeletedPage()) return;

  auto index = UIP::PageList::indexFetch(pagelist.data());
  UpdateNote();
  emit asSystemData(OpCode::PAGE_DELETE, QVariant(index), QVariant(0), QVariant(0));
}

void MainWindow::on_actRenameItem_triggered()
{
  if (!isUiUpdated()) return;

  OnListItemDoubleClicked(UIP::PageList::itemFetch(pagelist.data()));
}

void MainWindow::on_actMoveNext_triggered()
{
  if (!isUiUpdated()) return;

  auto index = UIP::PageList::indexFetch(pagelist.data());
  emit asSystemData(OpCode::PAGE_MOVE, QVariant(index), QVariant(index+1), QVariant(0));
}

void MainWindow::on_actMovePrevious_triggered()
{
  if (!isUiUpdated()) return;

  auto index = UIP::PageList::indexFetch(pagelist.data());
  emit asSystemData(OpCode::PAGE_MOVE, QVariant(index), QVariant(index-1), QVariant(0));
}

void MainWindow::on_actSort_AtoZ_triggered()
{
  if (!isUiUpdated()) return;

  emit asSystemData(OpCode::PAGE_SORT, QVariant(Qt::AscendingOrder), QVariant(0), QVariant(0));
}

void MainWindow::on_actSort_ZtoA_triggered()
{
  if (!isUiUpdated()) return;

  emit asSystemData(OpCode::PAGE_SORT, QVariant(Qt::DescendingOrder), QVariant(0), QVariant(0));
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

void MainWindow::on_actEditPlainText_triggered()
{
  if (!isUiUpdated()) return;

  emit asSystemData(OpCode::NOTE_CHANGE_MODE, QVariant(static_cast<int>(EditMode::PLAIN)),
                    QVariant(0), QVariant(0));
}

void MainWindow::on_actEditRichText_triggered()
{
  if (!isUiUpdated()) return;

  emit asSystemData(OpCode::NOTE_CHANGE_MODE, QVariant(static_cast<int>(EditMode::HTML)),
                    QVariant(0), QVariant(0));
}

void MainWindow::on_actNextTab_triggered()
{
  if (!isUiUpdated()) return;

  OnTabCurrentChanged(UIP::TabBar::indexFetch(tabbar.data()) + 1);
}

void MainWindow::on_actPreviousTab_triggered()
{
  if (!isUiUpdated()) return;

  OnTabCurrentChanged(UIP::TabBar::indexFetch(tabbar.data()) - 1);
}

void MainWindow::on_actNextItem_triggered()
{
  if (!isUiUpdated()) return;

  OnListCurrentRowChanged(UIP::PageList::indexFetch(pagelist.data()) + 1);
}

void MainWindow::on_actPreviousItem_triggered()
{
  if (!isUiUpdated()) return;

  OnListCurrentRowChanged(UIP::PageList::indexFetch(pagelist.data()) - 1);
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

auto ToUpdate(const T_sig sig, MainWindow* m, const T_arg arg) -> bool
{
  bool is_updated = false;
  if (Utl::Sig::Exists(sig, Sig::TITLE)) {
    m->setWindowTitle(arg.toString());
    is_updated = true;
  }
  return is_updated;
}

}  // ns UIP::TitleBar

namespace StatusBar {

auto ToUpdate(const T_sig sig, QStatusBar* sbar, const T_arg arg_txt,
              const T_arg arg_time) -> bool
{
  bool is_updated = false;
  if (Utl::Sig::Exists(sig, Sig::STATUS_MESSAGE)) {
    sbar->showMessage(arg_txt.toString(), arg_time.toInt());
    is_updated = true;
  }
  return is_updated;
}

}  // ns UIP::StatusBar

namespace TabBar {

auto ToUpdate(const T_sig sig, QTabBar* tbar, const T_arg arg_i,
              const T_arg arg_names, const T_arg arg_stats) -> bool
{
  if (Utl::Sig::Exists(sig, Sig::TAB_NAME)) {
    Utl::Widget::Names::Merge<QTabBar>(tbar, arg_names.toStringList());
  }
  if (Utl::Sig::Exists(sig, Sig::TAB_INDEX)) {
    tbar->setCurrentIndex(arg_i.toInt());
  }
  if (Utl::Sig::Exists(sig, Sig::TAB_STATE)) {
    ColorsUpdate(tbar, arg_stats);
  }
  return true;
}

auto ColorsUpdate(QTabBar* tbar, const T_arg arg) -> bool
{
  auto stats = arg.toList();
  for (int i = 0, size = tbar->count(); i < size; ++i) {
    auto v = stats.at(i);
    tbar->setTabData(i, v);
    auto stat = v.toBool();
    tbar->setTabTextColor(i, stat ? Nmemo::VALUE::TAB_MODIFIED_COLOR:
                                    Nmemo::VALUE::TAB_UNMODIFIED_COLOR);
  }
  return true;
}

}  // ns UIP::TabBar

namespace PageList {

auto ToUpdate(const T_sig sig, QListWidget* li, const T_arg arg_i,
              const T_arg arg_names) -> bool
{
  if (Utl::Sig::Exists(sig, Sig::LIST_NAME)) {
    Utl::Widget::Names::Merge<QListWidget>(li, arg_names.toStringList());
  }
  if (Utl::Sig::Exists(sig, Sig::LIST_INDEX)) {
    li->setCurrentRow(arg_i.toInt());
  }
  return true;
}

}  // ns UIP::PageList

namespace Editor {

auto ToUpdate(const T_sig sig, QTextEdit* editor, const T_arg arg_txt,
              const T_arg arg_mode, const T_arg arg_pos, const T_arg arg_stat) -> bool
{
  auto mode = Utl::Sig::Exists(sig, Sig::EDITOR_MODE) ?
                                      static_cast<EditMode>(arg_mode.toInt()):
        EditMode::PLAIN;
  if (Utl::Sig::Exists(sig, Sig::EDITOR_TEXT)) {
    if (mode == EditMode::HTML) {
      editor->setText(arg_txt.toString());
    } else {
      editor->setPlainText(arg_txt.toString());
    }
  }
  if (Utl::Sig::Exists(sig, Sig::EDITOR_POS)) {
    editor->verticalScrollBar()->setSliderPosition(arg_pos.toInt());
  }
  if (Utl::Sig::Exists(sig, Sig::EDITOR_STATE)) {
    editor->setReadOnly(!arg_stat.toBool());
  }
  return true;
}

}  // ns UIP::Editor

}  // ns UIP
