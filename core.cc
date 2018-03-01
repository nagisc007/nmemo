/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "core.h"

namespace Nmemo {

/* class: Core */
Core::Core(QObject* parent): QObject(parent),
  m_tid_(-1),
  m_bid_(-1),
  idu_(new Utl::IdUnit()),
  m_books_(new T_idmap()),
  m_labels_(new T_strmap()),
  m_memos_(new T_strmap()),
  m_paths_(new T_strmap()),
  m_tabs_(new T_ids())
{
  qDebug() << "Core: constructed";
}

Core::~Core()
{
  if (idu_) idu_.reset();
  if (m_books_) {
    m_books_->clear();
    m_books_.reset();
  }
  if (m_labels_) {
    m_labels_->clear();
    m_labels_.reset();
  }
  if (m_memos_) {
    m_memos_->clear();
    m_memos_.reset();
  }
  if (m_paths_) {
    m_paths_->clear();
    m_paths_.reset();
  }
  if (m_tabs_) {
    m_tabs_->clear();
    m_tabs_.reset();
  }
  qDebug() << "Core: destructed";
}

/* methods: features */
auto Core::UpdateData(T_cmd cmd, T_index index, const QString& text, T_arg arg) -> T_idset
{
  auto tab_i = tabIndexToConv()(cmd, index, m_tid_, m_tabs_.data());
  auto book_i = bookIndexToConv()(cmd, index, m_tid_, m_bid_, m_books_.data());
  auto tid = TabsDataUpdator()(cmd, m_tabs_.data(), m_paths_.data(), tab_i, arg);
  auto bid = BooksDataUpdator()(cmd, m_books_.data(), m_labels_.data(), tid, book_i, arg);
  auto arg2 = Utl::hasCmd()(cmd, CmdSig::SET) ? QVariant(text): QVariant("new text");
  m_bid_ = Utl::hasCmd()(cmd, CmdSig::SET) ? bid: m_bid_;
  bid = MemosDataUpdator()(cmd, m_memos_.data(), m_bid_, text, bid, arg2);
  return qMakePair(tid, bid);
}

auto Core::OutputData(T_tid tid, T_bid bid) -> bool
{
  auto tab_i = tabIndexToFetch()(m_tabs_.data(), tid);
  auto tabnames = tabNamesToConv()(m_paths_.data(), m_tabs_.data());
  auto book = bookToFetch()(m_books_.data(), tid);
  auto book_i = book.count() > 0 ? bookIndexToFetch()(&book, bid): -1;
  auto booknames = bookNamesToConv()(m_labels_.data(), &book);
  auto stat = bid > 0 && m_memos_->contains(bid) ? true: false;
  auto memo = stat ? memoToFetch()(m_memos_.data(), bid): QString("");
  emit tabOutputted(tab_i, tabnames);
  emit booksOutputted(book_i, booknames);
  emit memoOutputted(stat, memo);
  return true;
}

/* functors: tabs */
auto Core::tabIndexToConv::operator ()(T_cmd cmd, T_index index, T_tid tid,
                                       const T_tabs* tabs) -> T_tab_i
{
  return Utl::hasCmd()(cmd, CmdSig::TAB) ?
        Utl::hasCmd()(cmd, CmdSig::ADD) ? tabs->count() - 1:
                                          index:
                                          tabs->indexOf(tid);
}

/* functors: books */
auto Core::bookIndexToConv::operator ()(T_cmd cmd, T_index index,
                                        T_tid tid, T_bid bid,
                                       const T_books* books) -> T_book_i
{
  if (!(tid > 0 && books->contains(tid))) return -1;

  switch (cmd) {
  case CmdSig::BOOK_ADD: return books->count() - 1;
  case CmdSig::BOOK_DELETE:
  case CmdSig::BOOK_RENAME: return (*books)[tid].indexOf(bid);
  case CmdSig::BOOK_MOVE: return index;
  default: return -1;
  }
}

/* operator: tabs */
auto Core::TabsDataUpdator::operator ()(T_cmd cmd, T_tabs* tabs, T_paths* paths,
                                        T_tab_i tab_i, T_arg arg) -> T_tid
{
  auto tid = tabIdToFetch()(tabs, tab_i, -1);
  return tid > 0 ?
        PathsOverrider()(cmd, paths,
                         TabsOverrider()(cmd, tabs, tid, tab_i, arg),
                         arg):
        -1;
}

auto Core::TabsOverrider::operator ()(T_cmd cmd, T_tabs* tabs,
                                      T_tid tid, T_tab_i tab_i, T_arg arg) -> T_tid
{
  if (Utl::hasCmd()(cmd, CmdSig::TAB_CHANGE)) return -1;

  auto tabs_ = TabsUpdator()(cmd, tabs, tid, tab_i, arg);
  if (tabs_.count() > 0) {
    return TabsToOverride()(tabs, tabs_) ? tid: -1;
  } else {
    return -1;
  }
}

auto Core::PathsOverrider::operator ()(T_cmd cmd, T_paths* paths,
                                       T_tid tid, T_arg arg) -> T_tid
{
  if (tid < 0 ||
      Utl::hasCmd()(cmd, CmdSig::TAB_CHANGE) ||
      Utl::hasCmd()(cmd, CmdSig::TAB_MOVE)) return tid;

  auto paths_ = PathsUpdator()(cmd, paths, tid, arg);
  if (paths_.count() > 0) {
    return PathsToOverride()(paths, paths_) ? tid: tid;
  } else {
    return tid;
  }
}

auto Core::TabsUpdator::operator ()(T_cmd cmd, T_tabs* tabs,
                                    T_tid tid, T_tab_i tab_i, T_arg arg) -> T_tabs
{
  switch (cmd) {
  case CmdSig::TAB_ADD: return tabsToAdd()(tabs, tid);
  case CmdSig::TAB_DELETE: return tabsToRemove()(tabs, tid);
  case CmdSig::TAB_MOVE: return tabsToMove()(tabs, tab_i, arg.toInt());
  default: return T_tabs();
  }
}

auto Core::PathsUpdator::operator ()(T_cmd cmd, T_paths* paths,
                                     T_tid tid, T_arg arg) -> T_paths
{
  switch (cmd) {
  case CmdSig::TAB_ADD:
  case CmdSig::TAB_RENAME: return pathsToUpdate()(paths, tid, arg.toString());
  case CmdSig::TAB_DELETE: return pathsToRemove()(paths, tid);
  default: return T_paths();
  }
}

/* operator: books */
auto Core::BooksDataUpdator::operator ()(T_cmd cmd, T_books* books, T_labels* labels,
                                         T_tid tid, T_book_i book_i,
                                         T_arg arg) -> T_bid
{
  if (tid < 0) return -1;
  auto book = bookToFetch()(books, tid);
  auto bid = bookIdToFetch()(&book, book_i, -1);
  return LabelsOverrider()(cmd, labels,
                           BooksOverrider()(cmd, books, tid, bid, book_i, arg),
                           arg);
}

auto Core::BooksOverrider::operator ()(T_cmd cmd, T_books* books,
                                       T_tid tid, T_bid bid,
                                       T_book_i book_i, T_arg arg) -> T_bid
{
  if (Utl::hasCmd()(cmd, CmdSig::CHANGE) ||
      Utl::hasCmd()(cmd, CmdSig::TAB_ADD) ||
      Utl::hasCmd()(cmd, CmdSig::TAB_MOVE) ||
      Utl::hasCmd()(cmd, CmdSig::TAB_RENAME)) return -1;

  auto books_ = BooksUpdator()(cmd, books, tid, bid, book_i, arg);
  if (books_.count() > 0) {
    return BooksToOverride()(books, books_) ? bid: -1;
  } else {
    return -1;
  }
}

auto Core::LabelsOverrider::operator ()(T_cmd cmd, T_labels* labels,
                                        T_bid bid, T_arg arg) -> T_bid
{
  if (Utl::hasCmd()(cmd, CmdSig::CHANGE) ||
      Utl::hasCmd()(cmd, CmdSig::TAB_ADD) ||
      Utl::hasCmd()(cmd, CmdSig::TAB_MOVE) ||
      Utl::hasCmd()(cmd, CmdSig::TAB_RENAME)) return -1;

  if (Utl::hasCmd()(cmd, CmdSig::TAB_DELETE)) return bid;

  auto labels_ = LabelsUpdator()(cmd, labels, bid, arg);
  if (labels_.count() > 0) {
    return LabelsToOverride()(labels, labels_) ? bid: -1;
  } else {
    return -1;
  }
}

auto Core::BooksUpdator::operator ()(T_cmd cmd, T_books* books,
                                     T_tid tid, T_bid bid,
                                     T_book_i book_i, T_arg arg) -> T_books
{
  switch (cmd) {
  case CmdSig::BOOK_ADD: return booksToAdd()(books, tid, bid);
  case CmdSig::BOOK_DELETE: return booksToRemove()(books, tid, bid);
  case CmdSig::BOOK_MOVE: return booksToMove()(books, tid, book_i, arg.toInt());
  case CmdSig::TAB_DELETE: return booksToDelete()(books, tid);
  default: return T_books();
  }
}

auto Core::LabelsUpdator::operator ()(T_cmd cmd, T_labels* labels,
                                      T_bid bid, T_arg arg) -> T_labels
{
  switch (cmd) {
  case CmdSig::BOOK_ADD:
  case CmdSig::BOOK_RENAME: return labelsToUpdate()(labels, bid, arg.toString());
  case CmdSig::BOOK_DELETE: return labelsToRemove()(labels, bid);
  default: return T_labels();
  }
}

/* operator: memos */
auto Core::MemosDataUpdator::operator ()(T_cmd cmd, T_memos* memos,
                                         T_bid bid, T_text text,
                                         T_bid bid_w, T_arg arg) -> T_bid
{
  return MemosOverrider()(cmd, memos, bid, text, bid_w, arg);
}

auto Core::MemosOverrider::operator ()(T_cmd cmd, T_memos* memos,
                                       T_bid bid, T_text text,
                                       T_bid bid_w, T_arg arg) -> T_bid
{
  if (bid < 0 &&
      (Utl::hasCmd()(cmd, CmdSig::BOOK_ADD) ||
       Utl::hasCmd()(cmd, CmdSig::BOOK_DELETE))) return -1;

  auto memos_ = MemosUpdator()(memos, bid, text, bid_w, arg);
  if (memos_.count() > 0) {
    return MemosToOverride()(memos, memos_) ? bid_w: -1;
  } else {
    return -1;
  }
}

auto Core::MemosUpdator::operator ()(T_memos* memos, T_bid bid, T_text text,
                                     T_bid bid_w, T_arg arg) -> T_memos
{
  auto memos_ = bid > 0 ? memosToUpdate()(memos, bid, text):
                         T_memos(*memos);
  if (bid_w > 0) memos_ = memosToUpdate()(&memos_, bid_w, arg.toString());
  if ((bid > 0 || bid_w > 0) && memos_.count() > 0) {
    return memos_;
  } else {
    return T_memos();
  }
}

/* slots */
void Core::Update(T_cmd cmd, T_index index, const T_text& text, T_arg arg)
{
  auto idset = UpdateData(cmd, index, text, arg);
  m_tid_ = idset.first;
  m_bid_ = idset.second;
  if (m_tid_ > 0 && OutputData(m_tid_, m_bid_)) {
    // NOTE: notify to do output
    qDebug() << "Core:: Output ok!";
  }
  /*
  qDebug() << "Update Core:: starting ...";
  auto tid_r = Utl::GetTabIdToRead()(cmd, m_tabs_.data(), index, m_tid_);
  auto tid_w = Utl::GetTabIdToWrite()(cmd, m_tabs_.data(), index, tid_r);
  auto tname = Utl::GetTabNameToWrite()(cmd, arg);
  if (tid_r < 0 && tid_w < 0) return;
  qDebug() << "tid:" << tid_r << "|" << tid_w;
  qDebug() << "tname:" << tname;
  auto bid_r = Utl::GetBookIdToRead()(cmd, m_books_.data(), tid_r, index, m_book_i_);
  auto bid_w = Utl::GetBookIdToWrite()(cmd, m_books_.data(), tid_r, index, bid_r);
  auto bname = Utl::GetBookNameToWrite()(cmd, arg);
  qDebug() << "bid:" << bid_r << "|" << bid_w;
  qDebug() << "bname:" << bname;

  auto tab_res = Utl::OperateTabData()(cmd, m_tabs_.data(), m_paths_.data(),
                                       tid_r, tid_w, index, tname, arg);
  auto book_res = Utl::OperateBookData()(cmd, m_books_.data(), m_labels_.data(),
                                         tid_r, tid_w, bid_r, bid_w, index, bname, arg);
  auto memo_res = Utl::OperateMemoData()(cmd, m_memos_.data(), bid_r, bid_w, m_bid_, m_text_);
  m_tid_ = tid_r;
  m_bid_ = bid_r;
  m_book_i_ = book_res.at(0).toInt();
  qDebug() << "tab:: tab_i: " << tab_res.at(0).toInt();
  qDebug() << "tab:: tnames: " << tab_res.at(1).toStringList().count();
  qDebug() << "book:: book_i: " << book_res.at(0).toInt();
  qDebug() << "book:: bnames: " << book_res.at(1).toStringList().count();
  qDebug() << "memo:: stat: " << memo_res.at(0).toBool();
  qDebug() << "memo:: text: " << memo_res.at(1).toString();
  emit tabOutputted(tab_res.at(0).toInt(), tab_res.at(1).toStringList());
  emit booksOutputted(book_res.at(0).toInt(), book_res.at(1).toStringList());
  emit editorOutputted(memo_res.at(0).toBool(), memo_res.at(1).toString());
  */
}

void Core::LoadData(const T_fname &, T_tab_i, const T_text &)
{

}

void Core::SaveData(const T_fname &, T_tab_i, const T_text &)
{

}

}  // namespace Nmemo
