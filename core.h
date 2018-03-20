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
  T_stat r_enabled;
  QScopedPointer<T_pidset> r_pidset;
  QScopedPointer<T_statset> r_savedset;
  /* members: utils */
  T_id u_nextid;
  QScopedPointer<QStack<T_id>> u_idpool;
  /* methods: output */
  void OutToTabBar(const T_cmd);
  void OutToPageList(const T_cmd);
  void OutToEditor(const T_cmd);
  void OutToTitleBar(const T_cmd);
  void OutToStatusBar(const T_cmd);
  /* methods: Data */
  T_cmd ToFileData(const T_cmd, const T_arg, const T_arg, const T_arg);
  T_cmd ToBookData(const T_cmd, const T_arg, const T_arg, const T_arg);
  T_cmd ToPageData(const T_cmd, const T_arg, const T_arg, const T_arg);
  T_cmd ToNoteData(const T_cmd, const T_arg, const T_arg, const T_arg);
  /* methods: File */
  T_cmd FileToLoad(const T_path&);
  T_cmd FileToSave(const T_path&);
  T_cmd FileToUpdate();
  /* methods: Book */
  T_cmd BookToAdd(const T_name&);
  T_cmd BookToDelete(const T_index);
  T_cmd BookToChange(const T_index);
  T_cmd BookToMove(const T_index, const T_index);
  T_cmd BookToRename(const T_index, const T_name&);
  /* methods: Page */
  T_cmd PageToAdd(const T_bid, const T_name&, const T_note&);
  T_cmd PageToDelete(const T_bid, const T_index);
  T_cmd PageToChange(const T_bid, const T_index);
  T_cmd PageToMove(const T_bid, const T_index, const T_index);
  T_cmd PageToRename(const T_bid, const T_index, const T_name&);
  T_cmd PageToSort(const T_bid, const T_order);
  /* methods: Note */
  T_cmd NoteToAdd(const T_pid, const T_note&);
  T_cmd NoteToDelete(const T_pid);
  T_cmd NoteToCache(const T_note&);
  T_cmd NoteToModify();
  T_cmd NoteToUpdate(const T_pid);

signals:
  void asTabBarData(T_cmd, T_arg, T_arg, T_arg);
  void asPageListData(T_cmd, T_arg, T_arg);
  void asEditorData(T_cmd, T_arg, T_arg);
  void asTitleBarData(T_cmd, T_arg);
  void asStatusBarData(T_cmd, T_arg, T_arg);
  void asFileNameRequest();

public slots:
  void ToSystemData(const T_cmd, const T_arg, const T_arg, const T_arg);
};

/* process: Core Process */
namespace CP {

/* CP: File */
namespace File {

namespace Path {

T_name FetchBase(const T_pathset*, const T_bid);
T_path Valid(const T_path&, const T_ext&);

}  // ns CP::File::Path

namespace Data {

T_encoded Encode(const T_text&, const T_text&, const T_pagenames*, const T_notes*);
T_decoded Decode(const T_text&, const T_text&, const T_strs*);

}  // ns CP::File::Data

namespace States {

QList<QVariant> Convert(const T_stats*);
T_stats Filter(const T_statset*, const T_bids*);
T_statset Add(const T_statset*, const T_bid, const T_stat);
T_statset Delete(const T_statset*, const T_bid);
T_statset Edit(const T_statset*, const T_bid, const T_stat);
bool Merge(T_statset*, T_statset&);

}  // ns CP::File::States

}  // ns CP::File

/* CP: Book */
namespace Book {

namespace Id {

T_bid Fetch(const T_ids*, const T_index);

}  // ns CP::Book::Id

namespace Index {

T_index Fetch(const T_ids*, const T_id);

}  // ns CP::Book::Index

namespace CurrentId {

T_bid Fetch(const System*);
bool Merge(T_bid*, const T_bid);

}  // ns CP::Book::CurrentId

namespace Ids {

T_bids Add(const T_bids*, const T_bid);
T_bids Delete(const T_bids*, const T_bid);
T_bids Move(const T_bids*, const T_index, const T_index);
bool Merge(T_bids*, T_bids&);

}  // ns CP::Book::Ids

namespace Names {

T_tabnames Filter(const T_pathset*, const T_bids*);
T_pathset Add(const T_pathset*, const T_bid, const T_path&);
T_pathset Delete(const T_pathset*, const T_bid);
T_pathset Edit(const T_pathset*, const T_bid, const T_path&);
bool Merge(T_pathset*, T_pathset&);

}  // ns CP::Book::Names

}  // ns CP::Book

/* CP: Page */
namespace Page {

namespace Id {

T_pid Fetch(const T_pidsset*, const T_bid, const T_index);

}  // ns CP::Page::Id

namespace Index {

T_index Fetch(const T_pidsset*, const T_bid, const T_pid);

}  // ns CP::Page::Index

namespace Ids {

T_pids Fetch(const T_pidsset*, const T_bid);
T_pids Add(const T_pidsset*, const T_bid, const T_pid);
T_pids Delete(const T_pidsset*, const T_bid, const T_pid);
T_pids Move(const T_pidsset*, const T_bid, const T_index, const T_index);
bool Merge(T_pidsset*, const T_bid, T_pids&);
bool DeleteAll(T_pidsset*, const T_bid);

}  // ns CP::Page::Ids

namespace Names {

T_pagenames Filter(const T_nameset*, const T_pids*);
T_nameset Add(const T_nameset*, const T_pid, const T_name&);
T_nameset Delete(const T_nameset*, const T_pid);
T_nameset Edit(const T_nameset*, const T_pid, const T_name&);
bool Merge(T_nameset*, T_nameset&);

}  // ns CP::Page::Names

namespace CurrentId {

T_pid Fetch(const T_pidset*, const T_bid);

}  // ns CP::Page::CurrentId

namespace CurrentIds {

T_pids Filter(const T_pidset*, const T_bids*);
T_pidset Add(const T_pidset*, const T_bid, const T_pid);
T_pidset Delete(const T_pidset*, const T_bid);
T_pidset Edit(const T_pidset*, const T_bid, const T_pid);
bool Merge(T_pidset*, T_pidset&);

}  // ns CP::Page::CurrentIds

}  // ns CP::Page

/* CP: Note */
namespace Note {

namespace Text {

T_note Fetch(const T_noteset*, const T_pid);

}  // ns CP::Note::Text

namespace Texts {

T_notes Filter(const T_noteset*, const T_pids*);
T_noteset Add(const T_noteset*, const T_pid, const T_note&);
T_noteset Delete(const T_noteset*, const T_pid);
T_noteset Edit(const T_noteset*, const T_pid, const T_note&);
bool Merge(T_noteset*, T_noteset&);

}  // CP::Note::Texts

}  // ns CP::Note

}  // ns CP

}  // ns Core

#endif // CORE_H
