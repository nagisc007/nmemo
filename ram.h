/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef RAM_H
#define RAM_H

#include "common_types.h"
#include "common_values.h"

#include <QStack>

namespace CPU {

enum class IdAddr {
  FILE,
  BOOK,
  PAGE,
};

using T_id_addr = IdAddr;

struct MemoFile
{
  MemoFile(const T_str&);
  // members
  bool modified;
  T_str path;
  T_id current_bookid;
  QVector<int> book_ids;
  // methods
  void Refresh(const T_str&);
};

struct Book
{
  Book(const T_str&);
  // members
  bool modified;
  T_str label;
  T_id current_pageid;
  QVector<int> page_ids;
  // methods
  void Refresh(const T_str&);
};

struct Page
{
  Page(const T_str&, const T_str&);
  // members
  bool modified;
  T_str label;
  T_str text;
  // methods
  void Refresh(const T_str&, const T_str&);
};

struct Ram
{
  Ram();
  // members
  QVector<int> ireg;
  QVector<int> file_ids;
  QVector<MemoFile*> files;
  QVector<Book*> books;
  QVector<Page*> pages;
  QStack<T_id> fileid_stack;
  QStack<T_id> bookid_stack;
  QStack<T_id> pageid_stack;
  // methods
  void Reset();
};

enum class IRegAddr {
  CURRENT_FILEID,
  NEXT_FILEID,
  NEXT_BOOKID,
  NEXT_PAGEID,
};
const int IREG_SIZE = 4;

// controls
T_id currentFileId(const Ram*);
T_id currentBookId(const Ram*);
T_id currentPageId(const Ram*);
T_id fileIdOf(const Ram*, T_index);
T_id bookIdOf(const Ram*, T_id, T_index);
T_id pageIdOf(const Ram*, T_id, T_index);
T_index fileIndexOf(const Ram*, T_id);
T_index bookIndexOf(const Ram*, T_id, T_id);
T_index pageIndexOf(const Ram*, T_id, T_id);
T_strs fileLabelsOf(const Ram*);
T_strs bookLabelsOf(const Ram*, T_id);
T_strs pageLabelsOf(const Ram*, T_id);
T_strs pageTextsOf(const Ram*, T_id);
T_str pageTextOf(const Ram*, T_id);
T_states fileStatesOf(const Ram*);
T_states bookStatesOf(const Ram*, T_id);
T_states pageStatesOf(const Ram*, T_id);
bool IsValidFileId(const Ram*, T_id);
bool IsValidBookId(const Ram*, T_id, T_id);
bool IsValidPageId(const Ram*, T_id, T_id);
bool IsValidFileIndex(const Ram*, T_index);
bool IsValidBookIndex(const Ram*, T_id, T_index);
bool IsValidPageIndex(const Ram*, T_id, T_index);
bool IsValidPath(const T_str&);
bool IsExistsPath(const T_str&);
bool IsValidName(const T_str&);
bool UpdateCurrentFileId(Ram*, T_id);
bool UpdateCurrentBookId(Ram*, T_id, T_id);
bool UpdateCurrentPageId(Ram*, T_id, T_id);
bool UpdateFileModified(Ram*, T_id, bool);
bool UpdateBookModified(Ram*, T_id, bool);
bool UpdatePageModified(Ram*, T_id, bool);
bool AppendFileIds(Ram*, T_id);
bool RemoveFileIds(Ram*, T_id);
bool AddFile(Ram*, T_id, const T_str&);
bool CloseFile(Ram*, T_id);
bool MoveFile(Ram*, T_index, T_index);
bool RenameFile(Ram*, T_id, const T_str&);
bool AppendBookIds(Ram*, T_id, T_id);
bool RemoveBookIds(Ram*, T_id, T_id);
bool AddBook(Ram*, T_id, const T_str&);
bool MoveBook(Ram*, T_id, T_index, T_index);
bool RemoveBook(Ram*, T_id, T_id);
bool RenameBook(Ram*, T_id, const T_str&);
bool SortBooks(Ram*, T_id, T_order);
bool AppendPageIds(Ram*, T_id, T_id);
bool RemovePageIds(Ram*, T_id, T_id);
bool AddPage(Ram*, T_id, const T_str&, const T_str&);
bool MovePage(Ram*, T_id, T_index, T_index);
bool RemovePage(Ram*, T_id, T_id);
bool RenamePage(Ram*, T_id, const T_str&);
bool SortPages(Ram*, T_id, T_order);
T_id PopId(Ram*, T_id_addr);
bool PushId(Ram*, T_id_addr, T_id);

}  // ns CPU

#endif // RAM_H
