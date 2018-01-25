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

/* class */
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  core_(nullptr)
{
  ui->setupUi(this);
  if (!InitWidgets()) {
    qWarning() << "MainWindow: Cannot initialize widgets!";
  }
  if (!InitActions()) {
    qWarning() << "MainWindow: Cannot initialize actions!";
  }
  qDebug() << "MainWindow: construct";

  // reset
  setWindowTitle(APP::Values::NAME);
  emit resetQueue();
}

MainWindow::~MainWindow()
{
  if (core_) core_.reset();
  delete ui;
  qDebug() << "MainWindow: destruct";
}

/* methods: base */
auto MainWindow::InitActions() -> bool
{
  if (core_.isNull()) return false;
  connect(this, &MainWindow::fileOpenQueue, core_.data(), &NMEMO::Core::OnLoadFile);
  connect(this, &MainWindow::itemAddQueue, core_.data(), &NMEMO::Core::OnAddItem);
  connect(this, &MainWindow::itemDeleteQueue, core_.data(), &NMEMO::Core::OnDeleteItem);
  connect(this, &MainWindow::itemInsertQueue, core_.data(), &NMEMO::Core::OnInsertItem);
  connect(this, &MainWindow::resetQueue, core_.data(), &NMEMO::Core::OnReset);
  connect(this, &MainWindow::itemSortQueue, core_.data(), &NMEMO::Core::OnSortItems);
  connect(this, &MainWindow::saveFileQueue, core_.data(), &NMEMO::Core::OnSaveToFile);
  connect(core_.data(), &NMEMO::Core::filenameChangeQueue, this, &MainWindow::OnChangeFilename);
  connect(core_.data(), &NMEMO::Core::statusMessageQueue, this, &MainWindow::OnStatusMessage);
  return true;
}

auto MainWindow::InitWidgets() -> bool
{
  core_.reset(new NMEMO::Core());
  return !core_.isNull() &&
      core_->SetEditor(ui->memoEditor) &&
      core_->SetList(ui->titleDisplay);
}

/* methods: features */

/* slots */
void MainWindow::OnChangeFilename(const QString& filename, bool is_modified)
{
  auto title = QString("%1%2[%3]")
      .arg(is_modified ? "*": "")
      .arg(APP::Values::NAME)
      .arg(filename);
  setWindowTitle(title);
}

auto MainWindow::OnStatusMessage(const QString& msg) -> void
{
  ui->statusBar->showMessage(msg, Values::STATUS_MESSAGE_TIME);
}

/* slots: menus */
void MainWindow::on_actionOpen_triggered()
{
  emit fileOpenQueue(this);
}

void MainWindow::on_actionClose_triggered()
{
  emit resetQueue();
}

void MainWindow::on_actionSave_triggered()
{
  emit saveFileQueue(this, false);
}

void MainWindow::on_actionSave_As_triggered()
{
  emit saveFileQueue(this, true);
}

void MainWindow::on_actionQuit_triggered()
{
  close();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
  QMessageBox::aboutQt(this);
}

void MainWindow::on_actionAbout_Nmemo_triggered()
{
  auto title = QString("About %1")
      .arg(APP::Values::NAME);
  auto msg = QString("<h3>About %1 %2</h3><p>%3<br>Licensed by %4.<br>%5</p><p>%6</p>")
      .arg(APP::Values::NAME)
      .arg(APP::Values::VERSION)
      .arg(APP::Values::DESCRIPTION)
      .arg(APP::Values::LICENSE)
      .arg(APP::Values::COPYRIGHT)
      .arg(APP::Values::AUTHORS);
  QMessageBox::about(this, title, msg);
}

void MainWindow::on_action_Add_triggered()
{
  emit itemAddQueue();
}

void MainWindow::on_action_Insert_triggered()
{
  emit itemInsertQueue();
}

void MainWindow::on_action_Delete_triggered()
{
  emit itemDeleteQueue();
}

void MainWindow::on_actionSort_A_Z_triggered()
{
  emit itemSortQueue(NMEMO::SortStyle::AtoZ);
}

void MainWindow::on_actionSort_Z_A_triggered()
{
  emit itemSortQueue(NMEMO::SortStyle::ZtoA);
}
