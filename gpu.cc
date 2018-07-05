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
  return (static_cast<int>(addr) & static_cast<int>(target)) != 0;
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

  if (_IsExistsAddr(addr, Addr::FLUSH)) {
    result = ToFlushData();
    if (result == Result::SUCCESS) {
      ResetBReg(&vram);
    } else {
      emit ToError(result);
    }
  }
  if (_IsExistsAddr(addr, Addr::BOOKTAB)) {
    result = ToBookTabData(addr, _indexOf(ivec), strs, _statesOf(ivec));
    if (result != Result::SUCCESS) emit ToError(result);
  }
  if (_IsExistsAddr(addr, Addr::EDITOR)) {
    result = ToEditorData(addr, _indexOf(ivec), _strOf(strs));
    if (result != Result::SUCCESS) emit ToError(result);
  }
  if (_IsExistsAddr(addr, Addr::FILETAB)) {
    result = ToFileTabData(addr, _indexOf(ivec), strs, _statesOf(ivec));
    if (result != Result::SUCCESS) emit ToError(result);
  }
  if (_IsExistsAddr(addr, Addr::PAGELIST)) {
    result = ToPageListData(addr, _indexOf(ivec), strs, _statesOf(ivec));
    if (result != Result::SUCCESS) emit ToError(result);
  }
  if (_IsExistsAddr(addr, Addr::STATUSBAR)) {
    result = ToStatusBarData(addr, _strOf(strs));
    if (result != Result::SUCCESS) emit ToError(result);
  }
  if (_IsExistsAddr(addr, Addr::TOOLBAR)) {
    result = ToToolBarData(addr);
    if (result != Result::SUCCESS) emit ToError(result);
  }
  if (_IsExistsAddr(addr, Addr::WINDOW)) {
    result = ToWindowData(addr, _strOf(strs));
    if (result != Result::SUCCESS) emit ToError(result);
  }

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
  int submits = static_cast<int>(DEV::Addr::NOP);
  T_ivec ivec;
  T_strs strs;
  T_states states;

  // filetab
  if (vram.breg.at(static_cast<int>(BRegAddr::FILE_LABELS))) {
    strs = vram.file_labels;
    submits |= static_cast<int>(DEV::Addr::FILETAB_LABELS);
  }
  if (vram.breg.at(static_cast<int>(BRegAddr::FILE_INDEX))) {
    ivec.append(vram.ireg.at(static_cast<int>(IRegAddr::FILE_INDEX)));
    submits |= static_cast<int>(DEV::Addr::FILETAB_INDEX);
  }
  if (vram.breg.at(static_cast<int>(BRegAddr::FILE_STATES))) {
    states = vram.file_states;
    submits |= static_cast<int>(DEV::Addr::FILETAB_STATES);
  }
  if (submits > 0) {
    emit ToDev(static_cast<T_dev_addr>(submits), ivec, strs, states);
  }
  submits = static_cast<int>(DEV::Addr::NOP);
  ivec.clear();
  strs.clear();
  states.clear();

  // booktab
  if (vram.breg.at(static_cast<int>(BRegAddr::BOOK_LABELS))) {
    strs = vram.book_labels;
    submits |= static_cast<int>(DEV::Addr::BOOKTAB_LABELS);
  }
  if (vram.breg.at(static_cast<int>(BRegAddr::BOOK_INDEX))) {
    ivec.append(vram.ireg.at(static_cast<int>(IRegAddr::BOOK_INDEX)));
    submits |= static_cast<int>(DEV::Addr::BOOKTAB_INDEX);
  }
  if (vram.breg.at(static_cast<int>(BRegAddr::BOOK_STATES))) {
    states = vram.book_states;
    submits |= static_cast<int>(DEV::Addr::BOOKTAB_STATES);
  }
  if (submits > 0) {
    emit ToDev(static_cast<T_dev_addr>(submits), ivec, strs, states);
  }
  submits = static_cast<int>(DEV::Addr::NOP);
  ivec.clear();
  strs.clear();
  states.clear();

  // pagelist
  if (vram.breg.at(static_cast<int>(BRegAddr::PAGE_LABELS))) {
    strs = vram.page_labels;
    submits |= static_cast<int>(DEV::Addr::PAGELIST_LABELS);
  }
  if (vram.breg.at(static_cast<int>(BRegAddr::PAGE_INDEX))) {
    ivec.append(vram.ireg.at(static_cast<int>(IRegAddr::PAGE_INDEX)));
    submits |= static_cast<int>(DEV::Addr::PAGELIST_INDEX);
  }
  if (vram.breg.at(static_cast<int>(BRegAddr::PAGE_STATES))) {
    states = vram.page_states;
    submits |= static_cast<int>(DEV::Addr::PAGELIST_STATES);
  }
  if (submits > 0) {
    emit ToDev(static_cast<T_dev_addr>(submits), ivec, strs, states);
  }
  submits = static_cast<int>(DEV::Addr::NOP);
  ivec.clear();
  strs.clear();
  states.clear();

  // editor
  if (vram.breg.at(static_cast<int>(BRegAddr::EDITOR_TEXT))) {
    strs = T_strs{vram.sreg.at(static_cast<int>(SRegAddr::TEXT))};
    submits |= static_cast<int>(DEV::Addr::EDITOR_TEXT);
  }
  if (vram.breg.at(static_cast<int>(BRegAddr::EDITOR_READONLY))) {
    ivec.append(vram.ireg.at(static_cast<int>(IRegAddr::TEXT_READONLY)));
    submits |= static_cast<int>(DEV::Addr::EDITOR_READONLY);
  }
  if (submits > 0) {
    emit ToDev(static_cast<T_dev_addr>(submits), ivec, strs, states);
  }
  submits = static_cast<int>(DEV::Addr::NOP);
  ivec.clear();
  strs.clear();
  states.clear();

  // window
  if (vram.breg.at(static_cast<int>(BRegAddr::WINDOW_TITLE))) {
    strs = T_strs{vram.sreg.at(static_cast<int>(SRegAddr::TITLE))};
    submits |= static_cast<int>(DEV::Addr::WINDOW_TITLE);
  }
  if (submits > 0) {
    emit ToDev(static_cast<T_dev_addr>(submits), ivec, strs, states);
  }
  submits = static_cast<int>(DEV::Addr::NOP);
  ivec.clear();
  strs.clear();
  states.clear();

  // statusbar
  if (vram.breg.at(static_cast<int>(BRegAddr::STATUS_MESSAGE))) {
    strs = T_strs{vram.sreg.at(static_cast<int>(SRegAddr::MESSAGE))};
    submits |= static_cast<int>(DEV::Addr::STATUS_MESSAGE);
  }
  if (submits > 0) {
    emit ToDev(static_cast<T_dev_addr>(submits), ivec, strs, states);
  }
  submits = static_cast<int>(DEV::Addr::NOP);
  ivec.clear();
  strs.clear();
  states.clear();

  return Result::SUCCESS;
}

