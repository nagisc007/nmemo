/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "core.h"

namespace Core {

/* class: System */
System::System(QObject* parent): QObject(parent),
  // memory
  m_bids(new T_ids()),
  m_pathset(new T_strset()),
  m_pidsset(new T_pidsset()),
  m_nameset(new T_strset()),
  m_noteset(new T_strset()),
  // register
  r_bid(-1),
  r_note(""),
  r_enabled(false),
  r_pidset(new T_pidset()),
  r_savedset(new T_statset()),
  // utils
  u_nextid(0),
  u_idpool(new QStack<T_id>())
{
  qDebug() << "System: constructed";
}

System::~System()
{
  if (m_bids) {
    m_bids->clear();
    m_bids.reset();
  }
  if (m_pathset) {
    m_pathset->clear();
    m_pathset.reset();
  }
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
  if (r_pidset) {
    r_pidset->clear();
    r_pidset.reset();
  }
  if (r_savedset) {
    r_savedset->clear();
    r_savedset.reset();
  }
  if (u_idpool) {
    u_idpool->clear();
    u_idpool.reset();
  }
  qDebug() << "System: destructed";
}

/* methods: output */
auto System::OutToTabBar(const T_cmd cmd) -> void
{
  auto stats = CP::File::States::Filter(r_savedset.data(), m_bids.data());
  emit asTabBarData(cmd,
                    QVariant(CP::Book::Index::Fetch(m_bids.data(),
                                                    CP::Book::CurrentId::Fetch(this))),
                    QVariant(CP::Book::Names::Filter(m_pathset.data(), m_bids.data())),
                    QVariant(CP::File::States::Convert(&stats)));
}

auto System::OutToPageList(const T_cmd cmd) -> void
{
  auto bid = CP::Book::CurrentId::Fetch(this);
  auto pids = CP::Page::Ids::Fetch(m_pidsset.data(), bid);
  emit asPageListData(cmd,
                      QVariant(CP::Page::Index::Fetch(
                                 m_pidsset.data(),
                                 bid,
                                 CP::Page::CurrentId::Fetch(r_pidset.data(), bid))),
                      QVariant(CP::Page::Names::Filter(
                                 m_nameset.data(),
                                 &pids)));
}

auto System::OutToEditor(const T_cmd cmd) -> void
{
  auto current_pid = CP::Page::CurrentId::Fetch(
        r_pidset.data(),
        CP::Book::CurrentId::Fetch(this));
  r_enabled = current_pid > 0;
  emit asEditorData(cmd,
                    QVariant(CP::Note::Text::Fetch(
                               m_noteset.data(), current_pid)),
                    QVariant(r_enabled));
}

auto System::OutToStatusBar(const T_cmd cmd) -> void
{
  T_text msg;
  switch (cmd) {
  case Cmd::FILE_LOAD:
    msg = "File loaded ...";
    break;
  case Cmd::FILE_SAVE:
  case Cmd::FILE_SAVEAS:
    msg = "File saved ...";
    break;
  case Cmd::BOOK_ADD:
    msg = "Book added ...";
    break;
  case Cmd::BOOK_DELETE:
    msg = "Book removed ...";
    break;
  case Cmd::BOOK_MOVE:
    msg = "Book moved ...";
    break;
  case Cmd::BOOK_EDIT:
    msg = "Book renamed ...";
    break;
  case Cmd::PAGE_ADD:
    msg = "Page added ...";
    break;
  case Cmd::PAGE_DELETE:
    msg = "Page removed ...";
    break;
  case Cmd::PAGE_MOVE:
    msg = "Page moved ...";
    break;
  case Cmd::PAGE_EDIT:
    msg = "Page renamed ...";
    break;
  case Cmd::BOOK_CHANGE:
  case Cmd::PAGE_CHANGE:
  default:
    return;
  }
  emit asStatusBarData(Cmd::STATUS_TEXT, QVariant(msg), QVariant(0));
}

auto System::OutToTitleBar(const T_cmd cmd) -> void
{
  emit asTitleBarData(cmd,
                      QVariant(CP::File::Path::FetchBase(m_pathset.data(),
                                                         CP::Book::CurrentId::Fetch(this))));
}

/* methods: Data */
auto System::ToFileData(const T_cmd cmd, const T_arg arg0, const T_arg arg1,
                        const T_arg arg2) -> T_cmd
{
  Q_UNUSED(arg1);
  Q_UNUSED(arg2);

  switch (cmd) {
  case Cmd::FILE_LOAD:
    return FileToLoad(arg0.toString());
  case Cmd::FILE_SAVE:{
    auto bid = CP::Book::CurrentId::Fetch(this);
    if (bid <= 0) return Cmd::NOP;

    auto path = m_pathset->value(bid, Nmemo::VALUE::DEFAULT_FILENAME);
    if (path.isEmpty() || path == Nmemo::VALUE::DEFAULT_FILENAME) {
      emit asFileNameRequest();
      return Cmd::NOP;
    }
    return FileToSave(path);}
  case Cmd::FILE_SAVEAS:
    return FileToSave(arg0.toString());
  default:
    return Cmd::NOP;
  }
}

auto System::ToBookData(const T_cmd cmd, const T_arg arg0, const T_arg arg1,
                        const T_arg arg2) -> T_cmd
{
  Q_UNUSED(arg2);
  switch (cmd) {
  case Cmd::BOOK_ADD:
    return BookToAdd(arg0.toString());
  case Cmd::BOOK_DELETE:
    return BookToDelete(arg0.toInt());
  case Cmd::BOOK_CHANGE:
    return BookToChange(arg0.toInt());
  case Cmd::BOOK_MOVE:
    return BookToMove(arg0.toInt(), arg1.toInt());
  case Cmd::BOOK_EDIT:
    return BookToRename(arg0.toInt(), arg1.toString());
  default:
    return Cmd::NOP;
  }
}

auto System::ToPageData(const T_cmd cmd, const T_arg arg0, const T_arg arg1,
                        const T_arg arg2) -> T_cmd
{
  auto bid = CP::Book::CurrentId::Fetch(this);
  if (bid <= 0) return Cmd::NOP;
  Q_UNUSED(arg2);
  switch (cmd) {
  case Cmd::PAGE_ADD:
    return PageToAdd(bid, arg0.toString(), Nmemo::VALUE::DEFAULT_EDITOR_TEXT);
  case Cmd::PAGE_DELETE:
    return PageToDelete(bid, arg0.toInt());
  case Cmd::PAGE_CHANGE:
    return PageToChange(bid, arg0.toInt());
  case Cmd::PAGE_MOVE:
    return PageToMove(bid, arg0.toInt(), arg1.toInt());
  case Cmd::PAGE_EDIT:
    return PageToRename(bid, arg0.toInt(), arg1.toString());
  case Cmd::PAGE_SORT:
    return PageToSort(bid, static_cast<Qt::SortOrder>(arg0.toInt()));
  default:
    return Cmd::NOP;
  }
}

auto System::ToNoteData(const T_cmd cmd, const T_arg arg0, const T_arg arg1,
                        const T_arg arg2) -> T_cmd
{
  Q_UNUSED(arg1);
  Q_UNUSED(arg2);
  switch (cmd) {
  case Cmd::NOTE_EDIT:
    return NoteToCache(arg0.toString());
  case Cmd::NOTE_MODIFY:
    return NoteToModify();
  default:
    return Cmd::NOP;
  }
}

/* methods: File */
auto System::FileToLoad(const T_path& path) -> T_cmd
{
  if (path.isEmpty() || path == "" ||
      path == Nmemo::VALUE::DEFAULT_FILENAME) return Cmd::NOP;

  auto bid = CP::Book::CurrentId::Fetch(this);
  if (bid > 0) {
    NoteToUpdate(CP::Page::CurrentId::Fetch(r_pidset.data(), bid));
  }
  auto va_path = CP::File::Path::Valid(path, Nmemo::VALUE::FILE_EXT);

  // check already path
  bool is_already = false;
  QMapIterator<T_bid, T_path> it(*m_pathset);
  while (it.hasNext()) {
    it.next();
    if (it.value() == va_path) {
      is_already = true;
      break;
    }
  }
  if (is_already) {
    // register update
    auto cur_bid = m_pathset->key(va_path);
    CP::Book::CurrentId::Merge(&r_bid, cur_bid);
    return Cmd::TAB_ALL;
  }

  // load from file
  QFile file(va_path);
  if (!file.open(QIODevice::ReadOnly)) {
    emit asStatusBarData(Cmd::STATUS_TEXT, QVariant("Cannot open file!"), QVariant(0));
    return Cmd::NOP;
  }
  QDataStream in(&file);
  T_strs data_loaded;
  in.setVersion(QDataStream::Qt_5_10);
  in >> data_loaded;
  if (data_loaded.isEmpty()) {
    emit asStatusBarData(Cmd::STATUS_TEXT, QVariant("File data nothing!"), QVariant(0));
    return Cmd::NOP;
  }
  auto data = CP::File::Data::Decode(Nmemo::VALUE::SAVE_PREFIX, Nmemo::VALUE::SAVE_VERSION,
        &data_loaded);
  T_pagenames names = data.first;
  T_notes notes = data.second;

  // Add book
  BookToAdd(va_path);
  auto new_bid = CP::Book::CurrentId::Fetch(this);
  for (int i = 0, size = names.count(); i < size; ++i) {
    PageToAdd(new_bid, names.at(i), notes.at(i));
  }
  PageToChange(new_bid, 0);
  FileToUpdate(new_bid, true);

  return Utl::Cmd::Combine(Cmd::TITLE,
                           Utl::Cmd::Combine(Cmd::EDITOR_ALL,
                           Utl::Cmd::Combine(Cmd::LIST_ALL, Cmd::TAB_ALL)));
}

auto System::FileToSave(const T_path& path) -> T_cmd
{
  auto bid = CP::Book::CurrentId::Fetch(this);
  if (bid < 0 ||
      path.isEmpty() || path == "" ||
      path == Nmemo::VALUE::DEFAULT_FILENAME) return Cmd::NOP;

  NoteToUpdate(CP::Page::CurrentId::Fetch(r_pidset.data(), bid));
  auto va_path = CP::File::Path::Valid(path, Nmemo::VALUE::FILE_EXT);

  // save data
  auto pids = CP::Page::Ids::Fetch(m_pidsset.data(), bid);
  auto names = CP::Page::Names::Filter(m_nameset.data(), &pids);
  auto notes = CP::Note::Texts::Filter(m_noteset.data(), &pids);
  auto encoded = CP::File::Data::Encode(Nmemo::VALUE::SAVE_PREFIX, Nmemo::VALUE::SAVE_VERSION,
                                        &names, &notes);
  // save to file
  QFile file(va_path);
  if (!file.open(QIODevice::WriteOnly)) {
    emit asStatusBarData(Cmd::STATUS_TEXT, QVariant("Cannot open file!"), QVariant(0));
    return Cmd::NOP;
  }
  QDataStream out(&file);
  out.setVersion(QDataStream::Qt_5_10);
  out << encoded;

  // memory
  auto edited = CP::Book::Names::Edit(m_pathset.data(), bid, va_path);
  CP::Book::Names::Merge(m_pathset.data(), edited);
  // register
  auto saved_edited = CP::File::States::Edit(r_savedset.data(), bid, true);
  CP::File::States::Merge(r_savedset.data(), saved_edited);

  return Utl::Cmd::Combine(Cmd::TITLE, Cmd::TAB_ALL);
}

auto System::FileToUpdate(const T_bid bid, const T_stat stat) -> T_cmd
{
  // register
  auto saved_edited = CP::File::States::Edit(r_savedset.data(), bid, stat);
  CP::File::States::Merge(r_savedset.data(), saved_edited);
  return Cmd::TAB_STATUS;
}

/* methods: Book */
auto System::BookToAdd(const T_name& path) -> T_cmd
{
  NoteToUpdate(CP::Page::CurrentId::Fetch(r_pidset.data(),
                                          CP::Book::CurrentId::Fetch(this)));
  auto bid = Utl::ID::Allocate(u_idpool.data(), &u_nextid);

  // memory
  auto added = CP::Book::Ids::Add(m_bids.data(), bid);
  auto name_added = CP::Book::Names::Add(m_pathset.data(), bid, path);
  CP::Book::Ids::Merge(m_bids.data(), added);
  CP::Book::Names::Merge(m_pathset.data(), name_added);

  // register
  auto stat_added = Utl::Map::Add<T_bid, T_pid>(r_pidset.data(), bid, -1);
  auto saved_added = CP::File::States::Add(r_savedset.data(), bid, false);
  Utl::Map::Merge<T_bid, T_pid>(r_pidset.data(), stat_added);
  CP::Book::CurrentId::Merge(&r_bid, bid);
  CP::File::States::Merge(r_savedset.data(), saved_added);

  return Utl::Cmd::Combine(Cmd::TITLE,
                           Utl::Cmd::Combine(Cmd::EDITOR_ALL,
                           Utl::Cmd::Combine(Cmd::TAB_ALL, Cmd::LIST_ALL)));
}

auto System::BookToDelete(const T_index index) -> T_cmd
{
  NoteToUpdate(CP::Page::CurrentId::Fetch(r_pidset.data(),
                                          CP::Book::CurrentId::Fetch(this)));
  auto bid = CP::Book::Id::Fetch(m_bids.data(), index);
  if (bid <= 0) return Cmd::NOP;

  // memory
  auto removed = CP::Book::Ids::Delete(m_bids.data(), bid);
  auto name_removed = CP::Book::Names::Delete(m_pathset.data(), bid);
  CP::Book::Ids::Merge(m_bids.data(), removed);
  CP::Book::Names::Merge(m_pathset.data(), name_removed);

  // register
  auto cur_bid = CP::Book::Id::Fetch(m_bids.data(), index - 1);
  auto saved_removed = CP::File::States::Delete(r_savedset.data(), bid);
  CP::Book::CurrentId::Merge(&r_bid, cur_bid);
  CP::File::States::Merge(r_savedset.data(), saved_removed);

  // release
  auto pids = CP::Page::Ids::Fetch(m_pidsset.data(), bid);
  for (int i = 0, size = pids.count(); i < size; ++i) {
    Utl::ID::Release(u_idpool.data(), pids.at(i));
  }
  Utl::ID::Release(u_idpool.data(), bid);
  CP::Page::Ids::DeleteAll(m_pidsset.data(), bid);

  return Utl::Cmd::Combine(Cmd::TITLE,
                           Utl::Cmd::Combine(Cmd::EDITOR_ALL,
                           Utl::Cmd::Combine(Cmd::TAB_ALL, Cmd::LIST_ALL)));
}

auto System::BookToChange(const T_index index) -> T_cmd
{
  NoteToUpdate(CP::Page::CurrentId::Fetch(r_pidset.data(),
                                          CP::Book::CurrentId::Fetch(this)));
  auto bid = CP::Book::Id::Fetch(m_bids.data(), index);
  if (bid <= 0) return Cmd::NOP;

  // register
  CP::Book::CurrentId::Merge(&r_bid, bid);

  return Utl::Cmd::Combine(Cmd::TITLE,
                           Utl::Cmd::Combine(Cmd::EDITOR_ALL,
                           Utl::Cmd::Combine(Cmd::TAB_INDEX, Cmd::LIST_ALL)));
}

auto System::BookToMove(const T_index from, const T_index to) -> T_cmd
{
  auto va_from = Utl::List::Index::Valid<T_bid>(m_bids.data(), from);
  auto va_to = Utl::List::Index::Valid<T_bid>(m_bids.data(), to);
  if (va_from < 0 || va_to < 0) return Cmd::NOP;

  // memory
  auto moved = CP::Book::Ids::Move(m_bids.data(), va_from, va_to);
  CP::Book::Ids::Merge(m_bids.data(), moved);

  return Cmd::TAB_ALL;
}

auto System::BookToRename(const T_index index, const T_name& path) -> T_cmd
{
  auto bid = CP::Book::Id::Fetch(m_bids.data(), index);

  // memory
  auto renamed = CP::Book::Names::Edit(m_pathset.data(), bid, path);
  CP::Book::Names::Merge(m_pathset.data(), renamed);

  return Cmd::NOP;
}

/* methods: Page */
auto System::PageToAdd(const T_bid bid, const T_name& name, const T_note& note) -> T_cmd
{
  NoteToUpdate(CP::Page::CurrentId::Fetch(r_pidset.data(),
                                          CP::Book::CurrentId::Fetch(this)));
  auto pid = Utl::ID::Allocate(u_idpool.data(), &u_nextid);

  // memory
  auto added = CP::Page::Ids::Add(m_pidsset.data(), bid, pid);
  auto name_added = CP::Page::Names::Add(m_nameset.data(), pid, name);
  auto txt_added = CP::Note::Texts::Add(m_noteset.data(), pid, note);
  CP::Page::Ids::Merge(m_pidsset.data(), bid, added);
  CP::Page::Names::Merge(m_nameset.data(), name_added);
  CP::Note::Texts::Merge(m_noteset.data(), txt_added);

  // register
  auto cur_added = CP::Page::CurrentIds::Add(r_pidset.data(), bid, pid);
  auto saved_edited = CP::File::States::Edit(r_savedset.data(), bid, false);
  CP::Page::CurrentIds::Merge(r_pidset.data(), cur_added);
  CP::File::States::Merge(r_savedset.data(), saved_edited);

  return Utl::Cmd::Combine(Cmd::EDITOR_ALL,
                           Utl::Cmd::Combine(Cmd::LIST_ALL, Cmd::TAB_STATUS));
}

auto System::PageToDelete(const T_bid bid, const T_index index) -> T_cmd
{
  NoteToUpdate(CP::Page::CurrentId::Fetch(r_pidset.data(),
                                          CP::Book::CurrentId::Fetch(this)));
  auto pid = CP::Page::Id::Fetch(m_pidsset.data(), bid, index);
  if (pid <= 0) return Cmd::NOP;

  // memory
  auto removed = CP::Page::Ids::Delete(m_pidsset.data(), bid, pid);
  auto name_removed = CP::Page::Names::Delete(m_nameset.data(), pid);
  CP::Page::Ids::Merge(m_pidsset.data(), bid, removed);
  CP::Page::Names::Merge(m_nameset.data(), name_removed);
  auto txt_removed = CP::Note::Texts::Delete(m_noteset.data(), pid);
  CP::Note::Texts::Merge(m_noteset.data(), txt_removed);
  // id release
  Utl::ID::Release(u_idpool.data(), pid);

  // register
  auto cur_pid = CP::Page::Id::Fetch(m_pidsset.data(), bid, index - 1);
  auto cur_edited = CP::Page::CurrentIds::Edit(r_pidset.data(), bid, cur_pid);
  auto saved_edited = CP::File::States::Edit(r_savedset.data(), bid, false);
  CP::Page::CurrentIds::Merge(r_pidset.data(), cur_edited);
  CP::File::States::Merge(r_savedset.data(), saved_edited);

  return Utl::Cmd::Combine(Cmd::EDITOR_ALL,
                           Utl::Cmd::Combine(Cmd::LIST_ALL, Cmd::TAB_STATUS));
}

auto System::PageToChange(const T_bid bid, const T_index index) -> T_cmd
{
  NoteToUpdate(CP::Page::CurrentId::Fetch(r_pidset.data(),
                                          CP::Book::CurrentId::Fetch(this)));
  auto pid = CP::Page::Id::Fetch(m_pidsset.data(), bid, index);
  if (pid <= 0) return Cmd::NOP;

  // memory
  // register
  auto cur_edited = CP::Page::CurrentIds::Edit(r_pidset.data(), bid, pid);
  CP::Page::CurrentIds::Merge(r_pidset.data(), cur_edited);
  return Utl::Cmd::Combine(Cmd::EDITOR_ALL, Cmd::LIST_INDEX);
}

auto System::PageToMove(const T_bid bid, const T_index from, const T_index to) -> T_cmd
{
  auto va_from = Utl::Map::List::Index::Valid<T_bid, T_pid>(m_pidsset.data(), bid, from);
  auto va_to = Utl::Map::List::Index::Valid<T_bid, T_pid>(m_pidsset.data(), bid, to);
  if (va_from < 0 || va_to < 0) return Cmd::NOP;

  // memory
  auto moved = CP::Page::Ids::Move(m_pidsset.data(), bid, va_from, va_to);
  CP::Page::Ids::Merge(m_pidsset.data(), bid, moved);

  // register
  auto saved_edited = CP::File::States::Edit(r_savedset.data(), bid, false);
  CP::File::States::Merge(r_savedset.data(), saved_edited);

  return Utl::Cmd::Combine(Cmd::LIST_ALL, Cmd::TAB_STATUS);
}

auto System::PageToRename(const T_bid bid, const T_index index, const T_name& name) -> T_cmd
{
  auto pid = CP::Page::Id::Fetch(m_pidsset.data(), bid, index);
  if (pid <= 0) return Cmd::NOP;

  // memory
  auto renamed = CP::Page::Names::Edit(m_nameset.data(), pid, name);
  CP::Page::Names::Merge(m_nameset.data(), renamed);

  // register
  auto saved_edited = CP::File::States::Edit(r_savedset.data(), bid, false);
  CP::File::States::Merge(r_savedset.data(), saved_edited);

  return Utl::Cmd::Combine(Cmd::LIST_ALL, Cmd::TAB_STATUS);
}

auto System::PageToSort(const T_bid bid, const T_order order) -> T_cmd
{
  // memory
  auto pids = CP::Page::Ids::Fetch(m_pidsset.data(), bid);
  auto names = CP::Page::Names::Filter(m_nameset.data(), &pids);
  T_strs combined;
  for (int i = 0, size = pids.count(); i < size; ++i) {
    combined << names.at(i) + QString("///") + QString::number(pids.at(i));
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
  CP::Page::Ids::Merge(m_pidsset.data(), bid, tmp);

  // register
  auto saved_edited = CP::File::States::Edit(r_savedset.data(), bid, false);
  CP::File::States::Merge(r_savedset.data(), saved_edited);

  return Utl::Cmd::Combine(Cmd::LIST_ALL, Cmd::TAB_STATUS);
}

/* methods: Note */
auto System::NoteToAdd(const T_pid pid, const T_note& note) -> T_cmd
{
  // memory
  auto added = CP::Note::Texts::Add(m_noteset.data(), pid, note);
  CP::Note::Texts::Merge(m_noteset.data(), added);

  return Cmd::EDITOR_ALL;
}

auto System::NoteToDelete(const T_pid pid) -> T_cmd
{
  // memory
  auto removed = CP::Note::Texts::Delete(m_noteset.data(), pid);
  CP::Note::Texts::Merge(m_noteset.data(), removed);

  return Cmd::EDITOR_ALL;
}

auto System::NoteToCache(const T_note& note) -> T_cmd
{
  // register
  r_note = note;

  return Cmd::NOP;
}

auto System::NoteToModify() -> T_cmd
{
  auto bid = CP::Book::CurrentId::Fetch(this);
  if (bid < 0) return Cmd::NOP;

  auto edited = CP::File::States::Edit(r_savedset.data(), bid, false);
  CP::File::States::Merge(r_savedset.data(), edited);

  return Cmd::TAB_STATUS;
}

auto System::NoteToUpdate(const T_pid pid) -> T_cmd
{
  if (pid <= 0 || !r_enabled) return Cmd::NOP;

  // memory
  auto edited = CP::Note::Texts::Edit(m_noteset.data(), pid, r_note);
  CP::Note::Texts::Merge(m_noteset.data(), edited);

  return Cmd::EDITOR_TEXT;
}

/* slots */
void System::ToSystemData(T_cmd cmd, T_arg arg0, T_arg arg1, T_arg arg2)
{
  auto out_cmd = Utl::Cmd::Combine(
      Utl::Cmd::Combine(
      Utl::Cmd::Combine(ToFileData(cmd, arg0, arg1, arg2),
                        ToBookData(cmd, arg0, arg1, arg2)),
          ToPageData(cmd, arg0, arg1, arg2)),
        ToNoteData(cmd, arg0, arg1, arg2));
  if (Utl::Cmd::Exists(out_cmd, Cmd::TAB)) OutToTabBar(out_cmd);
  if (Utl::Cmd::Exists(out_cmd, Cmd::LIST)) OutToPageList(out_cmd);
  if (Utl::Cmd::Exists(out_cmd, Cmd::EDITOR)) OutToEditor(out_cmd);
  if (Utl::Cmd::Exists(out_cmd, Cmd::TITLE)) OutToTitleBar(out_cmd);
  if (static_cast<int>(out_cmd) != static_cast<int>(Cmd::NOP)) {
    OutToStatusBar(cmd);
  }
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
  return Utl::Map::Add<T_bid, T_stat>(statset, bid, stat);
}

auto Delete(const T_statset* statset, const T_bid bid) -> T_statset
{
  return Utl::Map::Delete<T_bid, T_stat>(statset, bid);
}

auto Edit(const T_statset* statset, const T_bid bid, const T_stat stat) -> T_statset
{
  return Utl::Map::Edit<T_bid, T_stat>(statset, bid, stat);
}

auto Merge(T_statset* base, T_statset& updated) -> bool
{
  return Utl::Map::Merge<T_bid, T_stat>(base, updated);
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

namespace CurrentId {

auto Fetch(const System* sys) -> T_bid
{
  return sys->r_bid;
}

auto Merge(T_bid* cache, const T_bid updated) -> bool
{
  (*cache) = updated;
  return true;
}

}  // ns CP::Book::CurrentId

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
  return Utl::Map::Add<T_bid, T_path>(pathset, bid, path);
}

auto Delete(const T_pathset* pathset, const T_bid bid) -> T_pathset
{
  return Utl::Map::Delete<T_bid, T_path>(pathset, bid);
}

auto Edit(const T_pathset* pathset, const T_bid bid, const T_path& path) -> T_pathset
{
  return Utl::Map::Edit<T_bid, T_path>(pathset, bid, path);
}

auto Merge(T_pathset* base, T_pathset& updated) -> bool
{
  Utl::Map::Merge<T_bid, T_path>(base, updated);
  return true;
}

}  // ns CP::Book::Names

}  // ns CP::Book

