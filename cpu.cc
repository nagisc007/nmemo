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

QPair<T_strs, T_strs> _fileDataDecoded(T_strs data)
{
  T_strs booklabels;
  T_strs bookdatalist;
  bool is_oldversion = false;
  // version check
  if (data.at(0) == COMMON::FILE_PREFIX) {
    auto version = data.at(1).toInt();
    is_oldversion = version < COMMON::FILE_VERSION.toInt();
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

T_submits _submitsOf(T_cpu_addr addr)
{
  switch (addr) {
  case Addr::BOOK_ADD: return 0;
  case Addr::BOOK_CHANGE: return 0;
  case Addr::BOOK_DELETE: return 0;
  case Addr::BOOK_MOVE: return 0;
  case Addr::BOOK_RENAME: return 0;
  case Addr::BOOK_SORT: return 0;
  case Addr::FILE_CHANGE: return 0;
  case Addr::FILE_CLOSE: return 0;
  case Addr::FILE_NEW: return 0;
  case Addr::FILE_OPEN: return 0;
  case Addr::FILE_MOVE: return 0;
  case Addr::FILE_RENAME: return 0;
  case Addr::FILE_SAVE: return 0;
  case Addr::FILE_SAVEAS: return 0;
  case Addr::PAGE_ADD: return 0;
  case Addr::PAGE_CHANGE: return 0;
  case Addr::PAGE_DELETE: return 0;
  case Addr::PAGE_MOVE: return 0;
  case Addr::PAGE_RENAME: return 0;
  case Addr::PAGE_SORT: return 0;
  case Addr::TEXT_MODIFY: return 0;
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
  return T_str();
}

T_str _titleOf()
{
  return DEFAULT::WINDOW_TITLE;
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
  ram.Reset();
}

T_cpu_result Core::ToAddBook(T_id fid, const T_str& name)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  auto bid = PopId(&ram, IdAddr::BOOK);
  if (!IsValidName(name)) return Result::INVALID_NAME;

  if (!AddBook(&ram, bid, name) ||
      !AppendBookIds(&ram, fid, bid) ||
      !UpdateCurrentBookId(&ram, fid, bid)) return Result::INVALID_OPERATION;

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
    if (!UpdateCurrentBookId(&ram, fid, bookIdOf(&ram, fid, idx - 1), true))
      return Result::INVALID_OPERATION;
  }
  if (!PushId(&ram, IdAddr::BOOK, bid)) return Result::INVALID_BOOKID;

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
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  if (_IsExistsSubmit(submits, GPU::Addr::BOOKTAB_INDEX)) {
    auto bid = currentBookId(&ram, fid, true);
    if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;
    idx = bookIndexOf(&ram, fid, currentBookId(&ram, fid, true), true);
  }
  if (_IsExistsSubmit(submits, GPU::Addr::BOOKTAB_LABELS))
    labels = bookLabelsOf(&ram, fid, true);
  if (_IsExistsSubmit(submits, GPU::Addr::BOOKTAB_STATES))
    states = bookStatesOf(&ram, fid, true);
  emit ToGpu(GPU::Addr::BOOKTAB,
             _ivecCompressred(idx, states), labels);
  return Result::SUCCESS;
}

T_cpu_result Core::ToGpuDataOfEditor(T_submits submits)
{
  T_str text;
  bool is_ro = true;
  auto fid = currentFileId(&ram);
  // NOTE: when invalid fid, show default editor and message
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  auto bid = currentBookId(&ram, fid, true);
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;

  auto pid = currentPageId(&ram, bid, true);
  is_ro = !IsValidPageId(&ram, bid, pid, true);

  if (_IsExistsSubmit(submits, GPU::Addr::EDITOR_TEXT)) {
    text = !is_ro ? pageTextOf(&ram, pid, false, fid, bid): DEFAULT::PAGE_TEXT;
  }
  if (_IsExistsSubmit(submits, GPU::Addr::EDITOR_READONLY)) {
    // NOTE: use to force readonly setting
  }
  emit ToGpu(GPU::Addr::EDITOR,
             _ivecPacked(is_ro), _strsPacked(text));
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
  emit ToGpu(GPU::Addr::FILETAB,
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
  emit ToGpu(GPU::Addr::PAGELIST,
             _ivecCompressred(idx, states), labels);
  return Result::SUCCESS;
}

T_cpu_result Core::ToGpuDataOfStatusBar(T_submits submits, T_cpu_addr addr)
{
  T_str msg;
  if (_IsExistsSubmit(submits, GPU::Addr::STATUS_MESSAGE))
    msg = _statusMessageOf(addr);
  emit ToGpu(GPU::Addr::STATUS_MESSAGE,
             T_ivec(), _strsPacked(msg));
  return Result::SUCCESS;
}

T_cpu_result Core::ToGpuDataOfToolBar(T_submits submits)
{
  qInfo() << "Gpu to toolbar data not implemented currently";
  return Result::SUCCESS;
}

T_cpu_result Core::ToGpuDataOfWindow(T_submits submits)
{
  T_str title;
  if (_IsExistsSubmit(submits, GPU::Addr::WINDOW_TITLE))
    title = _titleOf();
  emit ToGpu(GPU::Addr::WINDOW,
             T_ivec(), _strsPacked(title));
  return Result::SUCCESS;
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
  for (auto& id: bookIdsOf(&ram, fid, true)) {
    UpdateBookModified(&ram, id, false, true);
    for (auto& pid: pageIdsOf(&ram, id, true)) {
      UpdatePageModified(&ram, pid, false, true);
    }
  }
  UpdateFileModified(&ram, fid, false, true);

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
  case Addr::FILE_NEW: return ToCreateFile(DEFAULT::FILE_TITLE);
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
  default:
    break;
  }
  return Result::SUCCESS;
}

T_cpu_result Core::ToRenameBook(T_id fid, T_index idx, const T_str& name)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  if (!IsValidBookIndex(&ram, fid, idx, true)) return Result::INVALID_BOOKINDEX;

  auto bid = bookIdOf(&ram, fid, idx, true);
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;

  if (!IsValidName(name)) return Result::INVALID_NAME;

  if (!RenameBook(&ram, bid, name, true)) return Result::INVALID_OPERATION;

  return Result::SUCCESS;
}

