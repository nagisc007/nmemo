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
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>
#include <QScrollBar>
#include <QTabBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QCloseEvent>

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
  QScopedPointer<QLabel> mode_label;
  QScopedPointer<Core::System> sys;
  /* members: register */
  bool r_ui_updated;
  T_mode r_current_mode;
  T_filter r_filter_selected;
  T_dirname r_dirname;
  /* members: utils */
  QMutex u_mutex;
  /* methods: base */
  bool InitWidgets();
  bool InitActions();
  bool ToInitWidgetsLayouts();
  bool ToInitWidgetsProperties();
  /* methods: features */
  bool ExistsUnsavedAll();
  void UpdateNote();
  bool isDeletedBook();
  bool isDeletedPage();
  bool isApplyedClosed();
  /* methods: utils */
  inline constexpr bool isUnsaved(const T_index i) {
    return i < tabbar->count() && !tabbar->tabData(i).toBool();
  }
  inline constexpr bool isUiUpdated() {
    return r_ui_updated;
  }

signals:
  void asSystemData(T_code, T_arg, T_arg, T_arg);

public slots:
  /* for output */
  void ToTabBar(const T_sig, const T_arg, const T_arg, const T_arg);
  void ToPageList(const T_sig, const T_arg, const T_arg);
  void ToEditor(const T_sig, const T_arg, const T_arg, const T_arg, const T_arg);
  void ToTitleBar(const T_sig, const T_arg);
  void ToStatusBar(const T_sig, const T_arg, const T_arg);
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
  void on_actEditPlainText_triggered();
  void on_actEditRichText_triggered();
  void on_actNextTab_triggered();
  void on_actPreviousTab_triggered();
  void on_actNextItem_triggered();
  void on_actPreviousItem_triggered();
  /* menus: Help */
  void on_actAboutQt_triggered();
  void on_actAboutApp_triggered();

private:
  /* override */
  void closeEvent(QCloseEvent*) override;
};

/* process: UI Process */
namespace UIP {

namespace TitleBar {

bool ToUpdate(const T_sig, MainWindow*, const T_arg);

}  // ns UIP:TitleBar

namespace StatusBar {

bool ToUpdate(const T_sig, MainWindow*, const T_arg, const T_arg);

}  // ns UIP:StatusBar

namespace TabBar {

inline T_index indexFetch(const QTabBar* tbar) {
  return tbar->currentIndex();
}

bool ToUpdate(const T_sig, QTabBar*, const T_arg, const T_arg, const T_arg);
bool ColorsUpdate(QTabBar*, const T_arg);

}  // ns UIP:TabBar

namespace PageList {

inline T_index indexFetch(const QListWidget* li) {
  return li->currentRow();
}
inline T_item* itemFetch(const QListWidget* li) {
  return li->currentItem();
}

bool ToUpdate(const T_sig, QListWidget*, const T_arg, const T_arg);

}  // ns UIP:PageList

namespace Editor {

inline T_text textFetch(const T_mode mode, const QTextEdit* editor) {
  return mode == EditMode::HTML ? editor->toHtml(): editor->toPlainText();
}

inline T_pos posFetch(const QTextEdit* editor) {
  return editor->verticalScrollBar()->sliderPosition();
}

namespace Act {

inline void Undo(QTextEdit* e) {
  if (!e->isReadOnly()) e->undo();
}
inline void Redo(QTextEdit* e) {
  if (!e->isReadOnly()) e->redo();
}
inline void Cut(QTextEdit* e) {
  if (!e->isReadOnly()) e->cut();
}
inline void Copy(QTextEdit* e) {
  if (!e->isReadOnly()) e->copy();
}
inline void Paste(QTextEdit* e) {
  if (!e->isReadOnly()) e->paste();
}
inline void Erase(QTextEdit* e) {
  if (!e->isReadOnly()) e->textCursor().removeSelectedText();
}
inline void SelectAll(QTextEdit* e) {
  if (!e->isReadOnly()) e->selectAll();
}

}  // ns UIP::Editor::Act

bool ToUpdate(const T_sig, QTextEdit*, const T_arg, const T_arg, const T_arg, const T_arg);

}  // ns UIP:Editor

}  // namespace UIP

#endif // MAINWINDOW_H
