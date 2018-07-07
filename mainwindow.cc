/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QBrush>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QInputDialog>
#include <QMessageBox>

namespace {

const T_str _FILE_NEW_TITLE = "New file name";
const T_str _FILE_NEW_CAPTION = "Input a file name";
const T_str _FILE_OPEN_CAPTION = "Open a memo file";
const T_str _FILE_SAVEAS_TITLE = "Save the file as a name";
const T_str _FILE_SAVEAS_CAPTION = "Input a new name for saving this file";
const int _PAGELIST_MAXWIDTH = 120;
const qreal _EDITOR_TABDISTANCE = 40;

bool _IsExistsAddr(T_dev_addr addr, T_dev_addr target)
{
  return (static_cast<int>(addr) & static_cast<int>(target)) != 0;
}

T_strs _baseNamesOf(T_strs strs)
{
  T_strs res;
  for (auto& v: strs) {
    res << QFileInfo(v).baseName();
  }
  return res;
}

T_str _filePathOpened(QWidget* parent, const T_str& caption, const T_str& path,
                      const T_str& filter, T_str* selected)
{
  return QFileDialog::getOpenFileName(parent, caption, path, filter, selected);
}

T_str _filePathSaved(QWidget* parent, const T_str& caption, const T_str& path,
                      const T_str& filter, T_str* selected)
{
  return QFileDialog::getSaveFileName(parent, caption, path, filter, selected);
}

T_str _nameInputted(QWidget* parent, const T_str& title, const T_str& caption,
                    const T_str& name)
{
  return QInputDialog::getText(parent, title, caption, QLineEdit::Normal, name);
}

int _fromWithTo(T_index from, T_index to)
{
  return static_cast<int>(from) << 8 | static_cast<int>(to);
}

bool _SetFilePathsToTabData(QTabBar* tab, T_strs strs)
{
  for (int i = 0; i < strs.size(); ++i) {
    tab->setTabData(i, QVariant(strs.at(i)));
  }
  return true;
}

bool _SwapTabLabels(QTabBar* tab, T_strs strs)
{
  while (tab->count() > 0)
    tab->removeTab(0);
  for (auto& v: strs)
    tab->addTab(v);

  return true;
}

bool _SwapListWidgetLabels(QListWidget* list, T_strs strs)
{
  list->clear();
  list->addItems(strs);
  return true;
}

bool _ChangeTabColors(QTabBar* tab, T_states states)
{
  for (int i = 0; i < states.size(); ++i) {
    tab->setTabTextColor(i, states.at(i) ? DEFAULT::MODIFY_LABEL_COLOR:
                                           DEFAULT::NORMAL_LABEL_COLOR);
  }
  return true;
}

bool _ChangeListWidgetColors(QListWidget* list, T_states states)
{
  for (int i = 0; i < states.size(); ++i) {
    list->item(i)->setForeground(QBrush(states.at(i) ? DEFAULT::MODIFY_LABEL_COLOR:
                                                       DEFAULT::NORMAL_LABEL_COLOR));
  }
  return true;
}

}  // inner global

// struct: MainReg
MainReg::MainReg():
  filter(DEFAULT::SELECTED_FILTER),
  dirname(QDir::currentPath()),
  ui_updating(false)
{}

// class: Mainwindow
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
  ui(new Ui::MainWindow),
  filetab(new QTabBar(this)),
  booktab(new QTabBar(this)),
  pagelist(new QListWidget(this)),
  editor(new QTextEdit(this)),
  cpu(new CPU::Core()),
  gpu(new GPU::Core()),
  cpu_th(new QThread()),
  gpu_th(new QThread())
{
  ui->setupUi(this);
  if (!InitWidgets()) {
    qWarning() << "MainWindow: cannot initialize widgets!";
    exit(EXIT_FAILURE);
  }
  if (!InitConnections()) {
    qWarning() << "MainWindow: cannot initialize actions!";
    exit(EXIT_FAILURE);
  }
  qDebug() << "MainWindow: construct";
  emit ToCpuIrq(CPU::Irq::RESET);
}

