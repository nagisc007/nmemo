/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "core.h"

namespace Mem {

/* class: Book */
Book::Book():
  // memory
  m_bids(new T_ids()),
  m_pathset(new T_strset()),
  // register
  r_bid(-1),
  r_savedset(new T_statset())
{
  qDebug() << "Mem::Book: constructed";
}

Book::~Book()
{
  if (m_bids) {
    m_bids->clear();
    m_bids.reset();
  }
  if (m_pathset) {
    m_pathset->clear();
    m_pathset.reset();
  }
  if (r_savedset) {
    r_savedset->clear();
    r_savedset.reset();
  }
  qDebug() << "Mem::Book: destructed";
}

/* class: Page */
Page::Page():
  // memory
  m_pidsset(new T_pidsset()),
  m_nameset(new T_strset()),
  m_noteset(new T_strset()),
  m_modeset(new T_modeset()),
  // register
  r_msg_show_time(Nmemo::VALUE::STATUS_MESSAGE_TIME),
  r_pidset(new T_pidset()),
  r_posset(new T_posset())
{
  qDebug() << "Mem::Page: constructed";
}

Page::~Page()
{
  if (m_pidsset) {
    m_pidsset->clear();
    m_pidsset.reset();
  }
  if (m_nameset) {
    m_nameset->clear();
    m_nameset.reset();
  }
  if (m_noteset) {
    m_noteset->clear();
    m_noteset.reset();
  }
  if (m_modeset) {
    m_modeset->clear();
    m_modeset.reset();
  }
  if (r_pidset) {
    r_pidset->clear();
    r_pidset.reset();
  }
  qDebug() << "Mem::Page: destructed";
}

}  // ns Mem

