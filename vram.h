/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef VRAM_H
#define VRAM_H

#include "common_types.h"
#include "common_values.h"

namespace GPU {

struct Vram
{
  Vram();
  // members
  QVector<bool> breg;
  QVector<int> ireg;
  T_strs sreg;
  T_strs file_labels;
  T_strs book_labels;
  T_strs page_labels;
  T_states file_states;
  T_states book_states;
  T_states page_states;
  // methods
  void Reset();
};

enum class BRegAddr {
  FILE_INDEX,
  FILE_LABELS,
  FILE_STATES,
  BOOK_INDEX,
  BOOK_LABELS,
  BOOK_STATES,
  PAGE_INDEX,
  PAGE_LABELS,
  PAGE_STATES,
  EDITOR_TEXT,
  EDITOR_READONLY,
  WINDOW_TITLE,
  STATUS_MESSAGE,
};
const int BREG_SIZE = 13;

enum class IRegAddr {
  FILE_INDEX,
  BOOK_INDEX,
  PAGE_INDEX,
  TEXT_READONLY,
};
const int IREG_SIZE = 4;

enum class SRegAddr {
  TITLE,
  MESSAGE,
  TEXT,
};
const int SREG_SIZE = 3;

// controls
bool UpdateFileIndex(Vram*, T_index);
bool UpdateBookIndex(Vram*, T_index);
bool UpdatePageIndex(Vram*, T_index);
bool UpdateEditorReadOnly(Vram*, bool);
bool UpdateEditorText(Vram*, const T_str&);
bool UpdateWindowTitle(Vram*, const T_str&);
bool UpdateStatusMessage(Vram*, const T_str&);
bool UpdateFileLabels(Vram*, T_strs);
bool UpdateBookLabels(Vram*, T_strs);
bool UpdatePageLabels(Vram*, T_strs);
bool UpdateFileStates(Vram*, T_states);
bool UpdateBookStates(Vram*, T_states);
bool UpdatePageStates(Vram*, T_states);

}  // ns GPU

#endif // VRAM_H
