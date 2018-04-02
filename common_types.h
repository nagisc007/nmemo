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
enum class Sig {// Control Signal
  NOP = 0x00,
  // types
  EDITOR = 0x01,
  LIST = 0x02,
  STATUS = 0x04,
  TAB = 0x08,
  TITLE = 0x10,
  // items
  INDEX = 0x0100,
  MESSAGE = 0x0200,
  MODE = 0x0400,
  NAME = 0x0800,
  POSITION = 0x1000,
  STATE = 0x2000,
  TEXT = 0x4000,
  TIME = 0x8000,
  // utils
  EDITOR_TEXT = EDITOR | TEXT,
  EDITOR_MODE = EDITOR | MODE,
  EDITOR_POS = EDITOR | POSITION,
  EDITOR_STATE = EDITOR | STATE,
  EDITOR_TEXT_MODE = EDITOR | TEXT | MODE,
  EDITOR_ALL = EDITOR | MODE | POSITION | TEXT | STATE,
  LIST_INDEX = LIST | INDEX,
  LIST_NAME = LIST | NAME,
  LIST_ALL = LIST | INDEX | NAME,
  STATUS_MESSAGE = STATUS | MESSAGE,
  TAB_INDEX = TAB | INDEX,
  TAB_NAME = TAB | NAME,
  TAB_STATE = TAB | STATE,
  TAB_ALL = TAB | INDEX | NAME | STATE,
  TITLE_TEXT = TITLE | TEXT,
};

enum class OpCode {
  NOP = 0x00,
  FILE_LOAD, FILE_SAVE, FILE_SAVEAS,
  BOOK_ADD, BOOK_CHANGE, BOOK_DELETE, BOOK_MOVE, BOOK_RENAME,
  PAGE_ADD, PAGE_CHANGE, PAGE_DELETE, PAGE_MOVE, PAGE_RENAME, PAGE_SORT,
  NOTE_ADD, NOTE_CACHE, NOTE_CHANGE_MODE, NOTE_DELETE, NOTE_MODIFY, NOTE_UPDATE,
};

enum class EditMode {
  PLAIN,
  HTML,
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
using T_pos = int;
using T_mode = EditMode;

/* defines: common types (container) */
using T_ids = QList<T_id>;
using T_strset = QHash<T_id, QString>;
using T_listset = QHash<T_id, QList<T_id>>;
using T_strs = QStringList;

/* defines: specific types */
using T_sig = Sig;
using T_code = OpCode;
using T_tab_i = T_index;
using T_book_i = T_index;
using T_page_i = T_index;
using T_bid = T_id;
using T_pid = T_id;
using T_idset = QPair<T_bid, T_pid>;
using T_stats = QList<T_stat>;
using T_statset = QHash<T_id, T_stat>;
using T_pathset = T_strset;
using T_tabnames = T_strs;
using T_pagenames = T_strs;
using T_bids = T_ids;
using T_pids = T_ids;
using T_pidset = QHash<T_bid, T_pid>;
using T_pidsset = T_listset;
using T_poss = QList<T_pos>;
using T_posset = QHash<T_pid, T_pos>;
using T_nameset = T_strset;
using T_modeset = QHash<T_pid, T_mode>;
using T_note = T_text;
using T_notes = T_strs;
using T_noteset = T_strset;
using T_encoded = T_strs;
using T_decoded = QPair<T_strs, T_strs>;

#endif // COMMON_TYPES_H
