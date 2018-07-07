/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "ram.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>

namespace CPU {

namespace {

bool _IsValidIndex(T_index i, int size)
{
  return i >= 0 && i < size;
}

}  // inner global

// struct: MemoFile
MemoFile::MemoFile(const T_str& s):
  modified(true),
  path(s),
  current_bookid(-1),
  book_ids(QVector<int>())
{}

void MemoFile::Refresh(const T_str& s)
{
  modified = true;
  path = s;
  current_bookid = -1;
  book_ids.clear();
}

// struct: Book
Book::Book(const T_str& s):
  modified(true),
  label(s),
  current_pageid(-1),
  page_ids(QVector<int>())
{}

void Book::Refresh(const T_str& s)
{
  modified = true;
  label = s;
  current_pageid = -1;
  page_ids.clear();
}

// struct: Page
Page::Page(const T_str& s0, const T_str& s1):
  modified(true),
  label(s0),
  text(s1),
  slider_pos(0),
  cursor_pos(0)
{}

void Page::Refresh(const T_str& s0, const T_str& s1)
{
  modified = true;
  label = s0;
  text = s1;
  slider_pos = 0;
  cursor_pos = 0;
}

// struct: Ram
Ram::Ram():
  ireg(QVector<int>(IREG_SIZE, -1)),
  file_ids(QVector<int>()),
  files(QVector<MemoFile*>()),
  books(QVector<Book*>()),
  pages(QVector<Page*>())
{

}

void Ram::Reset()
{
  for (auto& v: ireg) v = -1;
  file_ids.clear();
  while (!files.isEmpty()) {
    auto tmp = files.takeFirst();
    delete tmp;
  }
  while (!books.isEmpty()) {
    auto tmp = books.takeFirst();
    delete tmp;
  }
  while (!pages.isEmpty()) {
    auto tmp = pages.takeFirst();
    delete tmp;
  }
  fileid_stack.clear();
  bookid_stack.clear();
  pageid_stack.clear();
}

// controls
bool IsExistsPath(const T_str& path)
{
  return QFileInfo(path).exists() &&
      QFileInfo(path).suffix() == COMMON::FILE_EXTENSION;
}

bool IsValidBookId(const Ram* ram, T_id fid, T_id bid, bool is_validated)
{
  return (is_validated || IsValidFileId(ram, fid)) &&
      _IsValidIndex(bid, ram->books.size()) && ram->files.at(fid)->book_ids.contains(bid);
}

bool IsValidBookIndex(const Ram* ram, T_id fid, T_index idx, bool is_validated)
{
  return (is_validated || IsValidFileId(ram, fid)) &&
      _IsValidIndex(idx, ram->files.at(fid)->book_ids.size());
}

bool IsValidFileId(const Ram* ram, T_id fid)
{
  return _IsValidIndex(fid, ram->files.size()) && ram->file_ids.contains(fid);
}

bool IsValidFileIndex(const Ram* ram, T_index idx)
{
  return _IsValidIndex(idx, ram->files.size());
}

bool IsValidName(const T_str& s)
{
  return !s.isEmpty() && s != "";
}

bool IsValidPageId(const Ram* ram, T_id bid, T_id pid, bool is_validated, T_id fid)
{
  return (is_validated || IsValidBookId(ram, fid, bid, false)) &&
      _IsValidIndex(pid, ram->pages.size()) &&
      ram->books.at(bid)->page_ids.contains(pid);
}

bool IsValidPageIndex(const Ram* ram, T_id bid, T_index idx, bool is_validated, T_id fid)
{
  return (is_validated || IsValidBookId(ram, fid, bid, false)) &&
      _IsValidIndex(idx, ram->books.at(bid)->page_ids.size());
}

bool IsValidPath(const T_str& path)
{
  return IsValidName(path) && QFileInfo(path).suffix() == COMMON::FILE_EXTENSION;
}

T_id bookIdOf(const Ram* ram, T_id fid, T_index idx, bool is_validated)
{
  return is_validated ||
      (IsValidFileId(ram, fid) && IsValidBookIndex(ram, fid, idx, false)) ?
        ram->files.at(fid)->book_ids.at(idx): -1;
}

T_ids bookIdsOf(const Ram* ram, T_id fid, bool is_validated)
{
  return is_validated || IsValidFileId(ram, fid) ?
        ram->files.at(fid)->book_ids: T_ids();
}

T_index bookIndexOf(const Ram* ram, T_id fid, T_id bid, bool is_validated)
{
  return is_validated || IsValidFileId(ram, fid) ?
        ram->files.at(fid)->book_ids.indexOf(bid): -1;
}

T_strs bookLabelsOf(const Ram* ram, T_id fid, bool is_validated)
{
  T_strs res;
  if (!is_validated && !IsValidFileId(ram, fid)) return res;

  for (auto& id: ram->files.at(fid)->book_ids) {
    res << ram->books.at(id)->label;
  }
  return res;
}

bool bookModified(const Ram* ram, T_id bid, bool is_validated, T_id fid)
{
  return is_validated || IsValidBookId(ram, fid, bid, false) ?
        ram->books.at(bid)->modified: false;
}

T_states bookStatesOf(const Ram* ram, T_id fid, bool is_validated)
{
  if (!is_validated && !IsValidFileId(ram, fid)) return T_states();

  T_states res(ram->files.at(fid)->book_ids.size());
  int idx = 0;
  for (auto& id: ram->files.at(fid)->book_ids) {
    res[idx] = ram->books.at(id)->modified;
    ++idx;
  }
  return res;
}

T_id currentFileId(const Ram* ram)
{
  return ram->ireg.at(static_cast<int>(IRegAddr::CURRENT_FILEID));
}

T_id currentBookId(const Ram* ram, T_id fid, bool is_validated)
{
  return is_validated || IsValidFileId(ram, fid) ?
        ram->files.at(fid)->current_bookid: -1;
}

T_id currentPageId(const Ram* ram, T_id bid, bool is_validated, T_id fid)
{
  return is_validated || IsValidBookId(ram, fid < 0 ? currentFileId(ram): fid, bid, false) ?
        ram->books.at(bid)->current_pageid: -1;
}

T_id fileIdOf(const Ram* ram, T_index idx)
{
  return IsValidFileIndex(ram, idx) ? ram->file_ids.at(idx): -1;
}

T_index fileIndexOf(const Ram* ram, T_id fid)
{
  return ram->file_ids.indexOf(fid);
}

T_strs fileLabelsOf(const Ram* ram)
{
  T_strs res;
  for (auto& id: ram->file_ids)
    res << ram->files.at(id)->path;
  return res;
}

bool fileModified(const Ram* ram, T_id fid, bool is_validated)
{
  return is_validated || IsValidFileId(ram, fid) ? ram->files.at(fid)->modified: false;
}

T_str filePathOf(const Ram* ram, T_id fid, bool is_validated)
{
  return is_validated || IsValidFileId(ram, fid) ? ram->files.at(fid)->path: T_str();
}

T_states fileStatesOf(const Ram* ram)
{
  T_states res(ram->file_ids.size());
  int idx = 0;
  for (auto& id: ram->file_ids) {
    res[idx] = ram->files.at(id)->modified;
    ++idx;
  }
  return res;
}

int pageCursorPosOf(const Ram* ram, T_id pid, bool is_validated, T_id fid, T_id bid)
{
  return is_validated ||
      IsValidPageId(ram, bid, pid, false, fid) ?
        ram->pages.at(pid)->cursor_pos: 0;
}

T_id pageIdOf(const Ram* ram, T_id bid, T_index idx, bool is_validated, T_id fid)
{
  return is_validated ||
      (IsValidBookId(ram, fid, bid, false) && IsValidPageIndex(ram, bid, idx, false, fid)) ?
        ram->books.at(bid)->page_ids.at(idx): -1;
}

T_ids pageIdsOf(const Ram* ram, T_id bid, bool is_validated, T_id fid)
{
  return is_validated || IsValidBookId(ram, fid, bid, false) ?
        ram->books.at(bid)->page_ids: T_ids();
}

T_index pageIndexOf(const Ram* ram, T_id bid, T_id pid, bool is_validated, T_id fid)
{
  return is_validated || IsValidBookId(ram, fid, bid, false) ?
        ram->books.at(bid)->page_ids.indexOf(pid): -1;
}

T_strs pageLabelsOf(const Ram* ram, T_id bid, bool is_validated, T_id fid)
{
  T_strs res;
  if (!is_validated && !IsValidBookId(ram, fid, bid, false)) return res;

  for (auto& id: ram->books.at(bid)->page_ids) {
    res << ram->pages.at(id)->label;
  }
  return res;
}

bool pageModified(const Ram* ram, T_id pid, bool is_validated, T_id fid, T_id bid)
{
  return is_validated || IsValidPageId(ram, bid, pid, false, fid) ?
        ram->pages.at(pid)->modified: false;
}

int pageSliderPosOf(const Ram* ram, T_id pid, bool is_validated, T_id fid, T_id bid)
{
  return is_validated ||
      IsValidPageId(ram, bid, pid, false, fid) ?
        ram->pages.at(pid)->slider_pos: 0;
}

T_states pageStatesOf(const Ram* ram, T_id bid, bool is_validated, T_id fid)
{
  if (!is_validated && !IsValidBookId(ram, fid, bid, false)) return T_states();

  T_states res(ram->books.at(bid)->page_ids.size());
  int idx = 0;
  for (auto& id: ram->books.at(bid)->page_ids) {
    res[idx] = ram->pages.at(id)->modified;
    ++idx;
  }
  return res;
}

T_strs pageTextsOf(const Ram* ram, T_id bid, bool is_validated, T_id fid)
{
  T_strs res;
  if (!is_validated && !IsValidBookId(ram, fid, bid, false)) return res;

  for (auto& id: ram->books.at(bid)->page_ids) {
    res << ram->pages.at(id)->text;
  }
  return res;
}

T_str pageTextOf(const Ram* ram, T_id pid, bool is_validated, T_id fid, T_id bid)
{
  return is_validated || IsValidPageId(ram, bid, pid, false, fid) ?
        ram->pages.at(pid)->text: T_str();
}

bool UpdateBookModified(Ram* ram, T_id bid, bool modified, bool is_validated, T_id fid)
{
  if (!is_validated && !IsValidBookId(ram, fid, bid, false)) return false;

  ram->books.at(bid)->modified = modified;
  return true;
}

bool UpdateCurrentBookId(Ram* ram, T_id fid, T_id bid, bool is_validated)
{
  if (!is_validated && !IsValidFileId(ram, fid)) return false;

  ram->files.at(fid)->current_bookid = bid;
  return true;
}

bool UpdateCurrentFileId(Ram* ram, T_id fid)
{
  ram->ireg[static_cast<int>(IRegAddr::CURRENT_FILEID)] = fid;
  return true;
}

bool UpdateCurrentPageId(Ram* ram, T_id bid, T_id pid, bool is_validated, T_id fid)
{
  if (!is_validated && !IsValidBookId(ram, fid, bid, false)) return false;

  ram->books.at(bid)->current_pageid = pid;
  return true;
}

bool UpdateFileModified(Ram* ram, T_id fid, bool modified, bool is_validated)
{
  if (!is_validated && !IsValidFileId(ram, fid)) return false;

  ram->files.at(fid)->modified = modified;
  return true;
}

bool UpdateFilePath(Ram* ram, T_id fid, const T_str& path, bool is_validated)
{
  if (!is_validated && !IsValidFileId(ram, fid)) return false;

  ram->files.at(fid)->path = path;
  return true;
}

bool UpdatePageModified(Ram* ram, T_id pid, bool modified, bool is_validated, T_id fid, T_id bid)
{
  if (!is_validated && !IsValidPageId(ram, bid, pid, false, fid)) return false;

  ram->pages.at(pid)->modified = modified;
  return true;
}

bool UpdatePagePosition(Ram* ram, T_id pid, int spos, int cpos, bool is_validated,
                        T_id fid, T_id bid)
{
  if (!is_validated && !IsValidPageId(ram, bid, pid, false, fid)) return false;

  ram->pages.at(pid)->slider_pos = spos;
  ram->pages.at(pid)->cursor_pos = cpos;
  return true;
}

bool UpdatePageText(Ram* ram, T_id pid, const T_str& text, bool is_validated, T_id fid, T_id bid)
{
  if (!is_validated && !IsValidPageId(ram, bid, pid, false, fid)) return false;

  ram->pages.at(pid)->text = text;
  return true;
}

bool AddBook(Ram* ram, T_id bid, const T_str& name)
{
  if (bid < 0) return false;

  if (bid < ram->books.size()) {
    ram->books.at(bid)->Refresh(name);
  } else {
    ram->books.append(new Book(name));
  }
  return true;
}

bool AddFile(Ram* ram, T_id fid, const T_str& name)
{
  if (fid < 0) return false;

  if (fid < ram->files.size()) {
    ram->files.at(fid)->Refresh(name);
  } else {
    ram->files.append(new MemoFile(name));
  }
  return true;
}

bool AddPage(Ram* ram, T_id pid, const T_str& name, const T_str& text)
{
  if (pid < 0) return false;

  if (pid < ram->pages.size()) {
    ram->pages.at(pid)->Refresh(name, text);
  } else {
    ram->pages.append(new Page(name, text));
  }
  return true;
}

bool AppendBookIds(Ram* ram, T_id fid, T_id bid, bool is_validated)
{
  if ((!is_validated && !IsValidFileId(ram, fid)) || bid < 0 ||
      ram->files.at(fid)->book_ids.contains(bid))
    return false;

  ram->files.at(fid)->book_ids.append(bid);
  return true;
}

bool AppendFileIds(Ram* ram, T_id fid)
{
  if (fid < 0 || ram->file_ids.contains(fid)) return false;

  ram->file_ids.append(fid);
  return true;
}

bool AppendPageIds(Ram* ram, T_id bid, T_id pid, bool is_validated, T_id fid)
{
  if ((!is_validated && !IsValidBookId(ram, fid, bid, false)) || pid < 0 ||
      ram->books.at(bid)->page_ids.contains(pid))
    return false;

  ram->books.at(bid)->page_ids.append(pid);
  return true;
}

bool MoveBook(Ram* ram, T_id fid, T_index from, T_index to, bool is_validated)
{
  if (!is_validated && !IsValidFileId(ram, fid)) return false;

  // NOTE: should need both indices validated?
  ram->files.at(fid)->book_ids.move(from, to);
  return true;
}

bool MoveFile(Ram* ram, T_index from, T_index to)
{
  // NOTE: should need both indices validated?
  ram->file_ids.move(from, to);
  return true;
}

bool MovePage(Ram* ram, T_id bid, T_index from, T_index to, bool is_validated, T_id fid)
{
  if (!is_validated && !IsValidBookId(ram, fid, bid, false)) return false;

  // NOTE: should need both indices validated?
  ram->books.at(bid)->page_ids.move(from, to);
  return true;
}

bool RemoveBookIds(Ram* ram, T_id fid, T_id bid, bool is_validated)
{
  if ((!is_validated && !IsValidFileId(ram, fid)) ||
      !ram->files.at(fid)->book_ids.contains(bid))
    return false;

  return ram->files.at(fid)->book_ids.removeOne(bid);
}

bool RemoveFileIds(Ram* ram, T_id fid)
{
  // NOTE: removeOne contains a feature to check the fid
  return ram->file_ids.removeOne(fid);
}

bool RemovePageIds(Ram* ram, T_id bid, T_id pid, bool is_validated, T_id fid)
{
  if ((!is_validated && !IsValidBookId(ram, fid, bid, false)) ||
      !ram->books.at(bid)->page_ids.contains(pid))
    return false;

  return ram->books.at(bid)->page_ids.removeOne(pid);
}

bool RenameBook(Ram* ram, T_id bid, const T_str& name, bool is_validated, T_id fid)
{
  if (!is_validated && !IsValidBookId(ram, fid, bid, false)) return false;

  ram->books.at(bid)->label = name;
  return true;
}

bool RenameFile(Ram* ram, T_id fid, const T_str& name, bool is_validated)
{
  if (!is_validated && !IsValidFileId(ram, fid)) return false;

  ram->files.at(fid)->path = name;
  return true;
}

bool RenamePage(Ram* ram, T_id pid, const T_str& name, bool is_validated, T_id fid, T_id bid)
{
  if (!is_validated && !IsValidPageId(ram, bid, pid, false, fid)) return false;

  ram->pages.at(pid)->label = name;
  return true;
}

bool SortBooks(Ram* ram, T_id fid, T_order order, bool is_validated)
{
  if (!is_validated && !IsValidFileId(ram, fid)) return false;

  auto origin = ram->files.at(fid)->book_ids;
  T_strs labels = bookLabelsOf(ram, fid, true);
  T_strs combined;
  for (int i = 0; i < labels.size(); ++i)
    combined << labels.at(i) + COMMON::SPLITTER + QString::number(origin.at(i));
  if (order == Qt::AscendingOrder) {
    std::sort(combined.begin(), combined.end());
  } else {
    std::sort(combined.begin(), combined.end(), std::greater<QString>());
  }
  QVector<int> ids;
  for (int i = 0; i < combined.size(); ++i) {
    ids << combined.at(i).section(COMMON::SPLITTER, 1, 1).toInt();
  }
  // NOTE: mutex needed?
  ram->files.at(fid)->book_ids = ids;
  return true;
}

bool SortPages(Ram* ram, T_id bid, T_order order, bool is_validated, T_id fid)
{
  if (!is_validated && !IsValidBookId(ram, fid, bid, false)) return false;

  auto origin = ram->books.at(bid)->page_ids;
  T_strs labels = pageLabelsOf(ram, bid, true);
  T_strs combined;
  for (int i = 0; i < labels.size(); ++i) {
    combined << labels.at(i) + COMMON::SPLITTER + QString::number(origin.at(i));
  }
  if (order == Qt::AscendingOrder) {
    std::sort(combined.begin(), combined.end());
  } else {
    std::sort(combined.begin(), combined.end(), std::greater<QString>());
  }
  QVector<int> ids;
  for (int i = 0; i < combined.size(); ++i) {
    ids << combined.at(i).section(COMMON::SPLITTER, 1, 1).toInt();
  }
  // NOTE: mutex needed?
  ram->books.at(bid)->page_ids = ids;
  return true;
}

T_id PopId(Ram* ram, T_id_addr addr)
{
  switch (addr) {
  case IdAddr::BOOK: {
    return ram->bookid_stack.isEmpty() ?
          ++ram->ireg[static_cast<int>(IRegAddr::NEXT_BOOKID)]:
          ram->bookid_stack.pop();
    }
  case IdAddr::FILE: {
    return ram->fileid_stack.isEmpty() ?
          ++ram->ireg[static_cast<int>(IRegAddr::NEXT_FILEID)]:
          ram->fileid_stack.pop();
    }
  case IdAddr::PAGE: {
    return ram->pageid_stack.isEmpty() ?
          ++ram->ireg[static_cast<int>(IRegAddr::NEXT_PAGEID)]:
          ram->pageid_stack.pop();
    }
  }
}

bool PushId(Ram* ram, T_id_addr addr, T_id id)
{
  switch (addr) {
  case IdAddr::BOOK: {
    if (ram->bookid_stack.contains(id)) return false;
    ram->bookid_stack.push(id);
    return true;
  }
  case IdAddr::FILE: {
    if (ram->fileid_stack.contains(id)) return false;
    ram->fileid_stack.push(id);
    return true;
  }
  case IdAddr::PAGE: {
    if (ram->pageid_stack.contains(id)) return false;
    ram->pageid_stack.push(id);
    return true;
  }
  }
}

}  // ns CPU
