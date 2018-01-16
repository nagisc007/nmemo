/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "core.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  // methods: base
  bool InitWidgets();
  // methods: menus
  void OpenFile();
  void CloseFile();
  void SaveFile();
  void SaveAsFile();
  void Quit();

private slots:
  void on_actionOpen_triggered();
  void on_actionClose_triggered();
  void on_actionSave_triggered();
  void on_actionSave_As_triggered();
  void on_actionQuit_triggered();
  void on_actionAbout_Qt_triggered();
  void on_actionAbout_Nmemo_triggered();

  void on_action_Add_triggered();

  void on_action_Insert_triggered();

  void on_action_Delete_triggered();

  void on_actionSort_A_Z_triggered();

  void on_actionSort_Z_A_triggered();

private:
  Ui::MainWindow *ui;
  QScopedPointer<NMEMO::Core> core_;
};

#endif // MAINWINDOW_H