namespace Core {
/* process: utils */
namespace {

auto statusMessageBy(const T_code code) -> T_msg
{
  switch (code) {
  case OpCode::FILE_LOAD: return T_msg("File loaded ...");
  case OpCode::FILE_SAVE:
  case OpCode::FILE_SAVEAS: return T_msg("File saved ...");
  case OpCode::BOOK_ADD: return T_msg("Book added ...");
  case OpCode::BOOK_DELETE: return T_msg("Book removed ...");
  case OpCode::BOOK_MOVE: return T_msg("Book moved ...");
  case OpCode::BOOK_RENAME: return T_msg("Book renamed ...");
  case OpCode::PAGE_ADD: return T_msg("Page added ...");
  case OpCode::PAGE_DELETE: return T_msg("Page removed ...");
  case OpCode::PAGE_MOVE: return T_msg("Page moved ...");
  case OpCode::PAGE_RENAME: return T_msg("Page renamed ...");
  case OpCode::BOOK_CHANGE:
  case OpCode::PAGE_CHANGE:
  default: return T_msg("");
  }
}

auto isInvalidPath(const T_path& path) -> bool
{
  return (path.isEmpty() || path == "" || path == Nmemo::VALUE::DEFAULT_FILENAME);
}

auto isExistedPath(const T_pathset* pathset, const T_path& path) -> bool
{
  QHashIterator<T_bid, T_path> it(*pathset);
  while (it.hasNext()) {
    it.next();
    if (it.value() == path) {
      return true;
    }
  }
  return false;
}

}  // ns Core(anonymous)

/* class: System */
System::System(QObject* parent): QObject(parent),
  // class
  m_books(new Mem::Book()),
  m_pages(new Mem::Page()),
  // register
  r_note(""),
  r_pos(0),
  r_enabled(false),
  // utils
  u_nextid(0),
  u_idpool(new QStack<T_id>())
{
  qDebug() << "System: constructed";
}

System::~System()
{
  if (u_idpool) {
    u_idpool->clear();
    u_idpool.reset();
  }
  qDebug() << "System: destructed";
}

/* methods: output */
auto System::ToOutputs(const T_sig sig, const T_code code) -> void
{
  if (sig == Sig::NOP) return;

  if (Utl::Sig::Exists(sig, Sig::TAB)) OutToTabBar(sig);
  if (Utl::Sig::Exists(sig, Sig::LIST)) OutToPageList(sig);
  if (Utl::Sig::Exists(sig, Sig::EDITOR_ALL)) OutToEditor(sig);
  if (Utl::Sig::Exists(sig, Sig::TITLE)) OutToTitleBar(sig);

  OutToStatusBar(statusMessageBy(code));
}

auto System::OutToTabBar(const T_sig sig) -> void
{
  auto stats = CP::File::States::Filter(m_books->savedset(), m_books->bids());
  emit asTabBarData(sig,
                    QVariant(CP::Book::Index::Fetch(m_books->bids(),
                                                    m_books->currentBid())),
                    QVariant(CP::Book::Names::Filter(m_books->pathset(), m_books->bids())),
                    QVariant(CP::File::States::Convert(&stats)));
}

auto System::OutToPageList(const T_sig sig) -> void
{
  auto bid = m_books->currentBid();
  auto pids = CP::Page::Ids::Fetch(m_pages->pidsset(), bid);
  emit asPageListData(sig,
                      QVariant(CP::Page::Index::Fetch(
                                 m_pages->pidsset(),
                                 bid,
                                 CP::Page::CurrentId::Fetch(m_pages->pidset(), bid))),
                      QVariant(CP::Page::Names::Filter(
                                 m_pages->nameset(),
                                 &pids)));
}

auto System::OutToEditor(const T_sig sig) -> void
{
  auto current_pid = CP::Page::CurrentId::Fetch(
        m_pages->pidset(), m_books->currentBid());
  r_enabled = current_pid > 0;
  emit asEditorData(sig,
                    QVariant(CP::Note::Text::Fetch(
                               m_pages->noteset(), current_pid)),
                    QVariant(static_cast<int>(CP::Note::Mode::Fetch(
                                                m_pages->modeset(), current_pid))),
                    QVariant(CP::Note::Pos::Fetch(m_pages->posset(), current_pid)),
                    QVariant(r_enabled));
}

auto System::OutToStatusBar(const T_msg& msg) -> void
{
  if (msg == "") return;

  emit asStatusBarData(Sig::STATUS_MESSAGE, QVariant(msg),
                       QVariant(m_pages->msg_show_time()));
}

auto System::OutToTitleBar(const T_sig sig) -> void
{
  emit asTitleBarData(sig,
                      QVariant(CP::File::Path::FetchBase(m_books->pathset(),
                                                         m_books->currentBid())));
}

/* methods: Data */
auto System::BranchToData(const T_code code, const T_arg arg0,
                          const T_arg arg1, const T_arg arg2) -> T_sig
{
  Q_UNUSED(arg2);
  if (!existsCurrentBook() &&
      (code != OpCode::BOOK_ADD && code != OpCode::FILE_LOAD)) {
      return Sig::NOP;
  }

  auto bid = m_books->currentBid();
  switch (code) {
  case OpCode::FILE_LOAD: return FileToLoad(arg0.toString());
  case OpCode::FILE_SAVE: return FileToSave(bid);
  case OpCode::FILE_SAVEAS: return FileToSaveAs(bid, arg0.toString());
  case OpCode::BOOK_ADD: return BookToAdd(arg0.toString());
  case OpCode::BOOK_CHANGE: return BookToChange(arg0.toInt());
  case OpCode::BOOK_DELETE: return BookToDelete(arg0.toInt());
  case OpCode::BOOK_MOVE: return BookToMove(arg0.toInt(), arg1.toInt());
  case OpCode::BOOK_RENAME: return BookToRename(arg0.toInt(), arg1.toString());
  case OpCode::PAGE_ADD:
    return PageToAdd(bid, arg0.toString(), Nmemo::VALUE::DEFAULT_EDITOR_TEXT);
  case OpCode::PAGE_CHANGE: return PageToChange(bid, arg0.toInt());
  case OpCode::PAGE_DELETE: return PageToDelete(bid, arg0.toInt());
  case OpCode::PAGE_MOVE: return PageToMove(bid, arg0.toInt(), arg1.toInt());
  case OpCode::PAGE_RENAME: return PageToRename(bid, arg0.toInt(), arg0.toString());
  case OpCode::PAGE_SORT: return PageToSort(bid, static_cast<Qt::SortOrder>(arg0.toInt()));
  case OpCode::NOTE_CACHE:
    return NoteToCache(arg0.toString(), arg1.toInt());
  case OpCode::NOTE_CHANGE_MODE:
    return NoteToChangeMode(bid, static_cast<EditMode>(arg0.toInt()));
  case OpCode::NOTE_MODIFY: return NoteToModify(bid);
  default:
    return Sig::NOP;
  }
}

/* methods: File */
auto System::FileToLoad(const T_path& path) -> T_sig
{
  if (isInvalidPath(path)) return Sig::NOP;

  auto va_path = CP::File::Path::Valid(path, Nmemo::VALUE::FILE_EXT);

  // check already path
  if (isExistedPath(m_books->pathset(), va_path)) {
    // register update
    return BookToChange(CP::Book::Index::Fetch(m_books->bids(),
                                               m_books->m_pathset->key(va_path)));
  }

  // load from file
  auto data = CP::File::Data::Load(va_path);
  if (data.count() == 2 && data.at(0) == Nmemo::ERR::PREFIX) {
    emit asStatusBarData(Sig::STATUS_MESSAGE, QVariant(data.at(1)),
                         QVariant(m_pages->msg_show_time()));
    return Sig::NOP;
  } else if (data.isEmpty()) {
    emit asStatusBarData(Sig::STATUS_MESSAGE, QVariant("File data nothing!"),
                         QVariant(m_pages->msg_show_time()));
    return Sig::NOP;
  }
  auto decoded = CP::File::Data::Decode(Nmemo::VALUE::SAVE_PREFIX, Nmemo::VALUE::SAVE_VERSION,
        &data);
  T_pagenames names = decoded.first;
  T_notes notes = decoded.second;

  // Add book
  BookToAdd(va_path);
  auto bid = m_books->currentBid();
  for (int i = 0, size = names.count(); i < size; ++i) {
    PageToAdd(bid, names.at(i), notes.at(i));
  }
  PageToChange(bid, 0);
  FileToUpdate(bid, true);

  return Utl::Sig::Combine(Sig::TITLE,
                           Utl::Sig::Combine(Sig::TAB_ALL,
                           Utl::Sig::Combine(Sig::LIST_ALL, Sig::EDITOR_ALL)));
}

auto System::FileToSave(const T_bid bid) -> T_sig
{
  auto path = m_books->m_pathset->value(bid, Nmemo::VALUE::DEFAULT_FILENAME);
  if (isInvalidPath(path)) {
    emit asFileNameRequest();
    return Sig::NOP;
  }
  return FileToSaveAs(bid, path);
}

auto System::FileToSaveAs(const T_bid bid, const T_path& path) -> T_sig
{
  if (isInvalidPath(path)) return Sig::NOP;

  auto va_path = CP::File::Path::Valid(path, Nmemo::VALUE::FILE_EXT);

  // save data
  auto pids = CP::Page::Ids::Fetch(m_pages->pidsset(), bid);
  auto names = CP::Page::Names::Filter(m_pages->nameset(), &pids);
  auto notes = CP::Note::Texts::Filter(m_pages->noteset(), &pids);
  auto encoded = CP::File::Data::Encode(Nmemo::VALUE::SAVE_PREFIX, Nmemo::VALUE::SAVE_VERSION,
                                        &names, &notes);
  // save to file
  if (!CP::File::Data::Save(va_path, &encoded)) {
    emit asStatusBarData(Sig::STATUS_MESSAGE, QVariant("Cannot open file!"),
                         QVariant(m_pages->msg_show_time()));
    return Sig::NOP;
  }

  // memory
  auto edited = CP::Book::Names::Edit(m_books->pathset(), bid, va_path);
  // register
  auto saved_edited = CP::File::States::Edit(m_books->savedset(), bid, true);
  // merge
  CP::Book::Names::Merge(m_books->pathset(), edited);
  CP::File::States::Merge(m_books->savedset(), saved_edited);

  return Utl::Sig::Combine(Sig::TITLE, Sig::TAB_ALL);
}

auto System::FileToUpdate(const T_bid bid, const T_stat stat) -> T_sig
{
  // register
  auto saved_edited = CP::File::States::Edit(m_books->savedset(), bid, stat);
  // merge
  CP::File::States::Merge(m_books->savedset(), saved_edited);

  return Sig::TAB_STATE;
}

/* methods: Book */
auto System::BookToAdd(const T_name& path) -> T_sig
{
  auto bid = Utl::ID::Allocate(u_idpool.data(), &u_nextid);

  // memory
  auto added = CP::Book::Ids::Add(m_books->bids(), bid);
  auto name_added = CP::Book::Names::Add(m_books->pathset(), bid, path);
  // register
  auto cpids_added = CP::Page::CurrentIds::Add(m_pages->pidset(), bid, -1);
  auto saved_added = CP::File::States::Add(m_books->savedset(), bid, false);
  // merge
  CP::Book::Ids::Merge(m_books->bids(), added);
  CP::Book::Names::Merge(m_books->pathset(), name_added);
  CP::Page::CurrentIds::Merge(m_pages->pidset(), cpids_added);
  m_books->ToMergeCurrentBid(bid);
  CP::File::States::Merge(m_books->savedset(), saved_added);

  return Utl::Sig::Combine(Sig::TITLE,
                           Utl::Sig::Combine(Sig::TAB_ALL,
                           Utl::Sig::Combine(Sig::LIST_ALL, Sig::EDITOR_ALL)));
}

auto System::BookToDelete(const T_index index) -> T_sig
{
  auto bid = CP::Book::Id::Fetch(m_books->bids(), index);
  if (bid <= 0) return Sig::NOP;

  // memory
  auto removed = CP::Book::Ids::Delete(m_books->bids(), bid);
  auto name_removed = CP::Book::Names::Delete(m_books->pathset(), bid);
  auto pids = CP::Page::Ids::Fetch(m_pages->pidsset(), bid);
  // register
  auto cur_bid = CP::Book::Id::Fetch(m_books->bids(), index - 1);
  auto cpids_removed = CP::Page::CurrentIds::Delete(m_pages->pidset(), bid);
  auto saved_removed = CP::File::States::Delete(m_books->savedset(), bid);
  // merge
  CP::Book::Ids::Merge(m_books->bids(), removed);
  CP::Book::Names::Merge(m_books->pathset(), name_removed);
  m_books->ToMergeCurrentBid(cur_bid);
  CP::Page::CurrentIds::Merge(m_pages->pidset(), cpids_removed);
  CP::Page::Ids::DeleteAll(m_pages->pidsset(), bid);
  CP::File::States::Merge(m_books->savedset(), saved_removed);
  // release
  for (int i = 0, size = pids.count(); i < size; ++i) {
    Utl::ID::Release(u_idpool.data(), pids.at(i));
  }
  Utl::ID::Release(u_idpool.data(), bid);

  return Utl::Sig::Combine(Sig::TITLE,
                           Utl::Sig::Combine(Sig::TAB_ALL,
                           Utl::Sig::Combine(Sig::LIST_ALL, Sig::EDITOR_ALL)));
}

auto System::BookToChange(const T_index index) -> T_sig
{
  auto bid = CP::Book::Id::Fetch(m_books->bids(), index);
  if (bid <= 0) return Sig::NOP;

  // register
  m_books->ToMergeCurrentBid(bid);

  return Utl::Sig::Combine(Sig::TITLE,
                           Utl::Sig::Combine(Sig::TAB_INDEX,
                           Utl::Sig::Combine(Sig::LIST_ALL, Sig::EDITOR_ALL)));
}

auto System::BookToMove(const T_index from, const T_index to) -> T_sig
{
  auto va_from = Utl::List::Index::Valid<T_bid>(m_books->bids(), from);
  auto va_to = Utl::List::Index::Valid<T_bid>(m_books->bids(), to);
  if (va_from < 0 || va_to < 0) return Sig::NOP;

  // memory
  auto moved = CP::Book::Ids::Move(m_books->bids(), va_from, va_to);
  // merge
  CP::Book::Ids::Merge(m_books->bids(), moved);

  return Sig::TAB_ALL;
}

auto System::BookToRename(const T_index index, const T_name& path) -> T_sig
{
  auto bid = CP::Book::Id::Fetch(m_books->bids(), index);

  // memory
  auto renamed = CP::Book::Names::Edit(m_books->pathset(), bid, path);
  CP::Book::Names::Merge(m_books->pathset(), renamed);

  return Sig::NOP;
}

/* methods: Page */
auto System::PageToAdd(const T_bid bid, const T_name& name, const T_note& note) -> T_sig
{
  auto pid = Utl::ID::Allocate(u_idpool.data(), &u_nextid);

  // memory
  auto added = CP::Page::Ids::Add(m_pages->pidsset(), bid, pid);
  auto name_added = CP::Page::Names::Add(m_pages->nameset(), pid, name);
  auto txt_added = CP::Note::Texts::Add(m_pages->noteset(), pid, note);
  auto mode_added = CP::Note::Mode::Add(m_pages->modeset(), pid, EditMode::PLAIN);
  // register
  auto cpids_added = CP::Page::CurrentIds::Add(m_pages->pidset(), bid, pid);
  auto pos_added = CP::Note::Pos::Add(m_pages->posset(), pid, 0);
  auto saved_edited = CP::File::States::Edit(m_books->savedset(), bid, false);
  // merge
  CP::Page::Ids::Merge(m_pages->pidsset(), bid, added);
  CP::Page::Names::Merge(m_pages->nameset(), name_added);
  CP::Page::CurrentIds::Merge(m_pages->pidset(), cpids_added);
  CP::Note::Texts::Merge(m_pages->noteset(), txt_added);
  CP::Note::Mode::Merge(m_pages->modeset(), mode_added);
  CP::Note::Pos::Merge(m_pages->posset(), pos_added);
  CP::File::States::Merge(m_books->savedset(), saved_edited);

  return Utl::Sig::Combine(Sig::TAB_STATE,
                           Utl::Sig::Combine(Sig::LIST_ALL, Sig::EDITOR_ALL));
}

auto System::PageToDelete(const T_bid bid, const T_index index) -> T_sig
{
  auto pid = CP::Page::Id::Fetch(m_pages->pidsset(), bid, index);
  if (pid <= 0) return Sig::NOP;

  // memory
  auto removed = CP::Page::Ids::Delete(m_pages->pidsset(), bid, pid);
  auto name_removed = CP::Page::Names::Delete(m_pages->nameset(), pid);
  auto txt_removed = CP::Note::Texts::Delete(m_pages->noteset(), pid);
  auto mode_removed = CP::Note::Mode::Delete(m_pages->modeset(), pid);
  // register
  auto cur_pid = CP::Page::Id::Fetch(m_pages->pidsset(), bid, index - 1);
  auto cpids_edited = CP::Page::CurrentIds::Edit(m_pages->pidset(), bid, cur_pid);
  auto pos_removed = CP::Note::Pos::Delete(m_pages->posset(), pid);
  auto saved_edited = CP::File::States::Edit(m_books->savedset(), bid, false);
  // merge
  CP::Page::Ids::Merge(m_pages->pidsset(), bid, removed);
  CP::Page::Names::Merge(m_pages->nameset(), name_removed);
  CP::Page::CurrentIds::Merge(m_pages->pidset(), cpids_edited);
  CP::Note::Texts::Merge(m_pages->noteset(), txt_removed);
  CP::Note::Mode::Merge(m_pages->modeset(), mode_removed);
  CP::Note::Pos::Merge(m_pages->posset(), pos_removed);
  CP::File::States::Merge(m_books->savedset(), saved_edited);
  // id release
  Utl::ID::Release(u_idpool.data(), pid);

  return Utl::Sig::Combine(Sig::TAB_STATE,
                           Utl::Sig::Combine(Sig::LIST_ALL, Sig::EDITOR_ALL));
}

auto System::PageToChange(const T_bid bid, const T_index index) -> T_sig
{
  auto pid = CP::Page::Id::Fetch(m_pages->pidsset(), bid, index);
  if (pid <= 0) return Sig::NOP;

  // memory
  // register
  auto cpids_edited = CP::Page::CurrentIds::Edit(m_pages->pidset(), bid, pid);
  // merge
  CP::Page::CurrentIds::Merge(m_pages->pidset(), cpids_edited);

  return Utl::Sig::Combine(Sig::LIST_INDEX, Sig::EDITOR_ALL);
}

auto System::PageToMove(const T_bid bid, const T_index from,
                        const T_index to) -> T_sig
{
  auto va_from = Utl::Hash::List::Index::Valid<T_bid, T_pid>(m_pages->pidsset(), bid, from);
  auto va_to = Utl::Hash::List::Index::Valid<T_bid, T_pid>(m_pages->pidsset(), bid, to);
  if (va_from < 0 || va_to < 0) return Sig::NOP;

  // memory
  auto moved = CP::Page::Ids::Move(m_pages->pidsset(), bid, va_from, va_to);
  // register
  auto saved_edited = CP::File::States::Edit(m_books->savedset(), bid, false);
  // merge
  CP::Page::Ids::Merge(m_pages->pidsset(), bid, moved);
  CP::File::States::Merge(m_books->savedset(), saved_edited);

  return Utl::Sig::Combine(Sig::TAB_STATE, Sig::LIST_ALL);
}

auto System::PageToRename(const T_bid bid, const T_index index,
                          const T_name& name) -> T_sig
{
  auto pid = CP::Page::Id::Fetch(m_pages->pidsset(), bid, index);
  if (pid <= 0) return Sig::NOP;

  // memory
  auto renamed = CP::Page::Names::Edit(m_pages->nameset(), pid, name);
  // register
  auto saved_edited = CP::File::States::Edit(m_books->savedset(), bid, false);
  // merge
  CP::Page::Names::Merge(m_pages->nameset(), renamed);
  CP::File::States::Merge(m_books->savedset(), saved_edited);

  return Utl::Sig::Combine(Sig::TAB_STATE, Sig::LIST_ALL);
}

auto System::PageToSort(const T_bid bid, const T_order order) -> T_sig
{
  // memory
  auto pids = CP::Page::Ids::Fetch(m_pages->pidsset(), bid);
  auto sorted = CP::Page::Ids::Sort(&pids, m_pages->nameset(), order);
  // register
  auto saved_edited = CP::File::States::Edit(m_books->savedset(), bid, false);
  // merge
  CP::Page::Ids::Merge(m_pages->pidsset(), bid, sorted);
  CP::File::States::Merge(m_books->savedset(), saved_edited);

  return Utl::Sig::Combine(Sig::TAB_STATE, Sig::LIST_ALL);
}

/* methods: Note */
auto System::NoteToAdd(const T_pid pid, const T_note& note) -> T_sig
{
  // memory
  auto added = CP::Note::Texts::Add(m_pages->noteset(), pid, note);
  auto mode_added = CP::Note::Mode::Add(m_pages->modeset(), pid, EditMode::PLAIN);
  // register
  auto pos_added = CP::Note::Pos::Add(m_pages->posset(), pid, 0);
  // merge
  CP::Note::Texts::Merge(m_pages->noteset(), added);
  CP::Note::Mode::Merge(m_pages->modeset(), mode_added);
  CP::Note::Pos::Merge(m_pages->posset(), pos_added);

  return Sig::EDITOR_ALL;
}

auto System::NoteToDelete(const T_pid pid) -> T_sig
{
  // memory
  auto removed = CP::Note::Texts::Delete(m_pages->noteset(), pid);
  auto mode_removed = CP::Note::Mode::Delete(m_pages->modeset(), pid);
  // register
  auto pos_removed = CP::Note::Pos::Delete(m_pages->posset(), pid);
  // merge
  CP::Note::Texts::Merge(m_pages->noteset(), removed);
  CP::Note::Mode::Merge(m_pages->modeset(), mode_removed);
  CP::Note::Pos::Merge(m_pages->posset(), pos_removed);

  return Sig::EDITOR_ALL;
}

auto System::NoteToCache(const T_note& note, const T_pos pos) -> T_sig
{
  // merge
  r_note = note;
  r_pos = pos;

  return Sig::NOP;
}

auto System::NoteToChangeMode(const T_bid bid, const T_mode mode) -> T_sig
{
  auto pid = CP::Page::CurrentId::Fetch(m_pages->pidset(), bid);
  if (pid <= 0 || !r_enabled) return Sig::NOP;
  // memory
  auto changed = CP::Note::Mode::Edit(m_pages->modeset(), pid, mode);
  // register
  auto saved_edited = CP::File::States::Edit(m_books->savedset(), bid, false);
  // merge
  CP::Note::Mode::Merge(m_pages->modeset(), changed);
  CP::File::States::Merge(m_books->savedset(), saved_edited);

  return Sig::EDITOR_ALL;
}

auto System::NoteToModify(const T_bid bid) -> T_sig
{
  // register
  auto edited = CP::File::States::Edit(m_books->savedset(), bid, false);
  // merge
  CP::File::States::Merge(m_books->savedset(), edited);

  return Sig::TAB_STATE;
}

auto System::NoteToUpdate(const T_pid pid) -> T_sig
{
  if (pid <= 0 || !r_enabled) return Sig::NOP;

  // memory
  auto edited = CP::Note::Texts::Edit(m_pages->noteset(), pid, r_note);
  // register
  auto pos_edited = CP::Note::Pos::Edit(m_pages->posset(), pid, r_pos);
  // merge
  CP::Note::Texts::Merge(m_pages->noteset(), edited);
  CP::Note::Pos::Merge(m_pages->posset(), pos_edited);

  return Sig::EDITOR_TEXT;
}

/* utils */

/* slots */
void System::ToSystemData(T_code code, T_arg arg0, T_arg arg1, T_arg arg2)
{
  auto cur_bid = m_books->currentBid();
  if (cur_bid > 0) {
    NoteToUpdate(CP::Page::CurrentId::Fetch(m_pages->pidset(), cur_bid));
  }

  auto sig = BranchToData(code, arg0, arg1, arg2);
  ToOutputs(sig, code);
}


/* process: Core Process */
namespace CP {

/* CP: File */
namespace File {

namespace Path {

auto FetchBase(const T_pathset* pathset, const T_bid bid) -> T_name
{
  auto path = pathset->value(bid);
  if (path.isEmpty() || path == Nmemo::VALUE::DEFAULT_FILENAME || path == "") {
    return Nmemo::VALUE::DEFAULT_FILENAME;
  }
  return Utl::Name::FilterDirname(path);
}

auto Valid(const T_path& path, const T_ext& ext) -> T_path
{
  return QFileInfo(path).suffix() == ext ?
        path: QString("%1.%2").arg(path).arg(ext);
}

}  // ns CP::File::Path

namespace Data {

auto Encode(const T_text& save_pref, const T_text& save_ver,
            const T_pagenames* names, const T_notes* notes) -> T_encoded
{
  T_encoded tmp;
  tmp << save_pref << save_ver;
  for (int i = 0, size = names->count(); i < size; ++i) {
    tmp << names->at(i) << notes->at(i);
  }
  return tmp;
}

auto Decode(const T_text& save_pref, const T_text& save_ver, const T_strs* strs) -> T_decoded
{
  T_pagenames names;
  T_notes notes;
  bool is_oldtype = false;
  // version check
  if (strs->at(0) == save_pref) {
    auto version = strs->at(1).toInt();
    is_oldtype = version < save_ver.toInt();
  } else {
    is_oldtype = true;
  }
  // data decode
  QStringList::const_iterator it;
  for (it = strs->constBegin(); it != strs->constEnd(); ++it) {
    if (!is_oldtype) {
      ++it;
      ++it;
      is_oldtype = true;
    }
    names << *it;
    ++it;
    notes << *it;
  }

  return qMakePair(names, notes);
}

auto Load(const T_path& path) -> T_strs
{
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly)) {
    return T_strs{Nmemo::ERR::PREFIX, "Cannot open file!"};
  }

