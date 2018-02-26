/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "utils.h"

namespace Utl {

/* values */
int next_id = 0;
QStack<int> id_pool;

/* utils: bits */
auto hasCmd::operator ()(CmdSig a, CmdSig b) -> bool
{
  return (static_cast<int>(a) & static_cast<int>(b)) != 0;
}

/* utils: for id */
auto GetId::operator ()() -> int
{
  return id_pool.isEmpty() ? ++next_id: id_pool.pop();
}

auto ReleaseId::operator ()(int released) -> int
{
  id_pool.push(released);
  return released;
}

/* utils: QListWidget */
auto OverrideListWidget::operator ()(QListWidget* list, const QStringList& slist) -> void
{
  // NOTE:
  //  The ListWidget bug to remove and add.
  //  If the reason is found, fix it.
  list->clear();
  list->addItems(slist);
}

/* utils: QTabBar */
auto OverrideTabBar::operator ()(QTabBar* tab, const QStringList& slist) -> void
{
  auto tab_size = tab->count();
  auto slist_size = slist.count();
  int i = 0;
  for (int size = qMin(tab_size, slist_size); i < size; ++i) {
    tab->setTabText(i, slist.at(i));
  }
  if (tab_size > slist_size) {
    for (; i < tab_size; ++i) {
      tab->removeTab(i);
    }
  } else {
    for (; i < slist_size; ++i) {
      tab->addTab(slist.at(i));
    }
  }
}

/* operation: for tab */
auto tabIdFrom::operator ()(CmdSig cmd, const T_ids* tabs, int index) -> int
{
  return index >= 0 && tabs->count() > 0 && index < tabs->count() ?
        hasCmd()(cmd, CmdSig::TAB_DELETE) ?
          index - 1 < tabs->count() ?
            tabs->at(index - 1):
            -1:
            tabs->at(index):
          -1;
}

auto GetTabIdToRead::operator ()(CmdSig cmd, const T_ids* tabs, int index, int tid) -> int
{
  return hasCmd()(cmd, CmdSig::TAB) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          GetId()():
          tabIdFrom()(cmd, tabs, index):
          tid;
}

auto GetTabIdToWrite::operator ()(CmdSig cmd, const T_ids* tabs, int index, int tid) -> int
{
  return hasCmd()(cmd, CmdSig::TAB) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          tid:
          hasCmd()(cmd, CmdSig::DELETE) ?
            tabIdFrom()(cmd, tabs, index):
            -1:
            -1;
}

auto GetTabNameToWrite::operator ()(CmdSig cmd, QVariant arg) -> QString
{
  return hasCmd()(cmd, CmdSig::TAB) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          QString("Undefined"):
          hasCmd()(cmd, CmdSig::RENAME) ?
            arg.toString():
            QString(""):
            QString("");
}

/* operation: for book */
auto bookIdFrom::operator ()(CmdSig cmd, const T_idpack* books,
                             int tid, int index) -> int
{
  return index >= 0 && books->contains(tid) &&
      books->value(tid).count() > 0 && index < books->value(tid).count() ?
        hasCmd()(cmd, CmdSig::BOOK_DELETE) ?
          index - 1 < books->value(tid).count() ?
            books->value(tid).at(index - 1):
            -1:
            books->value(tid).at(index):
            -1;
}

auto GetBookIdToRead::operator ()(CmdSig cmd, const T_idpack* books,
                                  int tid, int index, int book_i) -> int
{
  return hasCmd()(cmd, CmdSig::BOOK) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          GetId()():
          bookIdFrom()(cmd, books, tid, index):
          bookIdFrom()(cmd, books, tid, book_i);
}

auto GetBookIdToWrite::operator ()(CmdSig cmd, int bid) -> int
{
  return hasCmd()(cmd, CmdSig::BOOK) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          bid:
          -1:
          -1;
}

auto GetBookNameToWrite::operator ()(CmdSig cmd, QVariant arg) -> QString
{
  return hasCmd()(cmd, CmdSig::BOOK) ?
        hasCmd()(cmd, CmdSig::ADD_RENAME) ?
          arg.toString():
          QString(""):
          QString("");
}

}  // namespace Utl

