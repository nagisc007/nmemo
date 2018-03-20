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

#include <QHBoxLayout>
#include <QListWidget>
#include <QMainWindow>
#include <QTabBar>
#include <QTextEdit>
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
  /* members: class */
  Ui::MainWindow *ui;
  QScopedPointer<QTabBar> tabbar;
  QScopedPointer<QListWidget> pagelist;
  QScopedPointer<QTextEdit> editor;
  QScopedPointer<Core::System> sys;
  /* members: register */
  bool r_titlebar_updated;
  bool r_statusbar_updated;
  bool r_tabbar_updated;
  bool r_pagelist_updated;
  bool r_editor_updated;
  T_filter r_filter_selected;
  T_msgtime r_statusshow_time;
  T_dirname r_dirname;
  /* members: utils */
  QMutex u_mutex;
  /* methods: base */
  bool InitWidgets();
  bool InitActions();
  /* methods: features */
  bool CheckUnsaved();
  void UpdateNote();

signals:
  void asSystemData(T_cmd, T_arg, T_arg, T_arg);

public slots:
  /* for output */
  void ToTabBar(const T_cmd, const T_arg, const T_arg, const T_arg);
  void ToPageList(const T_cmd, const T_arg, const T_arg);
  void ToEditor(const T_cmd, const T_arg, const T_arg);
  void ToTitleBar(const T_cmd, const T_arg);
  void ToStatusBar(const T_cmd, const T_arg, const T_arg);
  /* for tabbar */
  void OnTabCurrentChanged(const T_index);
  void OnTabCloseRequested(const T_index);
  void OnTabMoved(const T_index, const T_index);
  /* for pagelist */
  void OnListCurrentRowChanged(const T_index);
  void OnListItemDoubleClicked(const T_item*);
  /* for editor */
  void OnEditorTextChanged();

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
  void on_actAddItem_triggered();
  void on_actDeleteItem_triggered();
  void on_actRenameItem_triggered();
  void on_actMoveNext_triggered();
  void on_actMovePrevious_triggered();
  void on_actSort_AtoZ_triggered();
  void on_actSort_ZtoA_triggered();
  /* menus: View */
  void on_actFullscreen_triggered();
  void on_actNextTab_triggered();
  void on_actPreviousTab_triggered();
  void on_actNextItem_triggered();
  void on_actPreviousItem_triggered();
  /* menus: Help */
  void on_actAboutQt_triggered();
  void on_actAboutApp_triggered();
};

/* process: UI Process */
namespace UIP {

namespace TitleBar {

bool ToUpdate(const T_cmd, MainWindow*, const T_arg);

}  // ns UIP:TitleBar

namespace StatusBar {

bool ToUpdate(const T_cmd, QStatusBar*, const T_arg, const T_arg, T_msgtime*);

}  // ns UIP:StatusBar

namespace TabBar {

namespace Index {

T_index Fetch(const QTabBar*);

}  // ns UIP::TabBar::Index

bool ToUpdate(const T_cmd, QTabBar*, const T_arg, const T_arg, const T_arg);

}  // ns UIP:TabBar

namespace PageList {

namespace Index {

T_index Fetch(const QListWidget*);

}  // ns UIP::PageList::Index

namespace Item {

constexpr T_item* Fetch(const QListWidget*);

}  // ns UIP::PageList::Item

bool ToUpdate(const T_cmd, QListWidget*, const T_arg, const T_arg);

}  // ns UIP:PageList

namespace Editor {

namespace Text {

T_text Fetch(const QTextEdit*);

}  // ns UIP::Editor::Text

namespace Act {

void Undo(QTextEdit*);
void Redo(QTextEdit*);
void Cut(QTextEdit*);
void Copy(QTextEdit*);
void Paste(QTextEdit*);
void Erase(QTextEdit*);
void SelectAll(QTextEdit*);

}  // ns UIP::Editor::Act

bool ToUpdate(const T_cmd, QTextEdit*, const T_arg, const T_arg);

}  // ns UIP:Editor

}  // namespace UIP

#endif // MAINWINDOW_H
