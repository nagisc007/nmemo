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
#include <QListWidgetItem>

/* enums */
enum class Cmd {
  // head XX000000
  TAB = 0x01000000,
  BOOK = 0x02000000,
  MEMO = 0x04000000,
  FILE = 0x08000000,
  STATUS = 0x10000000,
  // operator 0000XXXX
  ADD = 0x0001,
  DELETE = 0x0002,
  CHANGE = 0x0004,
  MOVE = 0x0008,
  EDIT = 0x0010,
  RENAME = 0x0020,
  SORT = 0x0040,
  MERGE = 0x0080,
  LOAD = 0x0100,
  SAVE = 0x0200,
  SAVEAS = 0x0400,
  // operand 00XX0000
  INDEX = 0x00010000,
  ID = 0x00020000,
  NAME = 0x00040000,
  NAMES = 0x00080000,
  // opcodes
  TAB_ADD = TAB | ADD,
  TAB_DELETE = TAB | DELETE,
  TAB_CHANGE = TAB | CHANGE,
  TAB_MOVE = TAB | MOVE,
  TAB_RENAME = TAB | RENAME,
  TAB_MERGE = TAB | MERGE,
  BOOK_ADD = BOOK | ADD,
  BOOK_DELETE = BOOK | DELETE,
  BOOK_CHANGE = BOOK | CHANGE,
  BOOK_MOVE = BOOK | MOVE,
  BOOK_RENAME = BOOK | RENAME,
  BOOK_SORT = BOOK | SORT,
  BOOK_MERGE = BOOK | MERGE,
  FILE_LOAD = FILE | LOAD,
  FILE_SAVE = FILE | SAVE,
  FILE_SAVEAS = FILE | SAVEAS,
  INDEX_ONLY = INDEX,
  INDEX_NAMES = INDEX | NAMES,
};

/* defines: common (base) */
using T_id = int;
using T_index = int;
using T_stat = bool;
using T_text = QString;

/* defines: common types */
using T_arg = QVariant;
using T_caption = T_text;
using T_ext = T_text;
using T_filter = T_text;
using T_item = QListWidgetItem;
using T_msg = T_text;
using T_msgtime = int;
using T_name = T_text;
using T_dirname = T_text;
using T_order = Qt::SortOrder;
using T_path = T_text;  // =filename
using T_title = T_text;

/* defines: common types (container) */
using T_ids = QList<T_id>;
using T_strset = QMap<T_id, QString>;
using T_listset = QMap<T_id, QList<T_id>>;
using T_strs = QStringList;

/* defines: specific types */
using T_cmd = Cmd;
using T_tab_i = T_index;
using T_book_i = T_index;
using T_tid = T_id;
using T_bid = T_id;
using T_idset = QPair<T_tid, T_bid>;
using T_stats = QList<T_stat>;
using T_statset = QMap<T_id, T_stat>;
using T_tabnames = T_strs;
using T_booknames = T_strs;
using T_tids = T_ids;
using T_bids = T_ids;
using T_bidset = QMap<T_tid, T_bid>;
using T_bidsset = T_listset;
using T_labels = T_strset;
using T_memo = T_text;
using T_memos = T_strset;

#endif // COMMON_TYPES_H