/* CP: Page */
namespace Page {

namespace Id {

auto Fetch(const T_pidsset* pidsset, const T_bid bid, const T_index index) -> T_pid
{
  return Utl::Map::List::Value::Fetch<T_bid, T_pid>(pidsset, bid, index, -1);
}

}  // ns CP::Page::Id

namespace Index {

auto Fetch(const T_pidsset* pidsset, const T_bid bid, const T_pid pid) -> T_index
{
  return Utl::Map::List::Index::Fetch<T_bid, T_pid>(pidsset, bid, pid);
}

}  // ns CP::Page::Index

namespace Ids {

auto Fetch(const T_pidsset* pidsset, const T_bid bid) -> T_pids
{
  return pidsset->value(bid);
}

auto Add(const T_pidsset* pidsset, const T_bid bid, const T_pid pid) -> T_pids
{
  return Utl::Map::List::Add<T_bid, T_pid>(pidsset, bid, pid);
}

auto Delete(const T_pidsset* pidsset, const T_bid bid, const T_pid pid) -> T_pids
{
  return Utl::Map::List::Delete<T_bid, T_pid>(pidsset, bid, pid);
}

auto Move(const T_pidsset* pidsset, const T_bid bid, const T_index from,
          const T_index to) -> T_pids
{
  return Utl::Map::List::Move<T_bid, T_pid>(pidsset, bid, from, to);
}

auto Merge(T_pidsset* pidsset, const T_bid bid, T_pids& pids) -> bool
{
  Utl::Map::List::Merge<T_bid, T_pid>(pidsset, bid, pids);
  return true;
}

auto DeleteAll(T_pidsset* pidsset, const T_bid bid) -> bool
{
  auto tmp = Utl::Map::List::DeleteAll<T_bid, T_pid>(pidsset, bid);
  return Utl::Map::List::MergeAll<T_bid, T_pid>(pidsset, tmp);
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
  return Utl::Map::Add<T_pid, T_name>(nameset, pid, name);
}

auto Delete(const T_nameset* nameset, const T_pid pid) -> T_nameset
{
  return Utl::Map::Delete<T_pid, T_name>(nameset, pid);
}

auto Edit(const T_nameset* nameset, const T_pid pid, const T_name& name) -> T_nameset
{
  return Utl::Map::Edit<T_pid, T_name>(nameset, pid, name);
}

auto Merge(T_nameset* base, T_nameset& updated) -> bool
{
  Utl::Map::Merge<T_pid, T_name>(base, updated);
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
  return Utl::Map::Add<T_bid, T_pid>(pidset, bid, pid);
}

auto Delete(const T_pidset* pidset, const T_bid bid) -> T_pidset
{
  return Utl::Map::Delete<T_bid, T_pid>(pidset, bid);
}

auto Edit(const T_pidset* pidset, const T_bid bid, const T_pid pid) -> T_pidset
{
  return Utl::Map::Edit<T_bid, T_pid>(pidset, bid, pid);
}

auto Merge(T_pidset* base, T_pidset& updated) -> bool
{
  Utl::Map::Merge<T_bid, T_pid>(base, updated);
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
  return Utl::Map::Add<T_pid, T_note>(noteset, pid, note);
}

auto Delete(const T_noteset* noteset, const T_pid pid) -> T_noteset
{
  return Utl::Map::Delete<T_pid, T_note>(noteset, pid);
}

auto Edit(const T_noteset* noteset, const T_pid pid, const T_note& note) -> T_noteset
{
  return Utl::Map::Edit<T_pid, T_note>(noteset, pid, note);
}

auto Merge(T_noteset* base, T_noteset& updated) -> bool
{
  return Utl::Map::Merge<T_pid, T_note>(base, updated);
}

}  // ns CP::Note::Texts

}  // ns CP::Note
}  // ns CP

}  // namespace Nmemo
