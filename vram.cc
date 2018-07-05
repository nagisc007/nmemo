/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "vram.h"

#include <QDebug>

namespace GPU {

// struct: Vram
Vram::Vram():
  breg(T_states(BREG_SIZE)),
  ireg(QVector<int>(IREG_SIZE)),
  sreg(T_strs())
{
  sreg.reserve(SREG_SIZE);
  for (int i = 0; i < SREG_SIZE; ++i)
    sreg << T_str();
}

void Vram::Reset()
{
  breg.fill(false);
  for (auto& v: ireg) v = -1;
  for (auto& v: sreg) v = T_str();
  file_labels.clear();
  book_labels.clear();
  page_labels.clear();
  file_states.clear();
  book_states.clear();
  page_states.clear();
}

// controls
void ResetBReg(Vram* vram)
{
  vram->breg.fill(false);
}

bool UpdateBookIndex(Vram* vram, T_index idx)
{
  vram->ireg[static_cast<int>(IRegAddr::BOOK_INDEX)] = idx;
  vram->breg.setBit(static_cast<int>(BRegAddr::BOOK_INDEX));
  return true;
}

bool UpdateBookLabels(Vram* vram, T_strs labels)
{
  vram->book_labels = labels;
  vram->breg.setBit(static_cast<int>(BRegAddr::BOOK_LABELS));
  return true;
}

bool UpdateBookStates(Vram* vram, T_states states)
{
  vram->book_states = states;
  vram->breg.setBit(static_cast<int>(BRegAddr::BOOK_STATES));
  return true;
}

bool UpdateEditorReadOnly(Vram* vram, bool is_ro)
{
  vram->ireg[static_cast<int>(IRegAddr::TEXT_READONLY)] = is_ro;
  vram->breg.setBit(static_cast<int>(BRegAddr::EDITOR_READONLY));
  return true;
}

bool UpdateEditorText(Vram* vram, const T_str& text)
{
  vram->sreg[static_cast<int>(SRegAddr::TEXT)] = text;
  vram->breg.setBit(static_cast<int>(BRegAddr::EDITOR_TEXT));
  return true;
}

bool UpdateFileIndex(Vram* vram, T_index idx)
{
  vram->ireg[static_cast<int>(IRegAddr::FILE_INDEX)] = idx;
  vram->breg.setBit(static_cast<int>(BRegAddr::FILE_INDEX));
  return true;
}

bool UpdateFileLabels(Vram* vram, T_strs labels)
{
  vram->file_labels = labels;
  vram->breg.setBit(static_cast<int>(BRegAddr::FILE_LABELS));
  return true;
}

bool UpdateFileStates(Vram* vram, T_states states)
{
  vram->file_states = states;
  vram->breg.setBit(static_cast<int>(BRegAddr::FILE_STATES));
  return true;
}

bool UpdatePageIndex(Vram* vram, T_index idx)
{
  vram->ireg[static_cast<int>(IRegAddr::PAGE_INDEX)] = idx;
  vram->breg.setBit(static_cast<int>(BRegAddr::PAGE_INDEX));
  return true;
}

bool UpdatePageLabels(Vram* vram, T_strs labels)
{
  vram->page_labels = labels;
  vram->breg.setBit(static_cast<int>(BRegAddr::PAGE_LABELS));
  return true;
}

bool UpdatePageStates(Vram* vram, T_states states)
{
  vram->page_states = states;
  vram->breg.setBit(static_cast<int>(BRegAddr::PAGE_STATES));
  return true;
}

bool UpdateStatusMessage(Vram* vram, const T_str& msg)
{
  vram->sreg[static_cast<int>(SRegAddr::MESSAGE)] = msg;
  vram->breg.setBit(static_cast<int>(BRegAddr::STATUS_MESSAGE));
  return true;
}

bool UpdateWindowTitle(Vram* vram, const T_str& title)
{
  vram->sreg[static_cast<int>(SRegAddr::TITLE)] = title;
  vram->breg.setBit(static_cast<int>(BRegAddr::WINDOW_TITLE));
  return true;
}

}  // ns GPU
