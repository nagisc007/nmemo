/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <QColor>
#include <QListWidgetItem>
#include <QPair>
#include <QString>
#include <QStringList>
#include <QtGlobal>
#include <QVariant>
#include <QVector>

namespace DEV {

enum class Addr {
  NOP = 0x00,
  WINDOW = 0x01,
  TOOLBAR = 0x02,
  STATUSBAR = 0x04,
  FILETAB = 0x08,
  BOOKTAB = 0x10,
  PAGELIST = 0x20,
  EDITOR = 0x40,
  TITLE = 0x100,
  INDEX = 0x200,
  LABELS = 0x400,
  STATES = 0x800,
  TEXT = 0x1000,
  READONLY = 0x2000,
  WINDOW_TITLE = TITLE | WINDOW,
  FILETAB_LABELS = LABELS | FILETAB,
  FILETAB_INDEX = INDEX | FILETAB,
  FILETAB_STATES = STATES | FILETAB,
  BOOKTAB_LABELS = LABELS | BOOKTAB,
  BOOKTAB_INDEX = INDEX | BOOKTAB,
  BOOKTAB_STATES = STATES | BOOKTAB,
  PAGELIST_LABELS = LABELS | PAGELIST,
  PAGELIST_INDEX = INDEX | PAGELIST,
  PAGELIST_STATES = STATES | PAGELIST,
  EDITOR_TEXT = TEXT | EDITOR,
  EDITOR_READONLY = READONLY | EDITOR,
};

}  // ns DEV

namespace CPU {

enum class Addr {
  NOP,
  FILE_OPEN,
  FILE_CLOSE,
  FILE_CHANGE,
  FILE_SAVE,
  FILE_SAVEAS,
  FILE_RENAME,
  BOOK_ADD,
  BOOK_DELETE,
  BOOK_CHANGE,
  BOOK_MOVE,
  BOOK_RENAME,
  BOOK_SORT,
  PAGE_ADD,
  PAGE_DELETE,
  PAGE_CHANGE,
  PAGE_MOVE,
  PAGE_RENAME,
  PAGE_SORT,
  TEXT_MODIFY,
};

}  // ns CPU

namespace GPU {

enum class Addr {
  NOP = 0x00,
  WINDOW = 0x01,
  TOOLBAR = 0x02,
  STATUSBAR = 0x04,
  FILETAB = 0x08,
  BOOKTAB = 0x10,
  PAGELIST = 0x20,
  EDITOR = 0x40,
  TITLE = 0x100,
  INDEX = 0x200,
  LABELS = 0x400,
  STATES = 0x800,
  TEXT = 0x1000,
  READONLY = 0x2000,
  WINDOW_TITLE = TITLE | WINDOW,
  FILETAB_LABELS = LABELS | FILETAB,
  FILETAB_INDEX = INDEX | FILETAB,
  FILETAB_STATES = STATES | FILETAB,
  BOOKTAB_LABELS = LABELS | BOOKTAB,
  BOOKTAB_INDEX = INDEX | BOOKTAB,
  BOOKTAB_STATES = STATES | BOOKTAB,
  PAGELIST_LABELS = LABELS | PAGELIST,
  PAGELIST_INDEX = INDEX | PAGELIST,
  PAGELIST_STATES = STATES | PAGELIST,
  EDITOR_TEXT = TEXT | EDITOR,
  EDITOR_READONLY = READONLY | EDITOR,
};

}  // ns GPU

// common types
using T_arg = QVariant;
using T_id = int;
using T_index = int;
using T_ivec = QVector<int>;
using T_order = Qt::SortOrder;
using T_str = QString;
using T_strs = QStringList;

#endif // COMMON_TYPES_H
