/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <QBitArray>
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
  WINDOW_TITLE = 0x100 | WINDOW,
  STATUS_MESSAGE = 0x200 | STATUSBAR,
  FILETAB_LABELS = 0x400 | FILETAB,
  FILETAB_INDEX = 0x800 | FILETAB,
  FILETAB_STATES = 0x1000 | FILETAB,
  BOOKTAB_LABELS = 0x2000 | BOOKTAB,
  BOOKTAB_INDEX = 0x4000 | BOOKTAB,
  BOOKTAB_STATES = 0x8000 | BOOKTAB,
  PAGELIST_LABELS = 0x10000 | PAGELIST,
  PAGELIST_INDEX = 0x20000 | PAGELIST,
  PAGELIST_STATES = 0x40000 | PAGELIST,
  EDITOR_TEXT = 0x80000 | EDITOR,
  EDITOR_READONLY = 0x100000 | EDITOR,
};

}  // ns DEV

namespace CPU {

enum class Addr {
  NOP,
  FILE_NEW,
  FILE_OPEN,
  FILE_CLOSE,
  FILE_CHANGE,
  FILE_SAVE,
  FILE_SAVEAS,
  FILE_MOVE,
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

enum class Irq {
  NOP,
  RESET,
};

enum class Result {
  SUCCESS,
  INVALID_FILEID,
  INVALID_FILEINDEX,
  INVALID_BOOKID,
  INVALID_BOOKINDEX,
  INVALID_PAGEID,
  INVALID_PAGEINDEX,
  INVALID_NAME,
  INVALID_PATH,
  INVALID_OPERATION,
  ERR_NOTOPEN_FILE,
  ERR_FILE_VERSION_MISMATCH,
  ERR_FILE_DATAHEAD,
  ERR_UNKNOWN,
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
  FLUSH = 0x80,
  WINDOW_TITLE = 0x100 | WINDOW,
  STATUS_MESSAGE = 0x200 | STATUSBAR,
  FILETAB_LABELS = 0x400 | FILETAB,
  FILETAB_INDEX = 0x800 | FILETAB,
  FILETAB_STATES = 0x1000 | FILETAB,
  BOOKTAB_LABELS = 0x2000 | BOOKTAB,
  BOOKTAB_INDEX = 0x4000 | BOOKTAB,
  BOOKTAB_STATES = 0x8000 | BOOKTAB,
  PAGELIST_LABELS = 0x10000 | PAGELIST,
  PAGELIST_INDEX = 0x20000 | PAGELIST,
  PAGELIST_STATES = 0x40000 | PAGELIST,
  EDITOR_TEXT = 0x80000 | EDITOR,
  EDITOR_READONLY = 0x100000 | EDITOR,
  WINDOW_ALL = WINDOW_TITLE,
  FILETAB_ALL = FILETAB_LABELS | FILETAB_INDEX | FILETAB_STATES,
  BOOKTAB_ALL = BOOKTAB_LABELS | BOOKTAB_INDEX | BOOKTAB_STATES,
  PAGELIST_ALL = PAGELIST_LABELS | PAGELIST_INDEX | PAGELIST_STATES,
  EDITOR_ALL = EDITOR_TEXT | EDITOR_READONLY,
  STATUSBAR_ALL = STATUS_MESSAGE,
};

enum class Irq {
  NOP,
  RESET,
};

enum class Result {
  SUCCESS,
  INVALID_INDEX,
  INVALID_LABELS,
  INVALID_STATES,
  INVALID_VALUE,
  INVALID_OPERATION,
};

}  // ns GPU

// common types
using T_arg = QVariant;
using T_cpu_addr = CPU::Addr;
using T_cpu_irq = CPU::Irq;
using T_cpu_result = CPU::Result;
using T_dev_addr = DEV::Addr;
using T_gpu_addr = GPU::Addr;
using T_gpu_irq = GPU::Irq;
using T_gpu_result = GPU::Result;
using T_id = int;
using T_ids = QVector<T_id>;
using T_index = int;
using T_item = QListWidgetItem;
using T_ivec = QVector<int>;
using T_order = Qt::SortOrder;
using T_states = QBitArray;
using T_str = QString;
using T_strs = QStringList;

#endif // COMMON_TYPES_H