  QDataStream in(&file);
  T_strs data_loaded;
  in.setVersion(QDataStream::Qt_5_10);
  in >> data_loaded;

  return data_loaded;
}

auto Save(const T_path& path, const T_encoded* encoded) -> bool
{
  QFile file(path);
  if (!file.open(QIODevice::WriteOnly)) {
    return false;
  }

  QDataStream out(&file);
  out.setVersion(QDataStream::Qt_5_10);
  out << (*encoded);

  return true;
}

}  // ns CP::File::Data

namespace States {

auto Convert(const T_stats* stats) -> QList<QVariant>
{
  QList<QVariant> tmp;
  for (int i = 0, size = stats->count(); i < size; ++i) {
    tmp << QVariant(stats->at(i));
  }
  return tmp;
}

auto Filter(const T_statset* statset, const T_bids* bids) -> T_stats
{
  T_stats tmp;
  for (int i = 0, size = bids->count(); i < size; ++i) {
    tmp << statset->value(bids->at(i));
  }
  return tmp;
}

auto Add(const T_statset* statset, const T_bid bid, const T_stat stat) -> T_statset
{
  return Utl::Hash::Add<T_bid, T_stat>(statset, bid, stat);
}

auto Delete(const T_statset* statset, const T_bid bid) -> T_statset
{
  return Utl::Hash::Delete<T_bid, T_stat>(statset, bid);
}

auto Edit(const T_statset* statset, const T_bid bid, const T_stat stat) -> T_statset
{
  return Utl::Hash::Edit<T_bid, T_stat>(statset, bid, stat);
}

auto Merge(T_statset* base, T_statset& updated) -> bool
{
  return Utl::Hash::Merge<T_bid, T_stat>(base, updated);
}

}  // ns CP::File::States

}  // ns CP::File

