/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <QtGlobal>
#include <QPair>
#include <QVariant>
#include <QString>
#include <QStringList>

/* enums */
enum class CmdSig {
  TAB = 0x01,
  BOOK = 0x02,
  ADD = 0x04,
  DELETE = 0x08,
  CHANGE = 0x10,
  MOVE = 0x20,
  RENAME = 0x40,
  SORT = 0x80,
  TAB_ADD = TAB | ADD,
  TAB_DELETE = TAB | DELETE,
  TAB_CHANGE = TAB | CHANGE,
  TAB_MOVE = TAB | MOVE,
  TAB_RENAME = TAB | RENAME,
  BOOK_ADD = BOOK | ADD,
  BOOK_DELETE = BOOK | DELETE,
  BOOK_CHANGE = BOOK | CHANGE,
  BOOK_MOVE = BOOK | MOVE,
  BOOK_RENAME = BOOK | RENAME,
  BOOK_SORT = BOOK | SORT,
  // utils
  ADD_RENAME = ADD | RENAME,
};

/* defines: common types */
using T_id = int;
using T_index = int;
using T_slist = QStringList;
using T_arg = QVariant;
using T_fname = QString;
using T_ext = QString;
using T_text = QString;
using T_name = QString;
using T_title = QString;
using T_caption = QString;
using T_filter = QString;
using T_ids = QList<T_id>;
using T_smap = QMap<T_id, QString>;
using T_lmap = QMap<T_id, QList<T_id>>;
using T_order = Qt::SortOrder;

/* defines: specific types */
using T_isUpdated = bool;
using T_cmd = CmdSig;
using T_tab_i = T_index;
using T_book_i = T_index;
using T_tid = T_id;
using T_bid = T_id;
using T_idset = QPair<T_tid, T_bid>;
using T_stat = bool;  // editor state(read only)
using T_stmap = QMap<T_id, T_stat>;
using T_tabnames = T_slist;
using T_booknames = T_slist;
using T_tids = T_ids;
using T_bids = T_ids;
using T_books = T_lmap;
using T_labels = T_smap;
using T_memo = T_text;
using T_memos = T_smap;

#endif // COMMON_TYPES_H
