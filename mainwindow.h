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

namespace APP {

struct Version {
  static const int MAJOR;
  static const int MINOR;
  static const int MICRO;
};

struct Values {
  static const QString NAME;
  static const QString VERSION;
  static const QString AUTHORS;
  static const QString DESCRIPTION;
  static const QString LICENSE;
  static const QString COPYRIGHT;
};

}  // namespace APP

/* class: MainWindow */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  /* con[de]structor */
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  /* values */
  struct Values {
    static const int STATUS_MESSAGE_TIME;
  };
  /* methods: base */
  bool InitWidgets();
  bool InitActions();
  /* methods: features */

signals:
  void updated(CmdSig, int, QVariant, const QString&);

public slots:
  /* for output */
  void outputToTab(int, QStringList);
  void outputToBookList(int, QStringList);
  void outputToEditor(bool, const QString&);
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
  QScopedPointer<QListWidget> booklist_;
  QScopedPointer<QTextEdit> editor_;
  QScopedPointer<QTabBar> tab_;
  QScopedPointer<Nmemo::Core> core_;
};

#endif // MAINWINDOW_H
