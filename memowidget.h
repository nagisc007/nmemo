/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef MEMOWIDGET_H
#define MEMOWIDGET_H

#include "common_values.h"
#include "utils.h"

#include <QHBoxLayout>
#include <QListWidget>
#include <QTabBar>
#include <QTextEdit>
#include <QWidget>
#include <QVBoxLayout>

namespace Nmemo {

class MemoWidget : public QWidget
{
  Q_OBJECT
public:
  explicit MemoWidget(QWidget *parent = nullptr);
  ~MemoWidget();
  /* members */
  QScopedPointer<QListWidget> booklist;
  QScopedPointer<QTextEdit> editor;
  QScopedPointer<QTabBar> tabbar;
  /* processes: tabbar */
  T_tab_i TabBar_Fetch();
  using TabBar::Fetch = TabBar_Fetch;

signals:

public slots:
};

}  // namespace Nmemo

#endif // MEMOWIDGET_H
