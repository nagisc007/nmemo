/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "cpu.h"

#include <QDataStream>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>

namespace CPU {

namespace {

T_gpu_addr _gpuAddrFetched(T_submits submits, T_gpu_addr addr)
{
  return static_cast<T_gpu_addr>(static_cast<int>(submits) & static_cast<int>(addr));
}

T_str _filePathValidated(T_str path)
{
  return QFileInfo(path).suffix() == COMMON::FILE_EXTENSION ?
        path: QString("%1.%2").arg(path).arg(COMMON::FILE_EXTENSION);
}

int _fromIndex(int i)
{
  return (i >> 8) & 0xff;
}

int _toIndex(int i)
{
  return i & 0xff;
}

int _sliderPosOf(int i)
{
  return (i >> 16) & 0xffff;
}

int _cursorPosOf(int i)
{
  return i & 0xffff;
}

QPair<T_strs, T_strs> _fileDataDecoded(T_strs data)
{
  T_strs booklabels;
  T_strs bookdatalist;
  bool is_oldversion = false;
  // version check
  if (data.at(0) == COMMON::FILE_PREFIX) {
    auto version = data.at(1).toInt();
    is_oldversion = version < COMMON::FILE_BASE_VERSION.toInt();
  } else {
    is_oldversion = true;
  }
  // data decode
  QStringList::const_iterator it;
  for (it = data.constBegin(); it != data.constEnd(); ++it) {
    if (!is_oldversion) {
      ++it;
      ++it;
      is_oldversion = true;
    }
    booklabels << *it;
    ++it;
    bookdatalist << *it;
  }
  return qMakePair(booklabels, bookdatalist);
}

QPair<T_strs, T_strs> _bookDataDecoded(T_str bookdata)
{
  auto data = bookdata.split(COMMON::SPLITTER);
  T_strs labels;
  T_strs texts;
  // data decode
  QStringList::const_iterator it;
  for (it = data.constBegin(); it != data.constEnd(); ++it) {
    labels << *it;
    ++it;
    texts << *it;
  }
  return qMakePair(labels, texts);
}

T_str _bookDataEncoded(T_strs labels, T_strs texts)
{
  T_strs res;
  for (int i = 0; i < labels.size(); ++i) {
    res << labels.at(i) << texts.at(i);
  }
  return res.join(COMMON::SPLITTER);
}

T_strs _fileDataEncoded(T_strs labels, T_strs bookDataList)
{
  T_strs res;
  res << COMMON::FILE_PREFIX << COMMON::FILE_VERSION;
  for (int i = 0; i < labels.size(); ++i) {
    res << labels.at(i) << bookDataList.at(i);
  }
  return res;
}

T_submits _submitsCombined(T_gpu_addr a0, T_gpu_addr a1 = GPU::Addr::NOP,
                           T_gpu_addr a2 = GPU::Addr::NOP,
                           T_gpu_addr a3 = GPU::Addr::NOP,
                           T_gpu_addr a4 = GPU::Addr::NOP,
                           T_gpu_addr a5 = GPU::Addr::NOP)
{
  return static_cast<T_submits>(static_cast<int>(a0) |
      static_cast<int>(a1) |
      static_cast<int>(a2) |
      static_cast<int>(a3) |
      static_cast<int>(a4) |
      static_cast<int>(a5));
}

T_submits _submitsOf(T_cpu_addr addr)
{
  switch (addr) {
  case Addr::BOOK_ADD:
    return _submitsCombined(GPU::Addr::FILETAB_STATES,
                            GPU::Addr::BOOKTAB_ALL,
                            GPU::Addr::PAGELIST_ALL,
                            GPU::Addr::EDITOR_READONLY,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::BOOK_CHANGE:
    return _submitsCombined(GPU::Addr::BOOKTAB_INDEX,
                            GPU::Addr::PAGELIST_ALL,
                            GPU::Addr::EDITOR_READONLY,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::BOOK_DELETE:
    return _submitsCombined(GPU::Addr::FILETAB_STATES,
                            GPU::Addr::BOOKTAB_ALL,
                            GPU::Addr::PAGELIST_ALL,
                            GPU::Addr::EDITOR_READONLY,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::BOOK_MOVE:
    return _submitsCombined(GPU::Addr::FILETAB_STATES,
                            GPU::Addr::BOOKTAB_ALL,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::BOOK_RENAME:
    return _submitsCombined(GPU::Addr::FILETAB_STATES,
                            GPU::Addr::BOOKTAB_LABELS,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::BOOK_SORT:
    return _submitsCombined(GPU::Addr::FILETAB_STATES,
                            GPU::Addr::BOOKTAB_ALL,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::FILE_CHANGE:
    return _submitsCombined(GPU::Addr::FILETAB_INDEX,
                            GPU::Addr::BOOKTAB_ALL,
                            GPU::Addr::PAGELIST_ALL,
                            GPU::Addr::EDITOR_READONLY,
                            GPU::Addr::WINDOW_TITLE,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::FILE_CLOSE:
    return _submitsCombined(GPU::Addr::FILETAB_ALL,
                            GPU::Addr::BOOKTAB_ALL,
                            GPU::Addr::PAGELIST_ALL,
                            GPU::Addr::EDITOR_READONLY,
                            GPU::Addr::WINDOW_TITLE,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::FILE_NEW:
    return _submitsCombined(GPU::Addr::FILETAB_ALL,
                            GPU::Addr::BOOKTAB_ALL,
                            GPU::Addr::PAGELIST_ALL,
                            GPU::Addr::EDITOR_ALL,
                            GPU::Addr::WINDOW_TITLE,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::FILE_OPEN:
    return _submitsCombined(GPU::Addr::FILETAB_ALL,
                            GPU::Addr::BOOKTAB_ALL,
                            GPU::Addr::PAGELIST_ALL,
                            GPU::Addr::EDITOR_ALL,
                            GPU::Addr::WINDOW_TITLE,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::FILE_MOVE:
    return _submitsCombined(GPU::Addr::FILETAB_ALL,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::FILE_RENAME:
    return _submitsCombined(GPU::Addr::FILETAB_LABELS,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::FILE_SAVE:
  case Addr::FILE_SAVEAS:
    return _submitsCombined(GPU::Addr::FILETAB_LABELS,
                            GPU::Addr::FILETAB_STATES,
                            GPU::Addr::BOOKTAB_STATES,
                            GPU::Addr::PAGELIST_STATES,
                            GPU::Addr::WINDOW_TITLE,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::PAGE_ADD:
    return _submitsCombined(GPU::Addr::FILETAB_STATES,
                            GPU::Addr::BOOKTAB_STATES,
                            GPU::Addr::PAGELIST_ALL,
                            GPU::Addr::EDITOR_READONLY,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::PAGE_CHANGE:
    return _submitsCombined(GPU::Addr::PAGELIST_INDEX,
                            GPU::Addr::EDITOR_READONLY,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::PAGE_DELETE:
    return _submitsCombined(GPU::Addr::FILETAB_STATES,
                            GPU::Addr::BOOKTAB_STATES,
                            GPU::Addr::PAGELIST_ALL,
                            GPU::Addr::EDITOR_READONLY,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::PAGE_MOVE:
    return _submitsCombined(GPU::Addr::FILETAB_STATES,
                            GPU::Addr::BOOKTAB_STATES,
                            GPU::Addr::PAGELIST_ALL,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::PAGE_RENAME:
    return _submitsCombined(GPU::Addr::FILETAB_STATES,
                            GPU::Addr::BOOKTAB_STATES,
                            GPU::Addr::PAGELIST_LABELS,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::PAGE_SORT:
    return _submitsCombined(GPU::Addr::FILETAB_STATES,
                            GPU::Addr::BOOKTAB_STATES,
                            GPU::Addr::PAGELIST_ALL,
                            GPU::Addr::STATUS_MESSAGE);
  case Addr::TEXT_MODIFY:
    return _submitsCombined(GPU::Addr::FILETAB_STATES,
                            GPU::Addr::BOOKTAB_STATES,
                            GPU::Addr::PAGELIST_STATES);
  case Addr::TEXT_UPDATE:
    return _submitsCombined(GPU::Addr::EDITOR_TEXT);
  case Addr::NOP: return 0;
  }
}

bool _IsExistsSubmit(T_submits submits, T_gpu_addr target)
{
  return (submits & static_cast<int>(target)) != 0;
}

T_ivec _ivecPacked(int i)
{
  T_ivec res;
  res << i;
  return res;
}

T_ivec _ivecCompressedForEditor(int i, int spos, int cpos)
{
  T_ivec res;
  res << i << spos << cpos;
  return res;
}

T_ivec _ivecCompressred(int iv, T_states states)
{
  T_ivec res;
  res << iv;
  for (int i = 0; i < states.size(); ++i) {
    res << states.at(i);
  }
  return res;
}

T_strs _strsPacked(const T_str& s)
{
  T_strs res;
  res << s;
  return res;
}

T_str _statusMessageOf(T_cpu_addr addr)
{
  switch (addr) {
  case Addr::BOOK_ADD: return MSG::BOOK_ADDED;
  case Addr::BOOK_CHANGE: return MSG::BOOK_CHANGED;
  case Addr::BOOK_DELETE: return MSG::BOOK_DELETED;
  case Addr::BOOK_MOVE: return MSG::BOOK_MOVED;
  case Addr::BOOK_RENAME: return MSG::BOOK_RENAMED;
  case Addr::BOOK_SORT: return MSG::BOOKS_SORTED;
  case Addr::FILE_CHANGE: return MSG::FILE_CHANGED;
  case Addr::FILE_CLOSE: return MSG::FILE_CLOSED;
  case Addr::FILE_MOVE: return MSG::FILE_MOVED;
  case Addr::FILE_NEW: return MSG::FILE_CREATED;
  case Addr::FILE_OPEN: return MSG::FILE_OPENED;
  case Addr::FILE_RENAME: return MSG::FILE_RENAMED;
  case Addr::FILE_SAVE: return MSG::FILE_SAVED;
  case Addr::FILE_SAVEAS: return MSG::FILE_SAVED;
  case Addr::PAGE_ADD: return MSG::PAGE_ADDED;
  case Addr::PAGE_CHANGE: return MSG::PAGE_CHANGED;
  case Addr::PAGE_DELETE: return MSG::PAGE_DELETED;
  case Addr::PAGE_MOVE: return MSG::PAGE_MOVED;
  case Addr::PAGE_RENAME: return MSG::PAGE_RENAMED;
  case Addr::PAGE_SORT: return MSG::PAGES_SORTED;
  case Addr::TEXT_MODIFY:
  case Addr::TEXT_UPDATE:
  case Addr::NOP:
    return T_str();
  }
}

T_str _titleOf(const T_str& path)
{
  return QFileInfo(path).suffix() == COMMON::FILE_EXTENSION ?
        QFileInfo(path).absoluteDir().dirName(): DEFAULT::WINDOW_TITLE;
}

}  // inner global

// class: CPU::Core
Core::Core(QObject *parent) : QObject(parent)
{

}

// slots
void Core::FromDev(T_cpu_addr addr, int i, const T_str& s)
{
  auto result = ToProcess(addr, i, s);
  if (result == Result::SUCCESS) {
    result = ToGpuData(addr);
  } else if (result == Result::SUCCESS_NOEFFECTED) {
    return;
  }
  if (result == Result::SUCCESS) {
    emit ToGpu(GPU::Addr::FLUSH);
  } else {
    emit ToError(result);
  }
}

void Core::FromIrq(T_cpu_irq irq)
{
  switch (irq) {
  case Irq::RESET:
    ToReset();
    break;
  case Irq::NOP:
    break;
  }
}

// streams
void Core::ToReset()
{
  emit ToGpuIrq(GPU::Irq::RESET);
  ram.Reset();
  ToGpuDataOfFileTab(_submitsCombined(GPU::Addr::FILETAB_ALL));
  ToGpuDataOfBookTab(_submitsCombined(GPU::Addr::BOOKTAB_ALL));
  ToGpuDataOfPageList(_submitsCombined(GPU::Addr::PAGELIST_ALL));
  ToGpuDataOfEditor(_submitsCombined(GPU::Addr::EDITOR_ALL));
  ToGpuDataOfWindow(_submitsCombined(GPU::Addr::WINDOW_ALL));
  emit ToGpu(GPU::Addr::FLUSH);
}

T_cpu_result Core::ToAddBook(T_id fid, const T_str& name)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  auto bid = PopId(&ram, IdAddr::BOOK);
  if (!IsValidName(name)) return Result::INVALID_NAME;

  if (!AddBook(&ram, bid, name) ||
      !AppendBookIds(&ram, fid, bid) ||
      !UpdateCurrentBookId(&ram, fid, bid)) return Result::INVALID_OPERATION;

  ToUpdateModified(fid, bid, -1, true, false);
  return Result::SUCCESS;
}

T_cpu_result Core::ToAddPage(T_id fid, T_id bid, const T_str& name, const T_str& text)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;

  auto pid = PopId(&ram, IdAddr::PAGE);
  if (!IsValidName(name)) return Result::INVALID_NAME;

  if (!AddPage(&ram, pid, name, text) ||
      !AppendPageIds(&ram, bid, pid, true) ||
      !UpdateCurrentPageId(&ram, bid, pid, true)) return Result::INVALID_OPERATION;

  ToUpdateModified(fid, bid, pid, true, true);
  return Result::SUCCESS;
}

T_cpu_result Core::ToChangeBook(T_id fid, T_index idx)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  if (!IsValidBookIndex(&ram, fid, idx, true)) return Result::INVALID_BOOKINDEX;

  auto bid = bookIdOf(&ram, fid, idx, true);
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;

  if (!UpdateCurrentBookId(&ram, fid, bid, true)) return Result::INVALID_OPERATION;

  return Result::SUCCESS;
}

T_cpu_result Core::ToChangeFile(T_index idx)
{
  auto fid = fileIdOf(&ram, idx);
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  if (!UpdateCurrentFileId(&ram, fid)) return Result::INVALID_OPERATION;

  return Result::SUCCESS;
}

T_cpu_result Core::ToChangePage(T_id fid, T_id bid, T_index idx)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;

  if (!IsValidPageIndex(&ram, bid, idx, true)) return Result::INVALID_PAGEINDEX;

  auto pid = pageIdOf(&ram, bid, idx, true);
  if (!IsValidPageId(&ram, bid, pid, true)) return Result::INVALID_PAGEID;

  if (!UpdateCurrentPageId(&ram, bid, pid, true)) return Result::INVALID_OPERATION;

  return Result::SUCCESS;
}

T_cpu_result Core::ToCloseFile(T_index idx)
{
  auto fid = fileIdOf(&ram, idx);
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  if (!RemoveFileIds(&ram, fid)) return Result::INVALID_OPERATION;

  if (!IsValidFileId(&ram, currentFileId(&ram))) {
    if (!UpdateCurrentFileId(&ram, fileIdOf(&ram, idx - 1)))
      return Result::INVALID_OPERATION;
  }
  if (!PushId(&ram, IdAddr::FILE, fid)) return Result::INVALID_FILEID;

  return Result::SUCCESS;
}

T_cpu_result Core::ToCreateFile(const T_str& name)
{
  if (!IsValidName(name)) return Result::INVALID_NAME;

  auto fid = PopId(&ram, IdAddr::FILE);

  if (!AddFile(&ram, fid, name) ||
      !AppendFileIds(&ram, fid) ||
      !UpdateCurrentFileId(&ram, fid)) return Result::INVALID_OPERATION;

  return Result::SUCCESS;
}

T_cpu_result Core::ToDeleteBook(T_id fid, T_index idx)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  if (!IsValidBookIndex(&ram, fid, idx)) return Result::INVALID_BOOKINDEX;

  auto bid = bookIdOf(&ram, fid, idx, true);
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;

  if (!RemoveBookIds(&ram, fid, bid, true)) return Result::INVALID_OPERATION;

  if (!IsValidBookId(&ram, fid, currentBookId(&ram, fid, true), true)) {
    if (!UpdateCurrentBookId(&ram, fid, bookIdOf(&ram, fid, idx - 1, false), true))
      return Result::INVALID_OPERATION;
  }
  if (!PushId(&ram, IdAddr::BOOK, bid)) return Result::INVALID_BOOKID;

  ToUpdateModified(fid, bid, -1, true, false);
  return Result::SUCCESS;
}

T_cpu_result Core::ToDeletePage(T_id fid, T_id bid, T_index idx)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;

  if (!IsValidPageIndex(&ram, bid, idx, true)) return Result::INVALID_PAGEINDEX;

  auto pid = pageIdOf(&ram, bid, idx, true);
  if (!IsValidPageId(&ram, bid, pid, true)) return Result::INVALID_PAGEID;

  if (!RemovePageIds(&ram, bid, pid, true)) return Result::INVALID_OPERATION;

  if (!IsValidPageId(&ram, bid, currentPageId(&ram, bid, true), true)) {
    if (!UpdateCurrentPageId(&ram, bid, pageIdOf(&ram, bid, idx - 1, false, fid), true)) {
      return Result::INVALID_OPERATION;
    }
  }
  if (!PushId(&ram, IdAddr::PAGE, pid)) return Result::INVALID_PAGEID;

  ToUpdateModified(fid, bid, currentPageId(&ram, bid, true), true, false);
  return Result::SUCCESS;
}

T_cpu_result Core::ToGpuData(T_cpu_addr addr)
{
  auto submits = _submitsOf(addr);
  auto result = Result::SUCCESS;
  if (_IsExistsSubmit(submits, GPU::Addr::BOOKTAB)) {
    // bookdata to booktab
    result = ToGpuDataOfBookTab(submits);
    if (result != Result::SUCCESS) return result;
  }
  if (_IsExistsSubmit(submits, GPU::Addr::EDITOR)) {
    // textdata to editor
    result = ToGpuDataOfEditor(submits);
    if (result != Result::SUCCESS) return result;
  }
  if (_IsExistsSubmit(submits, GPU::Addr::FILETAB)) {
    // filedata to filetab
    result = ToGpuDataOfFileTab(submits);
    if (result != Result::SUCCESS) return result;
  }
  if (_IsExistsSubmit(submits, GPU::Addr::PAGELIST)) {
    // pagedata to pagelist
    result = ToGpuDataOfPageList(submits);
    if (result != Result::SUCCESS) return result;
  }
  if (_IsExistsSubmit(submits, GPU::Addr::WINDOW)) {
    // to window
    result = ToGpuDataOfWindow(submits);
    if (result != Result::SUCCESS) return result;
  }
  if (_IsExistsSubmit(submits, GPU::Addr::STATUSBAR)) {
    // to statusbar
    result = ToGpuDataOfStatusBar(submits, addr);
    if (result != Result::SUCCESS) return result;
  }
  if (_IsExistsSubmit(submits, GPU::Addr::TOOLBAR)) {
    // to toolbar
    result = ToGpuDataOfToolBar(submits);
    if (result != Result::SUCCESS) return result;
  }
  return result;
}

T_cpu_result Core::ToGpuDataOfBookTab(T_submits submits)
{
  int idx = -1;
  T_strs labels;
  T_states states;
  auto fid = currentFileId(&ram);
  // NOTE: when invalid fid, show default booktab

  if (_IsExistsSubmit(submits, GPU::Addr::BOOKTAB_INDEX)) {
    auto bid = currentBookId(&ram, fid, false);
    idx = bookIndexOf(&ram, fid, bid, false);
  }
  if (_IsExistsSubmit(submits, GPU::Addr::BOOKTAB_LABELS))
    labels = bookLabelsOf(&ram, fid, false);
  if (_IsExistsSubmit(submits, GPU::Addr::BOOKTAB_STATES))
    states = bookStatesOf(&ram, fid, false);

  emit ToGpu(_gpuAddrFetched(submits, GPU::Addr::BOOKTAB_ALL),
             _ivecCompressred(idx, states), labels);
  return Result::SUCCESS;
}

T_cpu_result Core::ToGpuDataOfEditor(T_submits submits)
{
  T_str text;
  int spos = 0;
  int cpos = 0;
  bool is_ro = true;
  auto fid = currentFileId(&ram);
  auto bid = currentBookId(&ram, fid, false);
  // NOTE: when invalid fid, show default editor and message
  auto pid = currentPageId(&ram, bid, false, fid);
  is_ro = !IsValidPageId(&ram, bid, pid, false, fid);

  if (is_ro) {
    text = DEFAULT::PAGE_CAUTION_TEXT;
  } else if (_IsExistsSubmit(submits, GPU::Addr::EDITOR_TEXT)) {
    text = pageTextOf(&ram, pid, false, fid, bid);
    spos = pageSliderPosOf(&ram, pid, true);
    cpos = pageCursorPosOf(&ram, pid, true);
  }
  if (_IsExistsSubmit(submits, GPU::Addr::EDITOR_READONLY)) {
    // NOTE: use to force readonly setting
  }
  emit ToGpu(_gpuAddrFetched(submits, GPU::Addr::EDITOR_ALL),
             _ivecCompressedForEditor(is_ro, spos, cpos), _strsPacked(text));
  return Result::SUCCESS;
}

T_cpu_result Core::ToGpuDataOfFileTab(T_submits submits)
{
  int idx = -1;
  T_strs labels;
  T_states states;
  auto fid = currentFileId(&ram);

  if (_IsExistsSubmit(submits, GPU::Addr::FILETAB_INDEX))
    idx = fileIndexOf(&ram, fid);
  if (_IsExistsSubmit(submits, GPU::Addr::FILETAB_LABELS))
    labels = fileLabelsOf(&ram);
  if (_IsExistsSubmit(submits, GPU::Addr::FILETAB_STATES))
    states = fileStatesOf(&ram);

  emit ToGpu(_gpuAddrFetched(submits, GPU::Addr::FILETAB_ALL),
             _ivecCompressred(idx, states), labels);
  return Result::SUCCESS;
}

T_cpu_result Core::ToGpuDataOfPageList(T_submits submits)
{
  int idx = -1;
  T_strs labels;
  T_states states;
  auto fid = currentFileId(&ram);
  auto bid = currentBookId(&ram, fid, false);
  auto pid = currentPageId(&ram, bid, false, fid);
  // NOTE: when invalid fid or bid, default plain pagelist showing

  if (_IsExistsSubmit(submits, GPU::Addr::PAGELIST_INDEX))
    idx = pageIndexOf(&ram, bid, pid, false, fid);
  if (_IsExistsSubmit(submits, GPU::Addr::PAGELIST_LABELS))
    labels = pageLabelsOf(&ram, bid, false, fid);
  if (_IsExistsSubmit(submits, GPU::Addr::PAGELIST_STATES))
    states = pageStatesOf(&ram, bid, false, fid);

  emit ToGpu(_gpuAddrFetched(submits, GPU::Addr::PAGELIST),
             _ivecCompressred(idx, states), labels);
  return Result::SUCCESS;
}

T_cpu_result Core::ToGpuDataOfStatusBar(T_submits submits, T_cpu_addr addr)
{
  T_str msg;
  if (_IsExistsSubmit(submits, GPU::Addr::STATUS_MESSAGE))
    msg = _statusMessageOf(addr);

  emit ToGpu(_gpuAddrFetched(submits, GPU::Addr::STATUSBAR_ALL),
             T_ivec(), _strsPacked(msg));
  return Result::SUCCESS;
}

T_cpu_result Core::ToGpuDataOfToolBar(T_submits submits)
{
  Q_UNUSED(submits);
  qInfo() << "Gpu to toolbar data not implemented currently";
  return Result::SUCCESS;
}

T_cpu_result Core::ToGpuDataOfWindow(T_submits submits)
{
  T_str title;
  auto fid = currentFileId(&ram);
  if (_IsExistsSubmit(submits, GPU::Addr::WINDOW_TITLE))
    title = _titleOf(filePathOf(&ram, fid, false));

  emit ToGpu(_gpuAddrFetched(submits, GPU::Addr::WINDOW_ALL),
             T_ivec(), _strsPacked(title));
  return Result::SUCCESS;
}

T_cpu_result Core::ToModifyText(T_id fid, T_id bid, T_id pid)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;
  if (!IsValidPageId(&ram, bid, pid, true)) return Result::INVALID_PAGEID;

  if (fileModified(&ram, fid, true) &&
      bookModified(&ram, bid, true) &&
      pageModified(&ram, pid, true)) {
    return Result::SUCCESS_NOEFFECTED;
  }

  return ToUpdateModified(fid, bid, pid, true, true);
}

T_cpu_result Core::ToMoveBook(T_id fid, T_index from, T_index to)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  if (!IsValidBookIndex(&ram, fid, from, true) || !IsValidBookIndex(&ram, fid, to, true))
    return Result::INVALID_BOOKINDEX;

  if (!IsValidBookId(&ram, fid, bookIdOf(&ram, fid, from, true), true) ||
      !IsValidBookId(&ram, fid, bookIdOf(&ram, fid, to, true), true))
    return Result::INVALID_BOOKID;

  if (!MoveBook(&ram, fid, from, to, true) ||
      !UpdateCurrentBookId(&ram, fid, bookIdOf(&ram, fid, to, true), true))
    return Result::INVALID_OPERATION;

  UpdateFileModified(&ram, fid, true, true);
  return Result::SUCCESS;
}

T_cpu_result Core::ToMoveFile(T_index from, T_index to)
{
  if (!IsValidFileIndex(&ram, from) || !IsValidFileIndex(&ram, to))
    return Result::INVALID_FILEINDEX;

  if (!MoveFile(&ram, from, to) ||
      !UpdateCurrentFileId(&ram, fileIdOf(&ram, to)))
    return Result::INVALID_OPERATION;

  return Result::SUCCESS;
}

T_cpu_result Core::ToMovePage(T_id fid, T_id bid, T_index from, T_index to)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;

  if (!IsValidPageIndex(&ram, bid, from, true) || !IsValidPageIndex(&ram, bid, to, true))
    return Result::INVALID_PAGEINDEX;

  if (!IsValidPageId(&ram, bid, pageIdOf(&ram, bid, from, true), true) ||
      !IsValidPageId(&ram, bid, pageIdOf(&ram, bid, to, true), true))
    return Result::INVALID_PAGEID;

  if (!MovePage(&ram, bid, from, to, true) ||
      !UpdateCurrentPageId(&ram, bid, pageIdOf(&ram, bid, to, true), true))
    return Result::INVALID_OPERATION;

  UpdateFileModified(&ram, fid, true, true);
  UpdateBookModified(&ram, bid, true, true);
  return Result::SUCCESS;
}

T_cpu_result Core::ToOpenFile(const T_str& path)
{
  if (!IsValidPath(path)) return Result::INVALID_PATH;

  QFile file(path);
  if (!file.open(QIODevice::ReadOnly)) return Result::ERR_NOTOPEN_FILE;

  QDataStream in(&file);
  T_strs data_loaded;
  in >> data_loaded;
  if (data_loaded.count() < 2) return Result::ERR_FILE_VERSION_MISMATCH;
  if (data_loaded.at(0) != COMMON::FILE_PREFIX) return Result::ERR_FILE_DATAHEAD;

  // version check
  T_id fid = -1;
  T_id bid = -1;
  if (data_loaded.at(1).toInt() == COMMON::FILE_VERSION.toInt()) {
    // current version
    auto decoded = _fileDataDecoded(data_loaded);
    auto booklabels = decoded.first;
    auto bookdatalist = decoded.second;
    auto result = ToCreateFile(path);
    if (result != Result::SUCCESS) return result;

    auto fid = currentFileId(&ram);
    for (int i = 0; i < booklabels.size(); ++i) {
      result = ToAddBook(fid, booklabels.at(i));
      if (result != Result::SUCCESS) {
        return result;
      }

      auto bookdata = _bookDataDecoded(bookdatalist.at(i));
      auto pagelabels = bookdata.first;
      auto texts = bookdata.second;
      bid = currentBookId(&ram, fid, true);
      for (int j = 0; j < pagelabels.size(); ++j) {
        result = ToAddPage(fid, bid, pagelabels.at(j), texts.at(j));
        if (result != Result::SUCCESS) {
          return result;
        }
      }
    }
  } else {
    // old version
    auto decoded = _fileDataDecoded(data_loaded);
    auto pagelabels = decoded.first;
    auto texts = decoded.second;
    auto result = ToCreateFile(path);
    if (result != Result::SUCCESS) return result;

    result = ToAddBook(currentFileId(&ram), DEFAULT::BOOK_TITLE);
    if (result != Result::SUCCESS) return result;

    fid = currentFileId(&ram);
    bid = currentBookId(&ram, fid, true);
    for (int i = 0; i < pagelabels.size(); ++i) {
      result = ToAddPage(fid, bid, pagelabels.at(i), texts.at(i));
      if (result != Result::SUCCESS) return result;
    }
  }

  fid = currentFileId(&ram);
  bid = bookIdOf(&ram, fid, 0, true);
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;
  if (!UpdateCurrentBookId(&ram, fid, bid, true) ||
      !UpdateCurrentPageId(&ram, bid, pageIdOf(&ram, bid, 0, true), true))
    return Result::INVALID_OPERATION;

  // update modified
  for (auto& bid: bookIdsOf(&ram, fid, true)) {
    for (auto& pid: pageIdsOf(&ram, bid, true)) {
      ToUpdateModified(fid, bid, pid, false, false);
    }
  }

  return Result::SUCCESS;
}

T_cpu_result Core::ToProcess(T_cpu_addr addr, int i, const T_str& s)
{
  switch (addr) {
  case Addr::BOOK_ADD: return ToAddBook(currentFileId(&ram), s);
  case Addr::BOOK_CHANGE: return ToChangeBook(currentFileId(&ram), i);
  case Addr::BOOK_DELETE: return ToDeleteBook(currentFileId(&ram), i);
  case Addr::BOOK_MOVE: return ToMoveBook(currentFileId(&ram), _fromIndex(i), _toIndex(i));
  case Addr::BOOK_RENAME: return ToRenameBook(currentFileId(&ram), i, s);
  case Addr::BOOK_SORT: return ToSortBooks(currentFileId(&ram), static_cast<Qt::SortOrder>(i));
  case Addr::FILE_CHANGE: return ToChangeFile(i);
  case Addr::FILE_CLOSE: return ToCloseFile(i);
  case Addr::FILE_NEW: return ToCreateFile(s);
  case Addr::FILE_OPEN: return ToOpenFile(s);
  case Addr::FILE_MOVE: return ToMoveFile(_fromIndex(i), _toIndex(i));
  case Addr::FILE_RENAME: return ToRenameFile(i, s);
  case Addr::FILE_SAVE: {
    auto path = filePathOf(&ram, fileIdOf(&ram, i), false);
    return ToSaveFile(i, path);
  }
  case Addr::FILE_SAVEAS: return ToSaveFile(i, s);
  case Addr::PAGE_ADD: {
    auto fid = currentFileId(&ram);
    return ToAddPage(fid, currentBookId(&ram, fid, false), s, DEFAULT::PAGE_TEXT);
  }
  case Addr::PAGE_CHANGE: {
    auto fid = currentFileId(&ram);
    return ToChangePage(fid, currentBookId(&ram, fid, false), i);
  }
  case Addr::PAGE_DELETE: {
    auto fid = currentFileId(&ram);
    return ToDeletePage(fid, currentBookId(&ram, fid, false), i);
  }
  case Addr::PAGE_MOVE: {
    auto fid = currentFileId(&ram);
    return ToMovePage(fid, currentBookId(&ram, fid, false), _fromIndex(i), _toIndex(i));
  }
  case Addr::PAGE_RENAME: {
    auto fid = currentFileId(&ram);
    return ToRenamePage(fid, currentBookId(&ram, fid, false), i, s);
  }
  case Addr::PAGE_SORT: {
    auto fid = currentFileId(&ram);
    return ToSortPages(fid, currentBookId(&ram, fid, false), static_cast<Qt::SortOrder>(i));
  }
  case Addr::TEXT_MODIFY: {
    auto fid = currentFileId(&ram);
    auto bid = currentBookId(&ram, fid, false);
    return ToModifyText(fid, bid, currentPageId(&ram, bid, false, fid));
  }
  case Addr::TEXT_UPDATE: {
    auto fid = currentFileId(&ram);
    auto bid = currentBookId(&ram, fid, false);
    return ToUpdateText(fid, bid, currentPageId(&ram, bid, false, fid),
                        _sliderPosOf(i), _cursorPosOf(i), s);
  }
  case Addr::NOP:
    return Result::SUCCESS;
  }
}

T_cpu_result Core::ToRenameBook(T_id fid, T_index idx, const T_str& name)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  if (!IsValidBookIndex(&ram, fid, idx, true)) return Result::INVALID_BOOKINDEX;

  auto bid = bookIdOf(&ram, fid, idx, true);
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;

  if (!IsValidName(name)) return Result::INVALID_NAME;

  if (!RenameBook(&ram, bid, name, true)) return Result::INVALID_OPERATION;

  UpdateFileModified(&ram, fid, true, true);
  UpdateBookModified(&ram, bid, true, true);
  return Result::SUCCESS;
}

T_cpu_result Core::ToRenameFile(T_index idx, const T_str& name)
{
  auto fid = fileIdOf(&ram, idx);
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  if (!IsValidName(name)) return Result::INVALID_NAME;

  if (!RenameFile(&ram, fid, name, true)) return Result::INVALID_OPERATION;

  UpdateFileModified(&ram, fid, true, true);
  return Result::SUCCESS;
}

T_cpu_result Core::ToRenamePage(T_id fid, T_id bid, T_index idx, const T_str& name)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;

  if (!IsValidPageIndex(&ram, bid, idx, true)) return Result::INVALID_PAGEINDEX;

  auto pid = pageIdOf(&ram, bid, idx, true);
  if (!IsValidPageId(&ram, bid, pid, true)) return Result::INVALID_PAGEID;

  if (!IsValidName(name)) return Result::INVALID_NAME;

  if (!RenamePage(&ram, pid, name, true)) return Result::INVALID_OPERATION;

  ToUpdateModified(fid, bid, pid, true, true);
  return Result::SUCCESS;
}

T_cpu_result Core::ToSaveFile(T_index idx, const T_str& path)
{
  auto fid = fileIdOf(&ram, idx);
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;
  auto va_path = _filePathValidated(path);
  if (!IsValidPath(va_path)) {
    emit RequestFileName();
    return Result::INVALID_PATH;
  }

  T_strs bookdatalist;
  for (auto& id: bookIdsOf(&ram, fid, true)) {
    bookdatalist << _bookDataEncoded(pageLabelsOf(&ram, id, false, fid),
                                     pageTextsOf(&ram, id, false, fid));
  }
  auto encoded = _fileDataEncoded(bookLabelsOf(&ram, fid, true), bookdatalist);

  QFile file(va_path);
  if (!file.open(QIODevice::WriteOnly)) return Result::ERR_NOTOPEN_FILE;

  QDataStream out(&file);
  out.setVersion(QDataStream::Qt_5_11);
  out << (encoded);

  if (!UpdateFilePath(&ram, fid, va_path, true)) return Result::INVALID_OPERATION;

  for (auto& bid: bookIdsOf(&ram, fid, true)) {
    for (auto& pid: pageIdsOf(&ram, bid, false, fid))
      ToUpdateModified(fid, bid, pid, false, false);
  }

  return Result::SUCCESS;
}

T_cpu_result Core::ToSortBooks(T_id fid, T_order order)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  if (!SortBooks(&ram, fid, order, true) ||
      !UpdateCurrentBookId(&ram, fid, currentBookId(&ram, fid, true), true))
    return Result::INVALID_OPERATION;

  UpdateFileModified(&ram, fid, true, true);
  return Result::SUCCESS;
}

T_cpu_result Core::ToSortPages(T_id fid, T_id bid, T_order order)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;

  if (!SortPages(&ram, bid, order, true) ||
      !UpdateCurrentPageId(&ram, bid, currentPageId(&ram, bid, true), true))
    return Result::INVALID_OPERATION;

  UpdateFileModified(&ram, fid, true, true);
  UpdateBookModified(&ram, bid, true, true);
  return Result::SUCCESS;
}

T_cpu_result Core::ToUpdateText(T_id fid, T_id bid, T_id pid, int spos, int cpos,
                                const T_str& text)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;
  if (!IsValidPageId(&ram, bid, pid, true)) return Result::INVALID_PAGEID;

  if (!UpdatePagePosition(&ram, pid, spos, cpos, true) ||
      !UpdatePageText(&ram, pid, text, true)) return Result::INVALID_OPERATION;

  return Result::SUCCESS;
}

T_cpu_result Core::ToUpdateModified(T_id fid, T_id bid, T_id pid, bool modified,
                                    bool is_validated)
{
  if (!UpdateFileModified(&ram, fid, modified, is_validated) ||
      !UpdateBookModified(&ram, bid, modified, is_validated, fid) ||
      !UpdatePageModified(&ram, pid, modified, is_validated, fid, bid))
    return Result::INVALID_OPERATION;

  return Result::SUCCESS;
}

}  // ns CPU
