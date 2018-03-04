/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef CORE_H
#define CORE_H

#include "utils.h"
#include "common_values.h"

#include <QObject>

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

struct BookIdsToRelease
{
  bool operator ()(const T_books*, T_tid);
};

/* class: Core */
class Core: public QObject
{
  Q_OBJECT
public:
  /* con[de]structor */
  explicit Core(QObject* parent = nullptr);
  ~Core();
  /* methods: features */
  T_index tabIndexExisted(const T_fname&);
  bool UpdateData(T_cmd, T_index, const T_text&, T_arg);
  bool OutputData(T_tid, T_bid);
  bool UpdateTabData(T_cmd, T_tab_i, T_arg);
  bool UpdateBookData(T_cmd, T_book_i, T_arg,
                      const T_text& = "new text");
  bool UpdateMemoData(T_bid, T_text);
  T_slist EncodeData(const T_slist*, const T_slist*);
  QPair<T_slist, T_slist> DecodeData(const T_slist*);

signals:
  void tabOutputted(T_tab_i, T_tabnames);
  void booksOutputted(T_book_i, T_booknames);
  void memoOutputted(T_stat, T_memo);
  void filenameToSaveRequested();
  void statusUpdated(T_text);
  void fileUpdated(T_fname);

public slots:
  void Update(T_cmd, T_index, const T_text&, T_arg);
  void LoadData(const T_fname&, T_tab_i, const T_text&);
  void SaveData(const T_fname&, T_tab_i, const T_text&);

private:
  int m_tid_;
  int m_bid_;
  QScopedPointer<T_lmap> m_books_;
  QScopedPointer<T_smap> m_labels_;
  QScopedPointer<T_smap> m_memos_;
  QScopedPointer<T_ids> m_tids_;
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

#endif // CORE_H
