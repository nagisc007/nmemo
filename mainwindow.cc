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

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  qDebug() << "MainWindow: construct";
}

MainWindow::~MainWindow()
{
  delete ui;
  qDebug() << "MainWindow: destruct";
}

auto MainWindow::AboutApp() -> void
{
  qDebug() << "MainWindow: About app (unimplemented)";
}

auto MainWindow::AboutQt() -> void
{
  qDebug() << "MainWindow: About Qt (unimplemented)";
}

auto MainWindow::CloseFile() -> void
{
  qDebug() << "MainWindow: Close file (unimplemented)";
}

auto MainWindow::OpenFile() -> void
{
  qDebug() << "MainWindow: Open file (unimplemented)";
}

auto MainWindow::Quit() -> void
{
  close();
}

auto MainWindow::SaveAsFile() -> void
{
  qDebug() << "MainWindow: Save As file (unimplemented)";
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
  AboutQt();
}

void MainWindow::on_actionAbout_Nmemo_triggered()
{
  AboutApp();
}
