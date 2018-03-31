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

namespace Mem {

/* class: Book */
struct Book {
  explicit Book();
  ~Book();
  // memory
  QScopedPointer<T_ids> m_bids;
  QScopedPointer<T_strset> m_pathset;
  // register
  T_bid r_bid;
  QScopedPointer<T_statset> r_savedset;
  // props
  inline T_ids* bids() const {
    return m_bids.data();
  }
  inline T_strset* pathset() const {
    return m_pathset.data();
  }
  inline T_bid currentBid() const {
    return r_bid;
  }
  inline T_statset* savedset() const {
    return r_savedset.data();
  }// methods
  inline bool ToMergeCurrentBid(const T_bid bid) {
    r_bid = bid;
    return true;
  }
};

/* class: Page */
struct Page {
  explicit Page();
  ~Page();
  // memory
  QScopedPointer<T_pidsset> m_pidsset;
  QScopedPointer<T_strset> m_nameset;
  QScopedPointer<T_strset> m_noteset;
  QScopedPointer<T_modeset> m_modeset;
  // register
  T_msgtime r_msg_show_time;
  QScopedPointer<T_pidset> r_pidset;
  QScopedPointer<T_posset> r_posset;
  // props
  inline T_pidsset* pidsset() const {
    return m_pidsset.data();
  }
  inline T_strset* nameset() const {
    return m_nameset.data();
  }
  inline T_strset* noteset() const {
    return m_noteset.data();
  }
  inline T_modeset* modeset() const {
    return m_modeset.data();
  }
  inline T_msgtime msg_show_time() const {
    return r_msg_show_time;
  }
  inline T_pidset* pidset() const {
    return r_pidset.data();
  }
  inline T_posset* posset() const {
    return r_posset.data();
  }
};

}  // ns Mem

namespace Core {

/* class: System */
class System: public QObject
{
  Q_OBJECT
public:
  explicit System(QObject* parent = nullptr);
  ~System();
  /* members: class */
  QScopedPointer<Mem::Book> m_books;
  QScopedPointer<Mem::Page> m_pages;
  /* members: register */
  T_note r_note;
  T_pos r_pos;
  T_stat r_enabled;
  /* members: utils */
  T_id u_nextid;
  QScopedPointer<QStack<T_id>> u_idpool;
  /* methods: output */
  void ToOutputs(const T_sig, const T_code);
  void OutToTabBar(const T_sig);
  void OutToPageList(const T_sig);
  void OutToEditor(const T_sig);
  void OutToTitleBar(const T_sig);
  void OutToStatusBar(const T_msg&);
  /* methods: Data */
  T_sig BranchToData(const T_code, const T_arg, const T_arg, const T_arg);
  /* methods: File */
  T_sig FileToLoad(const T_path&);
  T_sig FileToSave(const T_bid);
  T_sig FileToSaveAs(const T_bid, const T_path&);
  T_sig FileToUpdate(const T_bid, const T_stat);
  /* methods: Book */
  T_sig BookToAdd(const T_name&);
  T_sig BookToDelete(const T_index);
  T_sig BookToChange(const T_index);
  T_sig BookToMove(const T_index, const T_index);
  T_sig BookToRename(const T_index, const T_name&);
  /* methods: Page */
  T_sig PageToAdd(const T_bid, const T_name&, const T_note&);
  T_sig PageToDelete(const T_bid, const T_index);
  T_sig PageToChange(const T_bid, const T_index);
  T_sig PageToMove(const T_bid, const T_index, const T_index);
  T_sig PageToRename(const T_bid, const T_index, const T_name&);
  T_sig PageToSort(const T_bid, const T_order);
  /* methods: Note */
  T_sig NoteToAdd(const T_pid, const T_note&);
  T_sig NoteToDelete(const T_pid);
  T_sig NoteToCache(const T_note&, const T_pos);
  T_sig NoteToChangeMode(const T_bid, const T_mode);
  T_sig NoteToModify(const T_bid);
  T_sig NoteToUpdate(const T_pid);
  /* utils */
  inline bool existsCurrentBook() {
    return (m_books->currentBid() > 0);
  }

signals:
  void asTabBarData(T_sig, T_arg, T_arg, T_arg);
  void asPageListData(T_sig, T_arg, T_arg);
  void asEditorData(T_sig, T_arg, T_arg, T_arg, T_arg);
  void asTitleBarData(T_sig, T_arg);
  void asStatusBarData(T_sig, T_arg, T_arg);
  void asFileNameRequest();

public slots:
  void ToSystemData(const T_code, const T_arg, const T_arg, const T_arg);
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
T_strs Load(const T_path&);
bool Save(const T_path&, const T_encoded*);

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

inline T_index indexValidated(const T_ids* ids, const T_index i) {
  if (ids->count() <= 0) return -1;
  return i < 0 ? 0: i;
}

namespace Id {

T_bid Fetch(const T_ids*, const T_index);

}  // ns CP::Book::Id

namespace Index {

T_index Fetch(const T_ids*, const T_id);

}  // ns CP::Book::Index

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
T_pids Sort(const T_pids*, const T_nameset*, const T_order);
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

namespace Pos {

T_pos Fetch(const T_posset*, const T_pid);
T_posset Add(const T_posset*, const T_pid, const T_pos);
T_posset Delete(const T_posset*, const T_pid);
T_posset Edit(const T_posset*, const T_pid, const T_pos);
bool Merge(T_posset*, T_posset&);

}  // ns CP::Note::Pos

namespace Mode {

T_mode Fetch(const T_modeset*, const T_pid);
T_modeset Add(const T_modeset*, const T_pid, const T_mode);
T_modeset Delete(const T_modeset*, const T_pid);
T_modeset Edit(const T_modeset*, const T_pid, const T_mode);
bool Merge(T_modeset*, T_modeset&);

}  // ns CP::Note::Mode

}  // ns CP::Note

}  // ns CP

}  // ns Core

#endif // CORE_H
