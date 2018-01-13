/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  qSetMessagePattern("[%{time h:mm:ss.zzz} %{if-debug}DEBG%{endif}%{if-info}INFO%{endif}%{if-warning}WARN%{endif}%{if-critical}CRIT%{endif}%{if-fatal}FATL%{endif}] %{file}:%{line} - %{message}");
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}
