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

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  core_(new NMEMO::Core())
{
  ui->setupUi(this);
  qDebug() << "MainWindow: construct";
  if (!InitWidgets()) {
    qWarning() << "MainWindow: Cannot initialize widgets!";
  }
  core_->Reset();
}

MainWindow::~MainWindow()
{
  if (core_) core_.reset();
  delete ui;
  qDebug() << "MainWindow: destruct";
}

auto MainWindow::CloseFile() -> void
{
  core_->Reset();
}

auto MainWindow::InitWidgets() -> bool
{
  return core_->SetEditor(ui->memoEditor) &&
      core_->SetList(ui->titleDisplay);
}

auto MainWindow::OpenFile() -> void
{
  core_->LoadFromFile(this);
}

auto MainWindow::Quit() -> void
{
  close();
}

auto MainWindow::SaveAsFile() -> void
{
  core_->SaveToFile(this);
}

auto MainWindow::SaveFile() -> void
{
  qDebug() << "MainWindow: Save file (unimplemented)";
}

/*
 * slots
 */
void MainWindow::on_actionOpen_triggered()
{
  OpenFile();
}

void MainWindow::on_actionClose_triggered()
{
  CloseFile();
}

void MainWindow::on_actionSave_triggered()
{
  SaveFile();
}

void MainWindow::on_actionSave_As_triggered()
{
  SaveAsFile();
}

void MainWindow::on_actionQuit_triggered()
{
  Quit();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
  QMessageBox::aboutQt(this);
}

void MainWindow::on_actionAbout_Nmemo_triggered()
{
  QString title = "About Nmemo";
  QString msg = "<h3>About Nmemo</h3><p>Nmemo is a simple memo editor by Qt.<br>Licensed by GNU GENERAL PUBLIC LICENSE Version 3.</p><p>Copyright (c) 2018 N.T.Works</p>";
  QMessageBox::about(this, title, msg);
}

void MainWindow::on_action_Add_triggered()
{
  core_->AddItem();
}

void MainWindow::on_action_Insert_triggered()
{
  core_->InsertItem();
}

void MainWindow::on_action_Delete_triggered()
{
  core_->DeleteItem();
}

void MainWindow::on_actionSort_A_Z_triggered()
{
  core_->SortItems(0);
}

void MainWindow::on_actionSort_Z_A_triggered()
{
  core_->SortItems(1);
}
