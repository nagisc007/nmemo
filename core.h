/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef CORE_H
#define CORE_H

#include "books.h"
#include "files.h"
#include "memos.h"
#include "tabs.h"

#include <QObject>
#include <QStack>

namespace Nmemo {

/* class: Core */
class Core: public QObject
{
  Q_OBJECT
public:
  explicit Core(QObject* parent = nullptr);
  ~Core();
  /* members */
  QScopedPointer<T_ids> m_tids;
  QScopedPointer<T_statset> m_statset;
  QScopedPointer<T_bidsset> m_bidsset;
  QScopedPointer<T_strset> m_labels;
  QScopedPointer<T_memos> m_memos;
  T_tid m_tid;
  T_text m_text;
  QScopedPointer<T_bidset> m_bidset;
  T_stat m_editor_enabled;
  T_id m_next_id;
  QScopedPointer<QStack<T_id>> m_idpool;
  /* process: output */
  void OutputTabBar();
  void OutputBookList();
  void OutputEditor();

signals:
  void asTabBarData(T_cmd, T_tab_i, T_tabnames, T_stats);
  void asBookListData(T_cmd, T_book_i, T_booknames);
  void asEditorData(T_cmd, T_stat, T_text);
  void asTitleData(T_cmd, T_title);
  void asStatusData(T_cmd, T_msg);
  void asFileData();  // currently saveas to request

public slots:
  void ToTabData(T_cmd, T_tab_i, T_arg);
  void ToBookData(T_cmd, T_book_i, T_arg);
  void ToMemoData(T_cmd, const T_text&, T_stat);
  void ToFileData(T_cmd, T_arg);
};

namespace CoreUtl {

bool TabVerify(const Core*);
bool BookVerify(const Core*);

}  // ns CoreUtl

}  // namespace Nmemo

#endif // CORE_H