MainWindow::~MainWindow()
{
  if (cpu_th) {
    cpu_th->quit();
    cpu_th->wait();
    cpu_th.reset();
  }
  if (gpu_th) {
    gpu_th->quit();
    gpu_th->wait();
    gpu_th.reset();
  }
  delete ui;
  qDebug() << "MainWindow: destruct";
}

// override methods
void MainWindow::closeEvent(QCloseEvent* evt)
{
  QWidget::closeEvent(evt);
}

// methods: base
auto MainWindow::InitWidgets() -> bool
{
  // layout
  auto mainLayout = new QVBoxLayout();
  auto fileLayout = new QVBoxLayout();
  auto bookLayout = new QHBoxLayout();
  auto mainWidget = new QWidget();
  bookLayout->addWidget(editor.data());
  bookLayout->addWidget(pagelist.data());
  fileLayout->addWidget(booktab.data());
  fileLayout->addLayout(bookLayout);
  mainLayout->addLayout(fileLayout);
  mainLayout->addWidget(filetab.data());
  mainWidget->setLayout(mainLayout);
  setCentralWidget(mainWidget);

  // props
  filetab->setTabsClosable(true);
  filetab->setMovable(true);
  booktab->setTabsClosable(true);
  booktab->setMovable(true);
  pagelist->setMaximumWidth(_PAGELIST_MAXWIDTH);
  editor->setTabStopDistance(_EDITOR_TABDISTANCE);
  editor->setReadOnly(true);
  editor->setAcceptRichText(false);

  return true;
}

auto MainWindow::InitConnections() -> bool
{
  // dev to cpu
  connect(this, &MainWindow::ToCpu, cpu.data(), &CPU::Core::FromDev);
  connect(this, &MainWindow::ToCpuIrq, cpu.data(), &CPU::Core::FromIrq);
  // dev to gpu
  connect(this, &MainWindow::ToGpuIrq, gpu.data(), &GPU::Core::FromIrq);
  // cpu to gpu
  connect(cpu.data(), &CPU::Core::ToGpu, gpu.data(), &GPU::Core::FromCpu);
  connect(cpu.data(), &CPU::Core::ToGpuIrq, gpu.data(), &GPU::Core::FromIrq);
  // cpu to dev
  connect(cpu.data(), &CPU::Core::ToError, this, &MainWindow::FromCpuError);
  // gpu to dev
  connect(gpu.data(), &GPU::Core::ToDev, this, &MainWindow::FromGpu);
  connect(gpu.data(), &GPU::Core::ToError, this, &MainWindow::FromGpuError);

  // filetab to main
  connect(filetab.data(), &QTabBar::currentChanged, this, &MainWindow::OnFileTabCurrentChanged);
  connect(filetab.data(), &QTabBar::tabCloseRequested,
          this, &MainWindow::OnFileTabCloseRequested);
  connect(filetab.data(), &QTabBar::tabMoved, this, &MainWindow::OnFileTabMoved);
  // booktab to main
  connect(booktab.data(), &QTabBar::currentChanged, this, &MainWindow::OnBookTabCurrentChanged);
  connect(booktab.data(), &QTabBar::tabCloseRequested,
          this, &MainWindow::OnBookTabCloseRequested);
  connect(booktab.data(), &QTabBar::tabMoved, this, &MainWindow::OnBookTabMoved);
  // pagelist to main
  connect(pagelist.data(), &QListWidget::currentRowChanged,
          this, &MainWindow::OnPageListCurrentRowChanged);
  connect(pagelist.data(), &QListWidget::itemDoubleClicked,
          this, &MainWindow::OnPageListItemDoubleClicked);
  // editor to main
  connect(editor.data(), &QTextEdit::textChanged, this, &MainWindow::OnEditorTextChanged);

  return true;
}

// filetab
void MainWindow::OnFileTabCurrentChanged(const T_index index)
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::FILE_CHANGE, index);
}

void MainWindow::OnFileTabCloseRequested(const T_index index)
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::FILE_CLOSE, index);
}

