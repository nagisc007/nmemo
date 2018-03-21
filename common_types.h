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
  // Operand (upper 16bit)
  FILE = 0x000010000,
  BOOK = 0x000020000,
  PAGE = 0x00040000,
  NOTE = 0x00080000,
  TAB = 0x00100000,
  LIST = 0x00200000,
  EDITOR = 0x00400000,
  TITLE = 0x00800000,
  STATUS = 0x01000000,
  ID = 0x02000000,
  INDEX = 0x04000000,
  NAME = 0x08000000,
  TEXT = 0x10000000,
  STATE = 0x20000000,
  TIME = 0x40000000,
  // Opcode (lower 16bit)
  ADD = 0x0001,
  DELETE = 0x0002,
  CHANGE = 0x0004,
  MOVE = 0x0008,
  EDIT = 0x0010,
  SORT = 0x0020,
  LOAD = 0x0040,
  SAVE = 0x0080,
  // opcodes
  NOP = 0x00,
  BOOK_ADD = BOOK | ADD,
  BOOK_DELETE = BOOK | DELETE,
  BOOK_CHANGE = BOOK | CHANGE,
  BOOK_MOVE = BOOK | MOVE,
  BOOK_EDIT = BOOK | EDIT,
  PAGE_ADD = PAGE | ADD,
  PAGE_DELETE = PAGE | DELETE,
  PAGE_CHANGE = PAGE | CHANGE,
  PAGE_MOVE = PAGE | MOVE,
  PAGE_EDIT = PAGE | EDIT,
  PAGE_SORT = PAGE | SORT,
  NOTE_ADD = NOTE | ADD,
  NOTE_DELETE = NOTE | DELETE,
  NOTE_EDIT = NOTE | EDIT,
  NOTE_MODIFY = NOTE | CHANGE,
  FILE_LOAD = FILE | LOAD,
  FILE_SAVE = FILE | SAVE,
  FILE_SAVEAS = FILE | SAVE | NAME,
  TAB_NAME = TAB | NAME,
  TAB_INDEX = TAB | INDEX,
  TAB_STATUS = TAB | STATUS,
  TAB_ALL = TAB | NAME | INDEX | STATUS,
  LIST_NAME = LIST | NAME,
  LIST_INDEX = LIST | INDEX,
  LIST_ALL = LIST | NAME | INDEX,
  EDITOR_TEXT = EDITOR | TEXT,
  EDITOR_STATE = EDITOR | STATE,
  EDITOR_ALL = EDITOR | TEXT | STATE,
  TITLE_NAME = TITLE | NAME,
  STATUS_TEXT = STATUS | TEXT,
  STATUS_TIME = STATUS | TIME,
  STATUS_ALL = STATUS | TEXT | TIME,
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
using T_color = QColor;

/* defines: common types (container) */
using T_ids = QList<T_id>;
using T_strset = QMap<T_id, QString>;
using T_listset = QMap<T_id, QList<T_id>>;
using T_strs = QStringList;

/* defines: specific types */
using T_cmd = Cmd;
using T_tab_i = T_index;
using T_book_i = T_index;
using T_page_i = T_index;
using T_bid = T_id;
using T_pid = T_id;
using T_idset = QPair<T_bid, T_pid>;
using T_stats = QList<T_stat>;
using T_statset = QMap<T_id, T_stat>;
using T_pathset = T_strset;
using T_tabnames = T_strs;
using T_pagenames = T_strs;
using T_bids = T_ids;
using T_pids = T_ids;
using T_pidset = QMap<T_bid, T_pid>;
using T_pidsset = T_listset;
using T_nameset = T_strset;
using T_note = T_text;
using T_notes = T_strs;
using T_noteset = T_strset;
using T_encoded = T_strs;
using T_decoded = QPair<T_strs, T_strs>;

#endif // COMMON_TYPES_H
