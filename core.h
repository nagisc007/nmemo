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

#include <QObject>

namespace Nmemo {

/* class: Core */
class Core: public QObject
{
  Q_OBJECT
public:
  /* con[de]structor */
  explicit Core(QObject* parent = nullptr);
  ~Core();
  /* methods: features */
  T_idset UpdateData(T_cmd, T_index, const T_text&, T_arg);
  bool OutputData(T_tid, T_bid);
  /* functors: tabs */
  struct tabIndexToConv
  {
    T_tab_i operator ()(T_cmd, T_index, T_tid, const T_tabs*);
  };
  /* functors: books */
  struct bookIndexToConv
  {
    T_book_i operator ()(T_cmd, T_index, T_tid, T_bid, const T_books*);
  };
  /* operator: tabs */
  struct TabsDataUpdator
  {
    T_tid operator ()(T_cmd, T_tabs*, T_paths*, T_tab_i, T_arg);
  };
  struct TabsOverrider
  {
    T_tid operator ()(T_cmd, T_tabs*, T_tid, T_tab_i, T_arg);
  };
  struct PathsOverrider
  {
    T_tid operator ()(T_cmd, T_paths*, T_tid, T_arg);
  };
  struct TabsUpdator
  {
    T_tabs operator ()(T_cmd, T_tabs*, T_tid, T_tab_i, T_arg);
  };
  struct PathsUpdator
  {
    T_paths operator ()(T_cmd, T_paths*, T_tid, T_arg);
  };
  /* operator: books */
  struct BooksDataUpdator
  {
    T_bid operator ()(T_cmd, T_books*, T_labels*, T_tid, T_book_i, T_arg);
  };
  struct BooksOverrider
  {
    T_bid operator ()(T_cmd, T_books*, T_tid, T_bid, T_book_i, T_arg);
  };
  struct LabelsOverrider
  {
    T_bid operator ()(T_cmd, T_labels*, T_bid, T_arg);
  };
  struct BooksUpdator
  {
    T_books operator ()(T_cmd, T_books*, T_tid, T_bid, T_book_i, T_arg);
  };
  struct LabelsUpdator
  {
    T_labels operator ()(T_cmd, T_labels*, T_bid, T_arg);
  };
  /* operator: memos */
  struct MemosDataUpdator
  {
    T_bid operator ()(T_cmd, T_memos*, T_bid, T_text, T_bid, T_arg);
  };
  struct MemosOverrider
  {
    T_bid operator ()(T_cmd, T_memos*, T_bid, T_text, T_bid, T_arg);
  };
  struct MemosUpdator
  {
    T_memos operator ()(T_memos*, T_bid, T_text, T_bid, T_arg);
  };

signals:
  void tabOutputted(T_tab_i, T_tabnames);
  void booksOutputted(T_book_i, T_booknames);
  void memoOutputted(T_stat, T_memo);
  void filenameToSaveRequested();

public slots:
  void Update(T_cmd, T_index, const T_text&, T_arg);
  void LoadData(const T_fname&, T_tab_i, const T_text&);
  void SaveData(const T_fname&, T_tab_i, const T_text&);

private:
  int m_tid_;
  int m_bid_;
  QScopedPointer<Utl::IdUnit> idu_;
  QScopedPointer<T_idmap> m_books_;
  QScopedPointer<T_strmap> m_labels_;
  QScopedPointer<T_strmap> m_memos_;
  QScopedPointer<T_strmap> m_paths_;
  QScopedPointer<T_ids> m_tabs_;
};

/* defines */
using tabsToAdd = Utl::listToAdd<T_tid>;
using tabsToRemove = Utl::listToRemove<T_tid>;
using tabsToMove = Utl::listToMove<T_tid>;
using TabsToOverride = Utl::ListToOverride<T_tid>;
using tabNamesToConv = Utl::strListFromMapToConv<T_tid>;
using tabIdToFetch = Utl::listValToFetch<T_tid>;
using tabIndexToFetch= Utl::listIndexToFetch<T_tid>;

using pathsToUpdate = Utl::strMapToUpdate<T_tid>;
using pathsToRemove = Utl::strMapToRemove<T_tid>;
using PathsToOverride = Utl::StrMapToOverride<T_tid>;

using bookIdToFetch = Utl::listValToFetch<T_bid>;
using booksToAdd = Utl::listMapToAdd<T_tid, T_bid>;
using booksToRemove = Utl::listMapToRemove<T_tid, T_bid>;
using booksToMove = Utl::listMapToMove<T_tid, T_bid>;
using booksToDelete = Utl::listMapToRemoveByKey<T_tid, T_bid>;
using BooksToOverride = Utl::ListMapToOverride<T_tid, T_bid>;
using bookNamesToConv = Utl::strListFromMapToConv<T_bid>;
using bookToFetch = Utl::listMapListToFetch<T_tid, T_bid>;
using bookIndexToFetch = Utl::listIndexToFetch<T_bid>;

using labelsToUpdate = Utl::strMapToUpdate<T_bid>;
using labelsToRemove = Utl::strMapToRemove<T_bid>;
using LabelsToOverride = Utl::StrMapToOverride<T_bid>;
using memosToUpdate = Utl::strMapToUpdate<T_bid>;
using MemosToOverride = Utl::StrMapToOverride<T_bid>;
using memoToFetch = Utl::strMapValToFetch<T_bid>;

}  // namespace Nmemo

#endif // CORE_H
