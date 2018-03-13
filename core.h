/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef CORE_H
#define CORE_H

#include "core_utils.h"

#include <QObject>

namespace Nmemo {

/* class: Core */
class Core: public QObject
{
  Q_OBJECT
public:
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
  void asTabBarData(T_cmd, T_tab_i, T_tabnames, T_statset);
  void asBookListData(T_cmd, T_book_i, T_booknames);
  void asEditorData(T_cmd, T_stat, T_text);
  void asTitleData(T_cmd, T_title);
  void asStatusData(T_cmd, T_msg);
  void asFileData(T_cmd);

public slots:
  void ToTabData(T_cmd, T_tab_i, T_arg);
  void ToBookData(T_cmd, T_book_i, T_arg);
  void ToMemoData(T_cmd, const T_text&, T_stat);
  void ToFileData(T_cmd, T_arg);

private:
  /* members */
  QScopedPointer<T_ids> tids;
  QScopedPointer<T_statset> statset;
  QScopedPointer<T_bidsset> bidsset;
  QScopedPointer<T_strset> labels;
  QScopedPointer<T_memos> memos;
  /* caches */
  T_id m_tid;
  T_text m_text;
  QScopedPointer<T_bidset> m_bidset;
};

}  // namespace Nmemo

#endif // CORE_H