T_gpu_result Core::ToBookTabData(T_gpu_addr addr, int idx, T_strs labels, T_states states)
{
  if (_IsExistsAddr(addr, Addr::BOOKTAB_INDEX)) {
    UpdateBookIndex(&vram, idx);
  }
  if (_IsExistsAddr(addr, Addr::BOOKTAB_LABELS)) {
    UpdateBookLabels(&vram, labels);
  }
  if (_IsExistsAddr(addr, Addr::BOOKTAB_STATES)) {
    UpdateBookStates(&vram, states);
  }

  return Result::SUCCESS;
}

T_gpu_result Core::ToEditorData(T_gpu_addr addr, bool is_ro, const T_str& text)
{
  if (_IsExistsAddr(addr, Addr::EDITOR_READONLY)) {
    UpdateEditorReadOnly(&vram, is_ro);
  }
  if (_IsExistsAddr(addr, Addr::EDITOR_TEXT)) {
    UpdateEditorText(&vram, text);
  }

  return Result::SUCCESS;
}

T_gpu_result Core::ToFileTabData(T_gpu_addr addr, int idx, T_strs labels, T_states states)
{
  if (_IsExistsAddr(addr, Addr::FILETAB_INDEX)) {
    UpdateFileIndex(&vram, idx);
  }
  if (_IsExistsAddr(addr, Addr::FILETAB_LABELS)) {
    UpdateFileLabels(&vram, labels);
  }
  if (_IsExistsAddr(addr, Addr::FILETAB_STATES)) {
    UpdateFileStates(&vram, states);
  }

  return Result::SUCCESS;
}

T_gpu_result Core::ToPageListData(T_gpu_addr addr, int idx, T_strs labels, T_states states)
{
  if (_IsExistsAddr(addr, Addr::PAGELIST_INDEX)) {
    UpdatePageIndex(&vram, idx);
  }
  if (_IsExistsAddr(addr, Addr::PAGELIST_LABELS)) {
    UpdatePageLabels(&vram, labels);
  }
  if (_IsExistsAddr(addr, Addr::PAGELIST_STATES)) {
    UpdatePageStates(&vram, states);
  }

  return Result::SUCCESS;
}

T_gpu_result Core::ToStatusBarData(T_gpu_addr addr, const T_str& msg)
{
  if (_IsExistsAddr(addr, Addr::STATUS_MESSAGE)) {
    UpdateStatusMessage(&vram, msg);
  }

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
  if (_IsExistsAddr(addr, Addr::WINDOW_TITLE)) {
    UpdateWindowTitle(&vram, title);
  }

  return Result::SUCCESS;
}

}  // ns GPU
