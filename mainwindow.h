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
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>

namespace Ui {
class MainWindow;
}

/* class: MainWindow */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  /* con[de]structor */
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  /* values */
  /* methods: base */
  bool InitWidgets();
  bool InitActions();
  /* methods: features */

signals:
  void updated(T_cmd, T_index, T_text, T_arg);
  void edited();
  void loaded(T_fname, T_tab_i, T_text);
  void saved(T_fname, T_tab_i, T_text);

public slots:
  /* for output */
  void outputToTab(T_tab_i, T_tabnames);
  void outputToBookList(T_book_i, T_booknames);
  void outputToEditor(T_stat, const T_memo&);
  void updateStatus(const QString&);
  void updateFile(const T_fname&, T_isUpdated);
  /* for tab */
  void AddTab();
  void DeleteTab(int);
  void ChangeTab(int);
  void MoveTab(int, int);
  void RenameTab();
  /* for book */
  void AddBook();
  void DeleteBook(int);
  void ChangeBook(int);
  void MoveBook(int, int);
  void RenameBook();
  void DoubleClickBook(QListWidgetItem*);
  void SortBook(T_order);
  /* for memo */
  void ChangeTextInMemo();

private slots:
  /* menus: File */
  void on_actNew_triggered();
  void on_actOpen_triggered();
  void on_actClose_triggered();
  void on_actSave_triggered();
  void on_actSaveAs_triggered();
  void on_actQuit_triggered();
  /* menus: Help */
  void on_actAboutQt_triggered();
  void on_actAboutApp_triggered();
  /* menus: Edit */
  void on_actUndo_triggered();
  void on_actRedo_triggered();
  void on_actCut_triggered();
  void on_actCopy_triggered();
  void on_actPaste_triggered();
  void on_actErase_triggered();
  void on_actSelectAll_triggered();
  /* menus: Book */
  void on_actAddBook_triggered();
  void on_actDeleteBook_triggered();
  void on_actRenameBook_triggered();
  void on_actMoveNext_triggered();
  void on_actMovePrevious_triggered();
  void on_actSort_AtoZ_triggered();
  void on_actSort_ZtoA_triggered();
  /* menus: View */
  void on_actFullscreen_triggered();
  void on_actNextTab_triggered();
  void on_actPreviousTab_triggered();
  void on_actNextBook_triggered();
  void on_actPreviousBook_triggered();

private:
  Ui::MainWindow *ui;
  bool is_booklist_updating_;
  bool is_editor_updating_;
  bool is_tab_updating_;
  QMutex mutex_;
  T_fname filename_;
  T_filter selected_;
  QScopedPointer<QListWidget> booklist_;
  QScopedPointer<QTextEdit> editor_;
  QScopedPointer<QTabBar> tab_;
  QScopedPointer<Nmemo::Core> core_;
};

#endif // MAINWINDOW_H