void MainWindow::OnFileTabMoved(const T_index from, const T_index to)
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::FILE_MOVE, _fromWithTo(from, to));
}

// booktab
void MainWindow::OnBookTabCurrentChanged(const T_index index)
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::BOOK_CHANGE, index);
}

void MainWindow::OnBookTabCloseRequested(const T_index index)
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::BOOK_DELETE, index);
}

void MainWindow::OnBookTabMoved(const T_index from, const T_index to)
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::BOOK_MOVE, _fromWithTo(from, to));
}

// pagelist
void MainWindow::OnPageListCurrentRowChanged(const T_index index)
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::PAGE_CHANGE, index);
}

void MainWindow::OnPageListItemDoubleClicked(const T_item* item)
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::PAGE_RENAME, pagelist->row(item),
             _nameInputted(this, _FILE_NEW_TITLE, _FILE_NEW_CAPTION,
                           item->text()));
}

/* slots: Editor */
void MainWindow::OnEditorTextChanged()
{
  if (reg.ui_updating) return;

  emit ToCpu(CPU::Addr::TEXT_MODIFY, 0, T_str());
}

// slots
void MainWindow::FromCpuError(T_cpu_result res)
{
  qDebug() << "CPU Error: " << static_cast<int>(res);
}

void MainWindow::FromGpu(T_dev_addr addr, T_ivec ivec, T_strs strs, T_states states)
{
  // window
  if (_IsExistsAddr(addr, DEV::Addr::WINDOW_TITLE)) {
    mutex.lock();
    reg.ui_updating = true;
    setWindowTitle(strs.at(0));
    mutex.unlock();
    reg.ui_updating = false;
  }

  // filetab
  if (_IsExistsAddr(addr, DEV::Addr::FILETAB_LABELS)) {
    mutex.lock();
    reg.ui_updating = true;
    _SwapTabLabels(filetab.data(), _baseNamesOf(strs));
    _SetFilePathsToTabData(filetab.data(), strs);
    mutex.unlock();
    reg.ui_updating = false;
  }
  if (_IsExistsAddr(addr, DEV::Addr::FILETAB_INDEX)) {
    mutex.lock();
    reg.ui_updating = true;
    filetab->setCurrentIndex(ivec.at(0));
    mutex.unlock();
    reg.ui_updating = false;
  }
  if (_IsExistsAddr(addr, DEV::Addr::FILETAB_STATES)) {
    mutex.lock();
    reg.ui_updating = true;
    _ChangeTabColors(filetab.data(), states);
    mutex.unlock();
    reg.ui_updating = false;
  }
  // booktab
  if (_IsExistsAddr(addr, DEV::Addr::BOOKTAB_LABELS)) {
    mutex.lock();
    reg.ui_updating = true;
    _SwapTabLabels(booktab.data(), strs);
    mutex.unlock();
    reg.ui_updating = false;
  }
  if (_IsExistsAddr(addr, DEV::Addr::BOOKTAB_INDEX)) {
    mutex.lock();
    reg.ui_updating = true;
    booktab->setCurrentIndex(ivec.at(0));
    mutex.unlock();
    reg.ui_updating = false;
  }
  if (_IsExistsAddr(addr, DEV::Addr::BOOKTAB_STATES)) {
    mutex.lock();
    reg.ui_updating = true;
    _ChangeTabColors(booktab.data(), states);
    mutex.unlock();
    reg.ui_updating = false;
  }
  // pagelist
  if (_IsExistsAddr(addr, DEV::Addr::PAGELIST_LABELS)) {
    mutex.lock();
    reg.ui_updating = true;
    _SwapListWidgetLabels(pagelist.data(), strs);
    mutex.unlock();
    reg.ui_updating = false;
  }
  if (_IsExistsAddr(addr, DEV::Addr::PAGELIST_INDEX)) {
    mutex.lock();
    reg.ui_updating = true;
    pagelist->setCurrentRow(ivec.at(0));
    mutex.unlock();
    reg.ui_updating = false;
  }
  if (_IsExistsAddr(addr, DEV::Addr::PAGELIST_STATES)) {
    mutex.lock();
    reg.ui_updating = true;
    _ChangeListWidgetColors(pagelist.data(), states);
    mutex.unlock();
    reg.ui_updating = false;
  }
  // editor
  if (_IsExistsAddr(addr, DEV::Addr::EDITOR_READONLY)) {
    mutex.lock();
    reg.ui_updating = true;
    editor->setReadOnly(ivec.at(0));
    mutex.unlock();
    reg.ui_updating = false;
  }
  if (_IsExistsAddr(addr, DEV::Addr::EDITOR_TEXT)) {
    mutex.lock();
    reg.ui_updating = true;
    editor->setText(strs.at(0));
    mutex.unlock();
    reg.ui_updating = false;
  }
  // statusbar
  if (_IsExistsAddr(addr, DEV::Addr::STATUS_MESSAGE)) {
    mutex.lock();
    reg.ui_updating = true;
    statusBar()->showMessage(strs.at(0), DEFAULT::STATUS_MESSAGE_TIME);
    mutex.unlock();
    reg.ui_updating = false;
  }
}