/* CP: Book */
namespace Book {

namespace Id {

auto Fetch(const T_ids* ids, const T_index index) -> T_bid
{
  return index >= 0 && ids->count() > 0 && index < ids->count() ? ids->at(index): -1;
}

}  // ns CP::Book:Id

namespace Index {

auto Fetch(const T_ids* ids, const T_id id) -> T_index
{
  return ids->indexOf(id);
}

}  // ns CP::BOOK::Index

namespace Ids {

auto Add(const T_bids* bids, const T_bid bid) -> T_bids
{
  return Utl::List::Add<T_bid>(bids, bid);
}

auto Delete(const T_bids* bids, const T_bid bid) -> T_bids
{
  return Utl::List::Delete<T_bid>(bids, bid);
}

auto Move(const T_bids* bids, const T_index from, const T_index to) -> T_bids
{
  return Utl::List::Move<T_bid>(bids, from, to);
}

auto Merge(T_bids* base, T_bids& updated) -> bool
{
  Utl::List::Merge<T_bid>(base, updated);
  return true;
}

}  // ns CP::Book::Ids

namespace Names {

auto Filter(const T_pathset* pathset, const T_bids* bids) -> T_tabnames
{
  T_tabnames names;
  for (int i = 0, size = bids->count(); i < size; ++i) {
    names << Utl::Name::Filter(pathset->value(bids->at(i)));
  }
  return names;
}

auto Add(const T_pathset* pathset, const T_bid bid, const T_path& path) -> T_pathset
{
  return Utl::Hash::Add<T_bid, T_path>(pathset, bid, path);
}

auto Delete(const T_pathset* pathset, const T_bid bid) -> T_pathset
{
  return Utl::Hash::Delete<T_bid, T_path>(pathset, bid);
}

auto Edit(const T_pathset* pathset, const T_bid bid, const T_path& path) -> T_pathset
{
  return Utl::Hash::Edit<T_bid, T_path>(pathset, bid, path);
}

auto Merge(T_pathset* base, T_pathset& updated) -> bool
{
  Utl::Hash::Merge<T_bid, T_path>(base, updated);
  return true;
}

}  // ns CP::Book::Names

}  // ns CP::Book

