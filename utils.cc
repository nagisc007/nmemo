/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "utils.h"

namespace Utl {

/* utils: bits */
auto hasCmd::operator ()(CmdSig a, CmdSig b) -> bool
{
  return static_cast<int>(a) & static_cast<int>(b) != 0;
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

}  // namespace Utl

