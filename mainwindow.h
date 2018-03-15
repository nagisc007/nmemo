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
#include "memowidget.h"

#include <QMainWindow>

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
  /* members */
  Ui::MainWindow *ui;
  bool m_tab_updated;
  bool m_booklist_updated;
  bool m_memo_updated;
  bool m_status_updated;
  bool m_title_updated;
  T_filter m_selected;
  T_dirname m_dirname;
  T_msgtime m_time;
  QMutex mutex;
  QScopedPointer<Nmemo::MemoWidget> memo;
  QScopedPointer<Nmemo::Core> core;
  /* methods: base */
  bool InitWidgets();
  bool InitActions();

signals:
  void asTabData(T_cmd, T_tab_i, T_arg);
  void asBookData(T_cmd, T_book_i, T_arg);
  void asMemoData(T_text, T_stat);
  void asFileData(T_cmd, T_path);

public slots:
  /* for output */
  void ToTabBar(T_cmd, T_tab_i, T_tabnames, T_stats);
  void ToBookList(T_cmd, T_book_i, T_booknames);
  void ToEditor(T_cmd, T_stat, const T_text&);
  void ToTitleBar(const T_title&);
  void ToStatusBar(const T_msg&);
  /* for tab */
  void OnAddTab();
  void OnDeleteTab(int);
  void OnChangeTab(int);
  void OnMoveTab(int, int);
  /* for book */
  void OnAddBook();
  void OnDeleteBook(int);
  void OnChangeBook(int);
  void OnMoveBook(int, int);
  void OnRenameBook(const T_item*);
  void OnSortBook(T_order);
  /* for memo */

private slots:
  /* menus: File */
  void on_actNew_triggered();
  void on_actOpen_triggered();
  void on_actClose_triggered();
  void on_actSave_triggered();
  void on_actSaveAs_triggered();
  void on_actQuit_triggered();
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
  /* menus: Help */
  void on_actAboutQt_triggered();
  void on_actAboutApp_triggered();
};

/* process: titlebar */
namespace TitleBar {

T_title Fetch(const MainWindow*);
bool Merge(MainWindow*, const T_title&);

}  // ns TitleBar

/* process: statusbar */
namespace StatusBar {
namespace Message {

T_msg Fetch(const MainWindow*);
bool Merge(MainWindow*, const T_msg&);

}  // ns StatusBar::Message

namespace Time {

T_msgtime Fetch(const MainWindow*);
bool Merge(MainWindow*, const T_msgtime);

}  // ns StatusBar::Time
}  // ns StatusBar

#endif // MAINWINDOW_H
