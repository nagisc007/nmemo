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
  text(s1)
{}

void Page::Refresh(const T_str& s0, const T_str& s1)
{
  modified = true;
  label = s0;
  text = s1;
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
T_id bookIdOf(const Ram* ram, T_id fid, T_index idx)
{
  return fid >= 0 && idx < ram->files.at(fid)->book_ids.size() ?
        ram->files.at(fid)->book_ids.at(idx): -1;
}

T_ids bookIdsOf(const Ram* ram, T_id fid)
{
  return fid >= 0 ? ram->files.at(fid)->book_ids: T_ids();
}

T_index bookIndexOf(const Ram* ram, T_id fid, T_id bid)
{
  return fid >= 0 ? ram->files.at(fid)->book_ids.indexOf(bid): -1;
}

T_strs bookLabelsOf(const Ram* ram, T_id fid)
{
  T_strs res;
  if (fid < 0) return res;

  for (auto& id: ram->files.at(fid)->book_ids) {
    res << ram->books.at(id)->label;
  }
  return res;
}

T_states bookStatesOf(const Ram* ram, T_id fid)
{
  if (fid < 0) return T_states();

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

T_id currentBookId(const Ram* ram)
{
  auto cur_fid = currentFileId(ram);
  return cur_fid >= 0 ? ram->files.at(cur_fid)->current_bookid: -1;
}

T_id currentPageId(const Ram* ram)
{
  auto cur_bid = currentBookId(ram);
  return cur_bid >= 0 ? ram->books.at(cur_bid)->current_pageid: -1;
}

T_id fileIdOf(const Ram* ram, T_index idx)
{
  return idx >= 0 && idx < ram->file_ids.size() ? ram->file_ids.at(idx): -1;
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

T_id pageIdOf(const Ram* ram, T_id bid, T_index idx)
{
  return bid >= 0 && idx >= 0 && idx < ram->books.at(bid)->page_ids.size() ?
        ram->books.at(bid)->page_ids.at(idx): -1;
}

T_ids pageIdsOf(const Ram* ram, T_id bid)
{
  return bid >= 0 ? ram->books.at(bid)->page_ids: T_ids();
}

T_index pageIndexOf(const Ram* ram, T_id bid, T_id pid)
{
  return bid >= 0 ? ram->books.at(bid)->page_ids.indexOf(pid): -1;
}

T_strs pageLabelsOf(const Ram* ram, T_id bid)
{
  T_strs res;
  if (bid < 0) return res;

  for (auto& id: ram->books.at(bid)->page_ids) {
    res << ram->pages.at(id)->label;
  }
  return res;
}

T_states pageStatesOf(const Ram* ram, T_id bid)
{
  if (bid < 0) return T_states();

  T_states res(ram->books.at(bid)->page_ids.size());
  int idx = 0;
  for (auto& id: ram->books.at(bid)->page_ids) {
    res[idx] = ram->pages.at(id)->modified;
    ++idx;
  }
  return res;
}

T_strs pageTextsOf(const Ram* ram, T_id bid)
{
  T_strs res;
  for (auto& id: ram->books.at(bid)->page_ids) {
    res << ram->pages.at(id)->text;
  }
  return res;
}

T_str pageTextOf(const Ram* ram, T_id pid)
{
  return ram->pages.at(pid)->text;
}

bool IsExistsPath(const T_str& path)
{
  return QFileInfo(path).exists() &&
      QFileInfo(path).suffix() == COMMON::FILE_EXTENSION;
}

bool IsValidBookId(const Ram* ram, T_id fid, T_id bid)
{
  return bid >= 0 && bid < ram->books.size() &&
      ram->files.at(fid)->book_ids.contains(bid);
}

bool IsValidBookIndex(const Ram* ram, T_id fid, T_index idx)
{
  return idx >= 0 && idx < ram->files.at(fid)->book_ids.size();
}

bool IsValidFileId(const Ram* ram, T_id fid)
{
  return fid >= 0 && fid < ram->files.size()
      && ram->file_ids.contains(fid);
}

bool IsValidFileIndex(const Ram* ram, T_index idx)
{
  return idx >= 0 && idx < ram->files.size();
}

bool IsValidName(const T_str& s)
{
  return !s.isEmpty() && s != "";
}

bool IsValidPageId(const Ram* ram, T_id bid, T_id pid)
{
  return pid >= 0 && pid < ram->books.at(bid)->page_ids.size() &&
      ram->books.at(bid)->page_ids.contains(pid);
}

bool IsValidPageIndex(const Ram* ram, T_id bid, T_index idx)
{
  return idx >= 0 && idx < ram->books.at(bid)->page_ids.size();
}

bool IsValidPath(const T_str& path)
{
  return IsValidName(path) && QFileInfo(path).suffix() == COMMON::FILE_EXTENSION;
}

bool UpdateBookModified(Ram* ram, T_id bid, bool modified)
{
  ram->books.at(bid)->modified = modified;
  return true;
}

bool UpdateCurrentBookId(Ram* ram, T_id fid, T_id bid)
{
  if (fid < 0) return false;

  ram->files.at(fid)->current_bookid = bid;
  return true;
}

bool UpdateCurrentFileId(Ram* ram, T_id fid)
{
  ram->ireg[static_cast<int>(IRegAddr::CURRENT_FILEID)] = fid;
  return true;
}

bool UpdateCurrentPageId(Ram* ram, T_id bid, T_id pid)
{
  if (bid < 0) return false;

  ram->books.at(bid)->current_pageid = pid;
  return true;
}

bool UpdateFileModified(Ram* ram, T_id fid, bool modified)
{
  ram->files.at(fid)->modified = modified;
  return true;
}

bool UpdateFilePath(Ram* ram, T_id fid, const T_str& path)
{
  if (fid < 0) return false;

  ram->files.at(fid)->path = path;
  return true;
}

bool UpdatePageModified(Ram* ram, T_id pid, bool modified)
{
  ram->pages.at(pid)->modified = modified;
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
  if (fid < ram->files.size()) {
    ram->files.at(fid)->Refresh(name);
  } else {
    ram->files.append(new MemoFile(name));
  }
  return true;
}

bool AddPage(Ram* ram, T_id pid, const T_str& name, const T_str& text)
{
  if (pid < ram->pages.size()) {
    ram->pages.at(pid)->Refresh(name, text);
  } else {
    ram->pages.append(new Page(name, text));
  }
  return true;
}

bool AppendBookIds(Ram* ram, T_id fid, T_id bid)
{
  if (ram->files.at(fid)->book_ids.contains(bid)) return false;

  ram->files.at(fid)->book_ids.append(bid);
  return true;
}

bool AppendFileIds(Ram* ram, T_id fid)
{
  if (ram->file_ids.contains(fid)) return false;

  ram->file_ids.append(fid);
  return true;
}

bool AppendPageIds(Ram* ram, T_id bid, T_id pid)
{
  if (ram->books.at(bid)->page_ids.contains(pid)) return false;

  ram->books.at(bid)->page_ids.append(pid);
  return true;
}

bool MoveBook(Ram* ram, T_id fid, T_index from, T_index to)
{
  ram->files.at(fid)->book_ids.move(from, to);
  return true;
}

bool MoveFile(Ram* ram, T_index from, T_index to)
{
  ram->file_ids.move(from, to);
  return true;
}

bool MovePage(Ram* ram, T_id bid, T_index from, T_index to)
{
  ram->books.at(bid)->page_ids.move(from, to);
  return true;
}

bool RemoveBookIds(Ram* ram, T_id fid, T_id bid)
{
  if (!ram->files.at(fid)->book_ids.contains(bid)) return false;

  return ram->files.at(fid)->book_ids.removeOne(bid);
}

bool RemoveFileIds(Ram* ram, T_id fid)
{
  if (!ram->file_ids.contains(fid)) return false;

  return ram->file_ids.removeOne(fid);
}

bool RemovePageIds(Ram* ram, T_id bid, T_id pid)
{
  if (!ram->books.at(bid)->page_ids.contains(pid)) return false;

  return ram->books.at(bid)->page_ids.removeOne(pid);
}

bool RenameBook(Ram* ram, T_id bid, const T_str& name)
{
  ram->books.at(bid)->label = name;
  return true;
}

bool RenameFile(Ram* ram, T_id fid, const T_str& name)
{
  ram->files.at(fid)->path = name;
  return true;
}

bool RenamePage(Ram* ram, T_id pid, const T_str& name)
{
  ram->pages.at(pid)->label = name;
  return true;
}

bool SortBooks(Ram* ram, T_id fid, T_order order)
{
  auto origin = ram->files.at(fid)->book_ids;
  T_strs labels = bookLabelsOf(ram, fid);
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
  ram->files.at(fid)->book_ids = ids;
  return true;
}

bool SortPages(Ram* ram, T_id bid, T_order order)
{
  auto origin = ram->books.at(bid)->page_ids;
  T_strs labels = pageLabelsOf(ram, bid);
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
  return -1;
}

bool PushId(Ram* ram, T_id_addr addr, T_id id)
{
  switch (addr) {
  case IdAddr::BOOK:
    ram->bookid_stack.push(id);
    return true;
  case IdAddr::FILE:
    ram->fileid_stack.push(id);
    return true;
  case IdAddr::PAGE:
    ram->pageid_stack.push(id);
    return true;
  }
  return false;
}

}  // ns CPU
