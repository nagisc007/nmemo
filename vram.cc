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
  ireg(QVector<int>(IREG_SIZE)),
  sreg(T_strs(SREG_SIZE))
{

}

void Vram::Reset()
{
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
bool UpdateBookIndex(Vram* vram, T_index idx)
{
  vram->ireg[static_cast<int>(IRegAddr::BOOK_INDEX)] = idx;
  return true;
}

bool UpdateBookLabels(Vram* vram, T_strs labels)
{
  vram->book_labels = labels;
  return true;
}

bool UpdateBookStates(Vram* vram, T_states states)
{
  vram->book_states = states;
  return true;
}

bool UpdateEditorReadOnly(Vram* vram, bool is_ro)
{
  vram->ireg[static_cast<int>(IRegAddr::TEXT_READONLY)] = is_ro;
  return true;
}

bool UpdateEditorText(Vram* vram, const T_str& text)
{
  vram->sreg[static_cast<int>(SRegAddr::TEXT)] = text;
  return true;
}

bool UpdateFileIndex(Vram* vram, T_index idx)
{
  vram->ireg[static_cast<int>(IRegAddr::FILE_INDEX)] = idx;
  return true;
}

bool UpdateFileLabels(Vram* vram, T_strs labels)
{
  vram->file_labels = labels;
  return true;
}

bool UpdateFileStates(Vram* vram, T_states states)
{
  vram->file_states = states;
  return true;
}

bool UpdatePageIndex(Vram* vram, T_index idx)
{
  vram->ireg[static_cast<int>(IRegAddr::PAGE_INDEX)] = idx;
  return true;
}

bool UpdatePageLabels(Vram* vram, T_strs labels)
{
  vram->page_labels = labels;
  return true;
}

bool UpdatePageStates(Vram* vram, T_states states)
{
  vram->page_states = states;
  return true;
}

bool UpdateStatusMessage(Vram* vram, const T_str& msg)
{
  vram->sreg[static_cast<int>(SRegAddr::MESSAGE)] = msg;
  return true;
}

bool UpdateWindowTitle(Vram* vram, const T_str& title)
{
  vram->sreg[static_cast<int>(SRegAddr::TITLE)] = title;
  return true;
}

}  // ns GPU
