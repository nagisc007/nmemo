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

signals:

public slots:
};

/* process */
namespace TabBar {

bool Exists(MemoWidget*);

namespace Index {

T_tab_i Fetch(MemoWidget*);
T_tab_i Merge(MemoWidget*, T_tab_i);

}  // ns TabBar::Index
namespace Names {

T_tabnames Fetch(MemoWidget*);
bool Merge(MemoWidget*, T_tabnames);

}  // ns TabBar::Names
namespace State {

bool Merge(MemoWidget*, T_stats);

}  // ns TabBar::State

}  // ns TabBar

namespace BookList {
namespace Index {

T_book_i Fetch(MemoWidget*);
T_book_i Merge(MemoWidget*, T_book_i);

}  // ns BookList::Index
namespace Names {

T_booknames Fetch(MemoWidget*);
bool Merge(MemoWidget*, T_booknames);

}  // ns BookList::Names
namespace Item {

T_item* Fetch(const MemoWidget*);

}  // ns BookList::Item
}  // ns BookList

namespace Editor {
namespace Act {

void Redo(MemoWidget*);
void Undo(MemoWidget*);
void Cut(MemoWidget*);
void Copy(MemoWidget*);
void Paste(MemoWidget*);
void Erase(MemoWidget*);
void SelectAll(MemoWidget*);

}  // ns Editor::Act
namespace State {

T_stat Fetch(MemoWidget*);
T_stat Merge(MemoWidget*, T_stat);

}  // ns Editor::State
namespace Text {

T_text Fetch(MemoWidget*);
T_text Merge(MemoWidget*, const T_text&);

}  // ns Editor::Text
}  // ns Editor

}  // namespace Nmemo

#endif // MEMOWIDGET_H