T_cpu_result Core::ToRenameFile(T_index idx, const T_str& name)
{
  auto fid = fileIdOf(&ram, idx);
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  if (!IsValidName(name)) return Result::INVALID_NAME;

  if (!RenameFile(&ram, fid, name, true)) return Result::INVALID_OPERATION;

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

  for (auto& id: bookIdsOf(&ram, fid, true)) {
    UpdateBookModified(&ram, id, false, false, fid);
    for (auto& pid: pageIdsOf(&ram, id, false, fid)) {
      UpdatePageModified(&ram, pid, false, false, fid, id);
    }
  }
  UpdateFileModified(&ram, fid, false, true);

  return Result::SUCCESS;
}

T_cpu_result Core::ToSortBooks(T_id fid, T_order order)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;

  if (!SortBooks(&ram, fid, order, true) ||
      !UpdateCurrentBookId(&ram, fid, currentBookId(&ram, fid, true), true))
    return Result::INVALID_OPERATION;

  return Result::SUCCESS;
}

T_cpu_result Core::ToSortPages(T_id fid, T_id bid, T_order order)
{
  if (!IsValidFileId(&ram, fid)) return Result::INVALID_FILEID;
  if (!IsValidBookId(&ram, fid, bid, true)) return Result::INVALID_BOOKID;

  if (!SortPages(&ram, bid, order, true) ||
      !UpdateCurrentPageId(&ram, bid, currentPageId(&ram, bid, true), true))
    return Result::INVALID_OPERATION;

  return Result::SUCCESS;
}

}  // ns CPU
