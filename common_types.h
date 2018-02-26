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
};

using T_labels = QStringList;

#endif // COMMON_TYPES_H