/* CP: Page */
namespace Page {

namespace Id {

auto Fetch(const T_pidsset* pidsset, const T_bid bid, const T_index index) -> T_pid
{
  return Utl::Hash::List::Value::Fetch<T_bid, T_pid>(pidsset, bid, index, -1);
}

}  // ns CP::Page::Id

namespace Index {

auto Fetch(const T_pidsset* pidsset, const T_bid bid, const T_pid pid) -> T_index
{
  return Utl::Hash::List::Index::Fetch<T_bid, T_pid>(pidsset, bid, pid);
}

}  // ns CP::Page::Index

namespace Ids {

auto Fetch(const T_pidsset* pidsset, const T_bid bid) -> T_pids
{
  return pidsset->value(bid);
}

auto Add(const T_pidsset* pidsset, const T_bid bid, const T_pid pid) -> T_pids
{
  return Utl::Hash::List::Add<T_bid, T_pid>(pidsset, bid, pid);
}

auto Delete(const T_pidsset* pidsset, const T_bid bid, const T_pid pid) -> T_pids
{
  return Utl::Hash::List::Delete<T_bid, T_pid>(pidsset, bid, pid);
}

auto Move(const T_pidsset* pidsset, const T_bid bid, const T_index from,
          const T_index to) -> T_pids
{
  return Utl::Hash::List::Move<T_bid, T_pid>(pidsset, bid, from, to);
}

auto Sort(const T_pids* pids, const T_nameset* nameset, const T_order order) -> T_pids
{
  auto names = CP::Page::Names::Filter(nameset, pids);
  T_strs combined;
  for (int i = 0, size = pids->count(); i < size; ++i) {
    combined << names.at(i) + QString("///") + QString::number(pids->at(i));
  }
  if (order == Qt::AscendingOrder) {
    std::sort(combined.begin(), combined.end());
  } else {// Qt::DescendingOrder
    std::sort(combined.begin(), combined.end(), std::greater<QString>());
  }
  T_ids tmp;
  for (int i = 0, size = combined.count(); i < size; ++i) {
    tmp << combined.at(i).section("///", 1, 1).toInt();
  }
  return tmp;
}

auto Merge(T_pidsset* pidsset, const T_bid bid, T_pids& pids) -> bool
{
  Utl::Hash::List::Merge<T_bid, T_pid>(pidsset, bid, pids);
  return true;
}

auto DeleteAll(T_pidsset* pidsset, const T_bid bid) -> bool
{
  auto tmp = Utl::Hash::List::DeleteAll<T_bid, T_pid>(pidsset, bid);
  return Utl::Hash::List::MergeAll<T_bid, T_pid>(pidsset, tmp);
}

}  // ns CP::Page::Ids

namespace Names {

auto Filter(const T_nameset* nameset, const T_pids* pids) -> T_pagenames
{
  T_pagenames tmp;
  for (int i = 0, size = pids->count(); i < size; ++i) {
    tmp << nameset->value(pids->at(i));
  }
  return tmp;
}

auto Add(const T_nameset* nameset, const T_pid pid, const T_name& name) -> T_nameset
{
  return Utl::Hash::Add<T_pid, T_name>(nameset, pid, name);
}

auto Delete(const T_nameset* nameset, const T_pid pid) -> T_nameset
{
  return Utl::Hash::Delete<T_pid, T_name>(nameset, pid);
}

auto Edit(const T_nameset* nameset, const T_pid pid, const T_name& name) -> T_nameset
{
  return Utl::Hash::Edit<T_pid, T_name>(nameset, pid, name);
}

auto Merge(T_nameset* base, T_nameset& updated) -> bool
{
  Utl::Hash::Merge<T_pid, T_name>(base, updated);
  return true;
}

}  // ns CP::Page::Names

namespace CurrentId {

auto Fetch(const T_pidset* pidset, const T_bid bid) -> T_pid
{
  return pidset->value(bid, -1);
}

}  // ns CP::Page::CurrentId

namespace CurrentIds {

auto Filter(const T_pidset* pidset, const T_bids* bids) -> T_pids
{
  T_pids tmp;
  for (int i = 0, size = bids->count(); i < size; ++i) {
    tmp << pidset->value(bids->at(i));
  }
  return tmp;
}

auto Add(const T_pidset* pidset, const T_bid bid, const T_pid pid) -> T_pidset
{
  return Utl::Hash::Add<T_bid, T_pid>(pidset, bid, pid);
}

auto Delete(const T_pidset* pidset, const T_bid bid) -> T_pidset
{
  return Utl::Hash::Delete<T_bid, T_pid>(pidset, bid);
}

auto Edit(const T_pidset* pidset, const T_bid bid, const T_pid pid) -> T_pidset
{
  return Utl::Hash::Edit<T_bid, T_pid>(pidset, bid, pid);
}

auto Merge(T_pidset* base, T_pidset& updated) -> bool
{
  Utl::Hash::Merge<T_bid, T_pid>(base, updated);
  return true;
}

}  // ns CP::Page::CurrentIds

}  // ns CP::Page

