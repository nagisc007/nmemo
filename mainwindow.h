/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  // values
  struct Values {
    static const int STATUS_MESSAGE_TIME;
  };
  // methods: base
  // methods: features

signals:

public slots:

private slots:

  void on_actOpen_triggered();

  void on_actClose_triggered();

  void on_actSave_triggered();

  void on_actSaveAs_triggered();

  void on_actQuit_triggered();

  void on_actAboutQt_triggered();

  void on_actAboutApp_triggered();

  void on_actNew_triggered();

  void on_actUndo_triggered();

  void on_actRedo_triggered();

  void on_actCut_triggered();

  void on_actCopy_triggered();

  void on_actPaste_triggered();

  void on_actErase_triggered();

  void on_actSelectAll_triggered();

  void on_actAddBook_triggered();

  void on_actDeleteBook_triggered();

  void on_actFullscreen_triggered();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
