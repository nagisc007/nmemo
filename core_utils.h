/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef CORE_UTILS_H
#define CORE_UTILS_H

#include "utils.h"
#include "common_values.h"

namespace Nmemo {

/* functors: tabs */
struct TabsToAdd
{
  T_tid operator ()(T_tids*, T_labels*, const T_name&);
};

struct TabsToRemove
{
  T_tid operator ()(T_tids*, T_labels*, T_books*, T_tab_i);
};

struct TabsToMove
{
  T_tid operator ()(T_tids*, T_tab_i, T_index);
};

struct TabsToRename
{
  T_tid operator ()(const T_tids*, T_labels*, T_tab_i, const T_name&);
};

struct TabNamesToConvertFromPaths
{
  T_slist operator ()(const T_slist*);
};

/* functors: books */
struct BooksToAdd
{
  T_bid operator ()(T_books*, T_labels*, T_memos*, T_tid, const T_name&, const T_text&);
};

struct BooksToRemove
{
  T_bid operator ()(T_books*, T_labels*, T_tid, T_book_i);
};

struct BooksToMove
{
  T_bid operator ()(T_books*, T_tid, T_book_i, T_index);
};

struct BooksToRename
{
  T_bid operator ()(const T_books*, T_labels*, T_tid, T_book_i, const T_name&);
};

struct BooksToSort
{
  T_bid operator ()(T_books*, const T_labels*, T_tid, T_order);
};

struct BookIdsToRelease
{
  bool operator ()(const T_books*, T_tid);
};

/* defines */
using tabIdFetched = Utl::listValFetched<T_tid>;
using tabIndexFetched = Utl::listIndexFetched<T_tid>;
using tidsAdded = Utl::listAdded<T_tid>;
using tidsRemoved = Utl::listRemoved<T_tid>;
using tidsMoved = Utl::listMoved<T_tid>;
using TidsToMerge = Utl::ListToMerge<T_tid>;

using bookIdFetched = Utl::listMapValFetched<T_tid, T_bid>;
using bookIndexFetched = Utl::listMapIndexFetched<T_tid, T_bid>;
using bidsFetched = Utl::listMapListFetched<T_tid, T_bid>;
using booksAdded = Utl::listMapAdded<T_tid, T_bid>;
using booksRemoved = Utl::listMapRemoved<T_tid, T_bid>;
using booksMoved = Utl::listMapMoved<T_tid, T_bid>;
using booksRemovedTid = Utl::listMapRemovedKey<T_tid, T_bid>;
using BooksToMerge = Utl::ListMapToMerge<T_tid, T_bid>;

using labelFetched = Utl::strMapValFetched<T_id>;
using labelsUpdated = Utl::strMapUpdated<T_id>;
using labelsRemoved = Utl::strMapRemoved<T_id>;
using LabelsToMerge = Utl::StrMapToMerge<T_id>;

using memoFetched = Utl::strMapValFetched<T_bid>;
using memosUpdated = Utl::strMapUpdated<T_bid>;
using MemosToMerge = Utl::StrMapToMerge<T_bid>;

using tabNamesConverted = Utl::strListConvertedFromMap<T_tid>;
using bookNamesConverted = Utl::strListConvertedFromMap<T_bid>;
using memosConverted = Utl::strListConvertedFromMap<T_bid>;

}  // namespace Nmemo

#endif // CORE_UTILS_H
