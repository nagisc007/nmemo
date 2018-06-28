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

}  // ns GPU