void MainWindow::FromGpuError(T_gpu_result res)
{
  qDebug() << "GPU Error: " << static_cast<int>(res);
}

// streams
bool MainWindow::ToCheckUIandUpdateText()
{
  if (reg.ui_updating) return false;

  if (editor->isReadOnly()) return true;

  emit ToCpu(CPU::Addr::TEXT_UPDATE, 0, editor->toPlainText());
  return true;
}

// slots: menus
void MainWindow::on_fileNew_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::FILE_NEW, 0,
             _nameInputted(this, _FILE_NEW_TITLE, _FILE_NEW_CAPTION, DEFAULT::FILE_TITLE));
}

void MainWindow::on_fileOpen_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::FILE_OPEN, 0,
             _filePathOpened(this, _FILE_OPEN_CAPTION, reg.dirname, COMMON::FILE_FILTER,
                             &reg.dirname));
}

void MainWindow::on_fileSave_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::FILE_SAVE, filetab->currentIndex());
}

void MainWindow::on_fileSaveAs_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::FILE_SAVEAS, filetab->currentIndex(),
             _filePathSaved(this, _FILE_SAVEAS_CAPTION,
                            QString("%1/%2").arg(reg.dirname)
                            .arg(filetab->tabData(filetab->currentIndex()).toString()),
                            COMMON::FILE_FILTER, &reg.filter));
}

void MainWindow::on_fileRename_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::FILE_RENAME, filetab->currentIndex(),
             _nameInputted(this, _FILE_NEW_TITLE, _FILE_NEW_CAPTION,
                           filetab->tabData(filetab->currentIndex()).toString()));
}

void MainWindow::on_fileClose_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::FILE_CLOSE, filetab->currentIndex());
}

void MainWindow::on_appQuit_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  close();
}

void MainWindow::on_editUndo_triggered()
{
  if (reg.ui_updating || editor->isReadOnly()) return;

  editor->undo();
}

void MainWindow::on_editRedo_triggered()
{
  if (reg.ui_updating || editor->isReadOnly()) return;

  editor->redo();
}

void MainWindow::on_editCut_triggered()
{
  if (reg.ui_updating || editor->isReadOnly()) return;

  editor->cut();
}

void MainWindow::on_editCopy_triggered()
{
  if (reg.ui_updating || editor->isReadOnly()) return;

  editor->copy();
}

void MainWindow::on_editPaste_triggered()
{
  if (reg.ui_updating || editor->isReadOnly()) return;

  editor->paste();
}

void MainWindow::on_editErase_triggered()
{
  if (reg.ui_updating || editor->isReadOnly()) return;

  editor->textCursor().removeSelectedText();
}

void MainWindow::on_editSelectAll_triggered()
{
  if (reg.ui_updating || editor->isReadOnly()) return;

  editor->selectAll();
}

