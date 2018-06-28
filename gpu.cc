/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "gpu.h"

#include <QDebug>

namespace GPU {

namespace {

bool _IsExistsAddr(T_gpu_addr addr, T_gpu_addr target)
{
  return (static_cast<int>(addr) & static_cast<iny>(target)) != 0;
}

T_index _indexOf(T_ivec i)
{
  return i.size() > 0 ? i.at(0): -1;
}

T_str _strOf(T_strs s)
{
  return s.size() > 0 ? s.at(0): T_str();
}

T_states _statesOf(T_ivec i)
{
  if (i.size() <= 1) return T_states();

  auto fetched = i.mid(1);
  T_states res(fetched.size());
  int idx = 0;
  for (auto& v: fetched) {
    res[idx] = v;
    ++idx;
  }
  return res;
}

}  // inner global

// class: GPU::Core
Core::Core(QObject *parent) : QObject(parent)
{

}

// slots
void Core::FromCpu(T_gpu_addr addr, T_ivec ivec, T_strs strs)
{
  auto result = Result::SUCCESS;
  switch (addr) {
  case Addr::BOOKTAB:
    result = ToBookTabData(addr, _indexOf(ivec), strs, _statesOf(ivec));
    break;
  case Addr::EDITOR:
    result = ToEditorData(addr, false, _strOf(strs));
    break;
  case Addr::FILETAB:
    result = ToFileTabData(addr, _indexOf(ivec), strs, _statesOf(ivec));
    break;
  case Addr::FLUSH:
    result = ToFlushData();
    break;
  case Addr::PAGELIST:
    result = ToPageListData(addr, _indexOf(ivec), strs, _statesOf(ivec));
    break;
  case Addr::STATUSBAR:
    result = ToStatusBarData(addr, _strOf(strs));
    break;
  case Addr::TOOLBAR:
    result = ToToolBarData(addr);
    break;
  case Addr::WINDOW:
    result = ToWindowData(addr, _strOf(strs));
    break;
  default:
    break;
  }
  if (result != Result::SUCCESS)
    emit ToError(result);
}

void Core::FromIrq(T_gpu_irq irq)
{
  switch (irq) {
  case Irq::RESET: ToReset();
    break;
  case Irq::NOP:
    break;
  }
}

// streams
void Core::ToReset()
{
  vram.Reset();
}

T_gpu_result Core::ToFlushData()
{
  return Result::SUCCESS;
}

T_gpu_result Core::ToBookTabData(T_gpu_addr addr, int idx, T_strs labels, T_states states)
{
  if (_IsExistsAddr(addr, Addr::BOOKTAB_INDEX))
    if (!UpdateBookIndex(&vram, idx)) return Result::INVALID_INDEX;
  if (_IsExistsAddr(addr, Addr::BOOKTAB_LABELS))
    if (!UpdateBookLabels(&vram, labels)) return Result::INVALID_LABELS;
  if (_IsExistsAddr(addr, Addr::BOOKTAB_STATES))
    if (!UpdateBookStates(&vram, states)) return Result::INVALID_STATES;

  return Result::SUCCESS;
}

T_gpu_result Core::ToEditorData(T_gpu_addr addr, bool is_ro, const T_str& text)
{
  if (_IsExistsAddr(addr, Addr::EDITOR_READONLY))
    if (!UpdateEditorReadOnly(&vram, is_ro)) return Result::INVALID_VALUE;
  if (_IsExistsAddr(addr, Addr::EDITOR_TEXT))
    if (!UpdateEditorText(&vram, text)) return Result::INVALID_VALUE;

  return Result::SUCCESS;
}

T_gpu_result Core::ToFileTabData(T_gpu_addr addr, int idx, T_strs labels, T_states states)
{
  if (_IsExistsAddr(addr, Addr::FILETAB_INDEX))
    if (!UpdateFileIndex(&vram, idx)) return Result::INVALID_INDEX;
  if (_IsExistsAddr(addr, Addr::FILETAB_LABELS))
    if (!UpdateFileLabels(&vram, labels)) return Result::INVALID_LABELS;
  if (_IsExistsAddr(addr, Addr::FILETAB_STATES))
    if (!UpdateFileStates(&vram, states)) return Result::INVALID_STATES;

  return Result::SUCCESS;
}

T_gpu_result Core::ToPageListData(T_gpu_addr addr, int idx, T_strs labels, T_states states)
{
  if (_IsExistsAddr(addr, Addr::PAGELIST_INDEX))
    if (!UpdatePageIndex(&vram, idx)) return Result::INVALID_INDEX;
  if (_IsExistsAddr(addr, Addr::PAGELIST_LABELS))
    if (!UpdatePageLabels(&vram, labels)) return Result::INVALID_LABELS;
  if (_IsExistsAddr(addr, Addr::PAGELIST_STATES))
    if (!UpdatePageStates(&vram, states)) return Result::INVALID_STATES;

  return Result::SUCCESS;
}

T_gpu_result Core::ToStatusBarData(T_gpu_addr addr, const T_str& msg)
{
  if (_IsExistsAddr(addr, Addr::STATUS_MESSAGE))
    if (!UpdateStatusMessage(&vram, msg)) return Result::INVALID_VALUE;

  return Result::SUCCESS;
}

T_gpu_result Core::ToToolBarData(T_gpu_addr addr)
{
  Q_UNUSED(addr);
  // currently nothing
  return Result::SUCCESS;
}

T_gpu_result Core::ToWindowData(T_gpu_addr addr, const T_str& title)
{
  if (_IsExistsAddr(addr, Addr::WINDOW_TITLE))
    if (!UpdateWindowTitle(&vram, title)) return Result::INVALID_VALUE;

  return Result::SUCCESS;
}

}  // ns GPU
