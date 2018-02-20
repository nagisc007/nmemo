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

/* methods: base */

/* methods: features */

/* slots */

/* slots: menus */

void MainWindow::on_actOpen_triggered()
{

}

void MainWindow::on_actClose_triggered()
{
}

void MainWindow::on_actSave_triggered()
{

}

void MainWindow::on_actSaveAs_triggered()
{

}

void MainWindow::on_actQuit_triggered()
{
  close();
}

void MainWindow::on_actAboutQt_triggered()
{

}

void MainWindow::on_actAboutApp_triggered()
{

}

void MainWindow::on_actNew_triggered()
{

}

void MainWindow::on_actUndo_triggered()
{

}

void MainWindow::on_actRedo_triggered()
{

}

void MainWindow::on_actCut_triggered()
{

}

void MainWindow::on_actCopy_triggered()
{

}

void MainWindow::on_actPaste_triggered()
{

}

void MainWindow::on_actErase_triggered()
{

}

void MainWindow::on_actSelectAll_triggered()
{

}

void MainWindow::on_actAddBook_triggered()
{

}

void MainWindow::on_actDeleteBook_triggered()
{

}

void MainWindow::on_actFullscreen_triggered()
{

}
