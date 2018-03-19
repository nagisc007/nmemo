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
#include <QStack>

namespace Core {

/* class: System */
class System: public QObject
{
  Q_OBJECT
public:
  explicit System(QObject* parent = nullptr);
  ~System();
  /* members: memory */
  QScopedPointer<T_ids> m_bids;
  QScopedPointer<T_strset> m_pathset;
  QScopedPointer<T_pidsset> m_pidsset;
  QScopedPointer<T_strset> m_nameset;
  QScopedPointer<T_strset> m_noteset;
  /* members: register */
  T_bid r_bid;
  T_note r_note;
  QScopedPointer<T_pidset> r_pidset;
  /* members: utils */
  T_id u_nextid;
  QScopedPointer<QStack<T_id>> u_idpool;
  /* process: output */
  void OutToTabBar();
  void OutToPageList();
  void OutToEditor();
  void OutToTitleBar();
  void OutToStatusBar();

signals:
  void asTabBarData(T_cmd, T_arg, T_arg, T_arg);
  void asPageListData(T_cmd, T_arg, T_arg);
  void asEditorData(T_cmd, T_arg, T_arg);
  void asTitleBarData(T_cmd, T_arg);
  void asStatusBarData(T_cmd, T_arg, T_arg);
  void asFileNameRequest();

public slots:
  void ToSystemData(T_cmd, T_arg, T_arg, T_arg);
};

/* process: Core Process */
namespace CP {

/* CP: Book */
namespace Book {

}  // ns CP::Book

/* CP: Page */
namespace Page {

}  // ns CP::Page

/* CP: Note */
namespace Note {

}  // ns CP::Note

}  // ns CP

}  // ns Core

#endif // CORE_H