void MainWindow::on_bookAdd_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::BOOK_ADD, 0,
             _nameInputted(this, _FILE_NEW_TITLE, _FILE_NEW_CAPTION,
                           DEFAULT::BOOK_TITLE));
}

void MainWindow::on_bookDelete_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::BOOK_DELETE, booktab->currentIndex());
}

void MainWindow::on_bookRename_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::BOOK_RENAME, booktab->currentIndex(),
             _nameInputted(this, _FILE_NEW_TITLE, _FILE_NEW_CAPTION,
                           booktab->tabText(booktab->currentIndex())));
}

void MainWindow::on_bookMoveNext_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::BOOK_MOVE,
             _fromWithTo(booktab->currentIndex(), booktab->currentIndex() + 1));
}

void MainWindow::on_bookMovePrevious_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::BOOK_MOVE,
             _fromWithTo(booktab->currentIndex(), booktab->currentIndex() - 1));
}

void MainWindow::on_booksSortAtoZ_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::BOOK_SORT, Qt::AscendingOrder);
}

void MainWindow::on_booksSortZtoA_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::BOOK_SORT, Qt::DescendingOrder);
}

void MainWindow::on_pageAdd_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::PAGE_ADD, 0,
             _nameInputted(this, _FILE_NEW_TITLE, _FILE_NEW_CAPTION, DEFAULT::PAGE_TITLE));
}

void MainWindow::on_pageDelete_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::PAGE_DELETE, pagelist->currentRow());
}

void MainWindow::on_pageRename_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::PAGE_RENAME, pagelist->currentRow(),
             _nameInputted(this, _FILE_NEW_TITLE, _FILE_NEW_CAPTION,
                           pagelist->item(pagelist->currentRow())->text()));
}

void MainWindow::on_pageMoveNext_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::PAGE_MOVE,
             _fromWithTo(pagelist->currentRow(), pagelist->currentRow() + 1));
}

void MainWindow::on_pageMovePrevious_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::PAGE_MOVE,
             _fromWithTo(pagelist->currentRow(), pagelist->currentRow() - 1));
}

void MainWindow::on_pagesSortAtoZ_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::PAGE_SORT, Qt::AscendingOrder);
}

void MainWindow::on_pagesSortZtoA_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::PAGE_SORT, Qt::DescendingOrder);
}

void MainWindow::on_toggleFullscreen_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  if (isFullScreen()) {
    ui->toggleFullscreen->setChecked(false);
    showNormal();
  } else {
    ui->toggleFullscreen->setChecked(true);
    showFullScreen();
  }
}

void MainWindow::on_fileChangeNext_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::FILE_CHANGE, filetab->currentIndex() + 1);
}

void MainWindow::on_fileChangePrevious_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::FILE_CHANGE, filetab->currentIndex() - 1);
}

void MainWindow::on_bookChangeNext_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::BOOK_CHANGE, booktab->currentIndex() + 1);
}

void MainWindow::on_bookChangePrevious_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::BOOK_CHANGE, booktab->currentIndex() - 1);
}

void MainWindow::on_pageChangeNext_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::PAGE_CHANGE, pagelist->currentRow() + 1);
}

void MainWindow::on_pageChangePrevious_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  emit ToCpu(CPU::Addr::PAGE_CHANGE, pagelist->currentRow() - 1);
}

void MainWindow::on_appAboutQt_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  QMessageBox::aboutQt(this);
}

void MainWindow::on_appAboutApp_triggered()
{
  if (!ToCheckUIandUpdateText()) return;

  auto title = QString("About %1")
      .arg(APP::NAME);
  auto msg = QString("<h3>About %1 %2</h3><p>%3<br>Licensed by %4.<br>%5</p><p>%6</p>")
      .arg(APP::NAME)
      .arg(APP::VERSION_STRING)
      .arg(APP::DESCRIPTION)
      .arg(APP::LICENSE)
      .arg(APP::COPYRIGHT)
      .arg(APP::AUTHORS);
  QMessageBox::about(this, title, msg);
}