/* CP: Note */
namespace Note {

namespace Text {

auto Fetch(const T_noteset* noteset, const T_pid pid) -> T_note
{
  return noteset->contains(pid) ? noteset->value(pid):
                                  Nmemo::VALUE::CAUTION_EDITOR_MESSAGE;
}

}  // ns CP::Note::Text

namespace Texts {

auto Filter(const T_noteset* noteset, const T_pids* pids) -> T_notes
{
  T_notes tmp;
  for (int i = 0, size = pids->count(); i < size; ++i) {
    tmp << noteset->value(pids->at(i));
  }
  return tmp;
}

auto Add(const T_noteset* noteset, const T_pid pid, const T_note& note) -> T_noteset
{
  return Utl::Hash::Add<T_pid, T_note>(noteset, pid, note);
}

auto Delete(const T_noteset* noteset, const T_pid pid) -> T_noteset
{
  return Utl::Hash::Delete<T_pid, T_note>(noteset, pid);
}

auto Edit(const T_noteset* noteset, const T_pid pid, const T_note& note) -> T_noteset
{
  return Utl::Hash::Edit<T_pid, T_note>(noteset, pid, note);
}

auto Merge(T_noteset* base, T_noteset& updated) -> bool
{
  return Utl::Hash::Merge<T_pid, T_note>(base, updated);
}

}  // ns CP::Note::Texts

namespace Pos {

auto Fetch(const T_posset* posset, const T_pid pid) -> T_pos
{
  return Utl::Hash::Fetch<T_pid, T_pos>(posset, pid, 0);
}

auto Add(const T_posset* posset, const T_pid pid, const T_pos pos) -> T_posset
{
  return Utl::Hash::Add<T_pid, T_pos>(posset, pid, pos);
}

auto Delete(const T_posset* posset, const T_pid pid) -> T_posset
{
  return Utl::Hash::Delete<T_pid, T_pos>(posset, pid);
}

auto Edit(const T_posset* posset, const T_pid pid, const T_pos pos) -> T_posset
{
  return Utl::Hash::Edit<T_pid, T_pos>(posset, pid, pos);
}

auto Merge(T_posset* base, T_posset& updated) -> bool
{
  return Utl::Hash::Merge<T_pid, T_pos>(base, updated);
}

}  // ns CP::Note::Pos

namespace Mode {

auto Fetch(const T_modeset* modeset, const T_pid pid) -> T_mode
{
  return Utl::Hash::Fetch<T_pid, T_mode>(modeset, pid, EditMode::PLAIN);
}

auto Add(const T_modeset* modeset, const T_pid pid, const T_mode mode) -> T_modeset
{
  return Utl::Hash::Add<T_pid, T_mode>(modeset, pid, mode);
}

auto Delete(const T_modeset* modeset, const T_pid pid) -> T_modeset
{
  return Utl::Hash::Delete<T_pid, T_mode>(modeset, pid);
}

auto Edit(const T_modeset* modeset, const T_pid pid, const T_mode mode) -> T_modeset
{
  return Utl::Hash::Edit<T_pid, T_mode>(modeset, pid, mode);
}

auto Merge(T_modeset* base, T_modeset& updated) -> bool
{
  return Utl::Hash::Merge<T_pid, T_mode>(base, updated);
}

}  // ns CP::Note::Mode

}  // ns CP::Note
}  // ns CP

}  // namespace Nmemo
