/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "memowidget.h"

namespace Nmemo {

/* class: MemoWidget */
MemoWidget::MemoWidget(QWidget *parent) : QWidget(parent),
  booklist(new QListWidget(this)),
  editor(new QTextEdit(this)),
  tabbar(new QTabBar(this))
{
  // props
  tabbar->setMovable(true);
  tabbar->setTabsClosable(true);
  booklist->setMaximumWidth(PROP::BOOKLIST_MAX_WIDTH);
  editor->setReadOnly(true);
  editor->setTabStopDistance(PROP::EDIT_TAB_DISTANCE);
  // layout
  auto mainLayout = new QVBoxLayout();
  auto subLayout = new QHBoxLayout();
  subLayout->addWidget(editor.data());
  subLayout->addWidget(booklist.data());
  mainLayout->addWidget(tabbar.data());
  mainLayout->addLayout(subLayout);
  setLayout(mainLayout);
  qDebug() << "MemoWidget: constructed";
}

MemoWidget::~MemoWidget()
{
  if (booklist) {
    booklist->clear();
    booklist.reset();
  }
  if (editor) editor.reset();
  if (tabbar) tabbar.reset();
  qDebug() << "MemoWidget: destructed";
}

/* process: tabbar */
namespace TabBar {

auto Exists(MemoWidget* m) -> bool
{
  return m->tabbar->count() > 0;
}

namespace Index {

auto Fetch(MemoWidget* m) -> T_tab_i
{
  return m->tabbar->currentIndex();
}

auto Merge(MemoWidget* m, T_tab_i tab_i) -> T_tab_i
{
  m->tabbar->setCurrentIndex(tab_i);
  return m->tabbar->currentIndex();
}

}  // ns TabBar::Index
namespace Names {

auto Fetch(MemoWidget* m) -> T_tabnames
{
  T_strs tmp;
  for (int i = 0, size = m->tabbar->count(); i < size; ++i) {
    tmp << m->tabbar->tabText(i);
  }
  return tmp;
}

auto Merge(MemoWidget* m, T_tabnames tabnames) -> bool
{
  if (m->tabbar->count() <= 0 && tabnames.count() <= 0) return false;
  Utl::Widget::Names::Merge<QTabBar>(m->tabbar.data(), &tabnames);
  return true;
}

}  // ns TabBar::Names
namespace State {

auto Merge(MemoWidget* m, T_stats stats) -> bool
{
  if (m->tabbar->count() != stats.count()) return false;
  for (int i = 0, size = m->tabbar->count(); i < size; ++i) {
    m->tabbar->setTabTextColor(i, stats.at(i) ? QColor(0,0,0): QColor(255,0,0));
  }
  return true;
}

}  // ns TabBar::State
}  // ns TabBar

/* process: booklist */
namespace BookList {
namespace Index {

auto Fetch(MemoWidget* m) -> T_book_i
{
  return m->booklist->currentRow();
}

auto Merge(MemoWidget* m, T_book_i book_i) -> T_book_i
{
  m->booklist->setCurrentRow(book_i);
  return m->booklist->currentRow();
}

}  // ns BookList::Index
namespace Names {

auto Fetch(MemoWidget* m) -> T_booknames
{
  T_strs tmp;
  for (int i = 0, size = m->booklist->count(); i < size; ++i) {
    auto item = m->booklist->item(i);
    if (item) tmp << item->text();
  }
  return tmp;
}

auto Merge(MemoWidget* m, T_booknames booknames) -> bool
{
  if (m->booklist->count() <= 0 && booknames.count() <= 0) return false;
  Utl::Widget::Names::Merge<QListWidget>(m->booklist.data(), &booknames);
  return true;
}

}  // ns BookList::Names
namespace Item {

auto Fetch(const MemoWidget* m) -> T_item*
{
  return m->booklist->currentItem();
}

}  // ns BookList::Item
}  // ns BookList

/* process: editor */
namespace Editor {

namespace Act {

auto Undo(MemoWidget* m) -> void
{
  if (!m->editor->isReadOnly()) m->editor->undo();
}

auto Redo(MemoWidget* m) -> void
{
  if (!m->editor->isReadOnly()) m->editor->redo();
}

auto Cut(MemoWidget* m) -> void
{
  if (!m->editor->isReadOnly()) m->editor->cut();
}

auto Copy(MemoWidget* m) -> void
{
  if (!m->editor->isReadOnly()) m->editor->copy();
}

auto Paste(MemoWidget* m) -> void
{
  if (!m->editor->isReadOnly()) m->editor->paste();
}

auto Erase(MemoWidget* m) -> void
{
  if (!m->editor->isReadOnly()) m->editor->textCursor().removeSelectedText();
}

auto SelectAll(MemoWidget* m) -> void
{
  if (!m->editor->isReadOnly()) m->editor->selectAll();
}

}  // ns Editor::Act

namespace State {

auto Fetch(MemoWidget* m) -> T_stat
{
  return m->editor->isReadOnly();
}

auto Merge(MemoWidget* m, T_stat stat) -> T_stat
{
  m->editor->setReadOnly(stat);
  return m->editor->isReadOnly();
}

}  // ns Editor::State

namespace Text {

auto Fetch(MemoWidget* m) -> T_text
{
  return m->editor->toPlainText();
}

auto Merge(MemoWidget* m, const T_text& text) -> T_text
{
  m->editor->setText(text);
  return m->editor->toPlainText();
}

}  // ns Editor::Text
}  // ns Editor

}  // namespace Nmemo
