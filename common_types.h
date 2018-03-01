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
#include <QStringList>

enum class CmdSig {
  TAB = 0x01,
  BOOK = 0x02,
  ADD = 0x04,
  DELETE = 0x08,
  CHANGE = 0x10,
  MOVE = 0x20,
  RENAME = 0x40,
  SET = 0x80,
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
  BOOK_SET = BOOK | SET | ADD,
  // utils
  ADD_RENAME = ADD | RENAME,
};

/* defines: common types */
using T_id = int;
using T_index = int;
using T_slist = QStringList;
using T_arg = QVariant;
using T_fname = QString;
using T_text = QString;
using T_ids = QList<T_id>;
using T_strmap = QMap<T_id, QString>;
using T_idmap = QMap<T_id, T_ids>;

/* defines: specific types */
using T_cmd = CmdSig;
using T_tab_i = T_index;
using T_book_i = T_index;
using T_tid = T_id;
using T_bid = T_id;
using T_idset = QPair<T_tid, T_bid>;
using T_stat = bool;  // editor state
using T_tabnames = T_slist;
using T_booknames = T_slist;
using T_tabs = T_ids;
using T_paths = T_strmap;
using T_book = T_ids;
using T_books = QMap<T_tid, T_book>;
using T_labels = T_strmap;
using T_memo = T_text;
using T_memos = T_strmap;

#endif // COMMON_TYPES_H
