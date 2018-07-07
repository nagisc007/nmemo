/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common_types.h"
#include "common_values.h"
#include "cpu.h"
#include "gpu.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QMutex>
#include <QScrollBar>
#include <QTabBar>
#include <QTextEdit>
#include <QThread>
#include <QVBoxLayout>
#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class MainWindow;
}

// struct: MainReg
struct MainReg {
  MainReg();
  // members
  T_str filter;
  T_str dirname;
  bool ui_updating;
};

// class: MainWindow
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  // members
  Ui::MainWindow *ui;
  MainReg reg;
  QScopedPointer<QTabBar> filetab;
  QScopedPointer<QTabBar> booktab;
  QScopedPointer<QListWidget> pagelist;
  QScopedPointer<QTextEdit> editor;
  QScopedPointer<CPU::Core> cpu;
  QScopedPointer<GPU::Core> gpu;
  QScopedPointer<QThread> cpu_th;
  QScopedPointer<QThread> gpu_th;
  QMutex mutex;
  // methods
  bool InitWidgets();
  bool InitConnections();
  // stream
  bool ToCheckUIandUpdateText();

signals:
  void ToCpu(T_cpu_addr, int i = -1, T_str s = T_str());
  void ToCpuIrq(T_cpu_irq);
  void ToGpuIrq(T_gpu_irq);

public slots:
  void FromGpu(T_dev_addr, T_ivec, T_strs, T_states);
  void FromCpuError(T_cpu_result);
  void FromGpuError(T_gpu_result);

private slots:
  // ui
  void OnFileTabCurrentChanged(const T_index);
  void OnFileTabCloseRequested(const T_index);
  void OnFileTabMoved(const T_index, const T_index);
  void OnBookTabCurrentChanged(const T_index);
  void OnBookTabCloseRequested(const T_index);
  void OnBookTabMoved(const T_index, const T_index);
  void OnPageListCurrentRowChanged(const T_index);
  void OnPageListItemDoubleClicked(const T_item*);
  void OnEditorTextChanged();
  // menus
  void on_fileNew_triggered();
  void on_fileOpen_triggered();
  void on_fileSave_triggered();
  void on_fileSaveAs_triggered();
  void on_fileRename_triggered();
  void on_fileClose_triggered();
  void on_appQuit_triggered();
  void on_editUndo_triggered();
  void on_editRedo_triggered();
  void on_editCut_triggered();
  void on_editCopy_triggered();
  void on_editPaste_triggered();
  void on_editErase_triggered();
  void on_editSelectAll_triggered();
  void on_bookAdd_triggered();
  void on_bookDelete_triggered();
  void on_bookRename_triggered();
  void on_bookMoveNext_triggered();
  void on_bookMovePrevious_triggered();
  void on_booksSortAtoZ_triggered();
  void on_booksSortZtoA_triggered();
  void on_pageAdd_triggered();
  void on_pageDelete_triggered();
  void on_pageRename_triggered();
  void on_pageMoveNext_triggered();
  void on_pageMovePrevious_triggered();
  void on_pagesSortAtoZ_triggered();
  void on_pagesSortZtoA_triggered();
  void on_toggleFullscreen_triggered();
  void on_fileChangeNext_triggered();
  void on_fileChangePrevious_triggered();
  void on_bookChangeNext_triggered();
  void on_bookChangePrevious_triggered();
  void on_pageChangeNext_triggered();
  void on_pageChangePrevious_triggered();
  void on_appAboutQt_triggered();
  void on_appAboutApp_triggered();

private:
  /* override */
  void closeEvent(QCloseEvent*) override;
};

#endif // MAINWINDOW_H
