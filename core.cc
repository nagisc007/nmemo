/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "core.h"

namespace Nmemo {

/* functors: tabs */
auto TabsToAdd::operator ()(T_tids* tids, T_labels* labels,
                            const T_name& name) -> T_tid
{
  auto tid = Utl::idGenerated()();
  auto tids_ = tidsAdded()(tids, tid);
  auto labels_ = labelsUpdated()(labels, tid, name);
  if (tids_.count() > 0) TidsToMerge()(tids, tids_);
  if (labels_.count() > 0) LabelsToMerge()(labels, labels_);
  return tid;
}

auto TabsToRemove::operator ()(T_tids* tids, T_labels* labels,
                               T_books* books, T_tab_i tab_i) -> T_tid
{
  auto tid = tabIdFetched()(tids, tab_i, -1);
  auto tids_ = tidsRemoved()(tids, tid);
  auto labels_ = labelsRemoved()(labels, tid);
  BookIdsToRelease()(books, tid);
  auto books_ = booksRemovedTid()(books, tid);
  if (tids_.count() > 0) TidsToMerge()(tids, tids_);
  if (labels_.count() > 0) LabelsToMerge()(labels, labels_);
  if (books_.count() > 0) BooksToMerge()(books, books_);
  Utl::idReleased()(tid);
  return tabIdFetched()(&tids_, tab_i - 1, -1);
}

auto TabsToMove::operator ()(T_tids* tids, T_tab_i tab_i, T_index to) -> T_tid
{
  auto tids_ = tidsMoved()(tids, tab_i, to);
  if (tids_.count() > 0) TidsToMerge()(tids, tids_);
  return tabIdFetched()(&tids_, to, -1);
}

auto TabsToRename::operator ()(const T_tids* tids, T_labels* labels,
                               T_tab_i tab_i, const T_name& name) -> T_tid
{
  auto tid = tabIdFetched()(tids, tab_i, -1);
  auto labels_ = labelsUpdated()(labels, tid, name);
  if (labels_.count() > 0) LabelsToMerge()(labels, labels_);
  return tid;
}

/* functors: books */
auto BooksToAdd::operator ()(T_books* books, T_labels* labels, T_memos* memos,
                             T_tid tid, const T_name& name,
                             const T_text& text) -> T_bid
{
  auto bid = Utl::idGenerated()();
  auto books_ = booksAdded()(books, tid, bid);
  auto labels_ = labelsUpdated()(labels, bid, name);
  auto memos_ = memosUpdated()(memos, bid, text);
  if (books_.count() > 0) BooksToMerge()(books, books_);
  if (labels_.count() > 0) LabelsToMerge()(labels, labels_);
  if (memos_.count() > 0) MemosToMerge()(memos, memos_);
  return bid;
}

auto BooksToRemove::operator ()(T_books* books, T_labels* labels,
                                T_tid tid, T_book_i book_i) -> T_bid
{
  auto bid = bookIdFetched()(books, tid, book_i, -1);
  auto books_ = booksRemoved()(books, tid, bid);
  auto labels_ = labelsRemoved()(labels, bid);
  if (books_.count() > 0) BooksToMerge()(books, books_);
  if (labels_.count() > 0) LabelsToMerge()(labels, labels_);
  Utl::idReleased()(bid);
  return bookIdFetched()(&books_, tid, book_i - 1, -1);
}

auto BooksToMove::operator ()(T_books* books, T_tid tid,
                              T_book_i book_i, T_index to) -> T_bid
{
  auto books_ = booksMoved()(books, tid, book_i, to);
  if (books_.count() > 0) BooksToMerge()(books, books_);
  return bookIdFetched()(&books_, tid, book_i, -1);
}

auto BooksToRename::operator ()(const T_books* books, T_labels* labels,
                                T_tid tid, T_book_i book_i,
                                const T_name& name) -> T_bid
{
  auto bid = bookIdFetched()(books, tid, book_i, -1);
  auto labels_ = labelsUpdated()(labels, bid, name);
  if (labels_.count() > 0) LabelsToMerge()(labels, labels_);
  return bid;
}

auto BookIdsToRelease::operator ()(const T_books* books, T_tid tid) -> bool
{
  auto bids = bidsFetched()(books, tid);
  for (int i = 0, size = bids.count(); i < size; ++i) {
    if (bids.at(i)) Utl::idReleased()(bids.at(i));
  }
  return true;
}

/* class: Core */
Core::Core(QObject* parent): QObject(parent),
  m_tid_(-1),
  m_bid_(-1),
  m_books_(new T_lmap()),
  m_labels_(new T_smap()),
  m_memos_(new T_smap()),
  m_tids_(new T_ids())
{
  qDebug() << "Core: constructed";
}

Core::~Core()
{
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
  if (m_tids_) {
    m_tids_->clear();
    m_tids_.reset();
  }
  qDebug() << "Core: destructed";
}

/* methods: features */
auto Core::UpdateData(T_cmd cmd, T_index index, const T_text& text, T_arg arg) -> bool
{
  UpdateMemoData(m_bid_, text);
  return Utl::hasCmd()(cmd, CmdSig::TAB) ?
        UpdateTabData(cmd, index, arg):
        UpdateBookData(cmd, index, arg);
}

auto Core::OutputData(T_tid tid, T_bid bid) -> bool
{
  auto tab_i = tabIndexFetched()(m_tids_.data(), tid);
  auto tabnames = tabNamesConverted()(m_labels_.data(), m_tids_.data());
  auto book_i = bookIndexFetched()(m_books_.data(), tid, bid);
  auto bids = bidsFetched()(m_books_.data(), tid);
  auto booknames = bookNamesConverted()(m_labels_.data(), &bids);
  auto stat = bid > 0 && m_memos_->contains(bid) ? true: false;
  auto memo = stat ? memoFetched()(m_memos_.data(), bid):
                     QString("Please create new book, or choice a book!");
  emit tabOutputted(tab_i, tabnames);
  emit booksOutputted(book_i, booknames);
  emit memoOutputted(stat, memo);
  return true;
}

auto Core::UpdateTabData(T_cmd cmd, T_tab_i tab_i, T_arg arg) -> bool
{
  T_tid tid = m_tid_;
  T_bid bid = m_bid_;
  switch (cmd) {
  case CmdSig::TAB_ADD:
    tid = TabsToAdd()(m_tids_.data(), m_labels_.data(), arg.toString());
    bid = -1;
    break;
  case CmdSig::TAB_DELETE:
    tid = TabsToRemove()(m_tids_.data(), m_labels_.data(), m_books_.data(),
                         tab_i);
    bid = -1;
    break;
  case CmdSig::TAB_RENAME:
    tid = TabsToRename()(m_tids_.data(), m_labels_.data(), tab_i, arg.toString());
    break;
  case CmdSig::TAB_MOVE:
    tid = TabsToMove()(m_tids_.data(), tab_i, arg.toInt());
    break;
  case CmdSig::TAB_CHANGE:
    tid = tabIdFetched()(m_tids_.data(), tab_i, -1);
    bid = -1;
    break;
  default:
    break;
  }
  m_tid_ = tid;  // To merge
  m_bid_ = bid;  // To merge
  return true;
}

auto Core::UpdateBookData(T_cmd cmd, T_book_i book_i, T_arg arg) -> bool
{
  T_bid bid = m_bid_;
  T_books books;
  T_labels labels;
  switch (cmd) {
  case CmdSig::BOOK_ADD:
    bid = BooksToAdd()(m_books_.data(), m_labels_.data(), m_memos_.data(),
                       m_tid_, arg.toString(), T_text("new text"));
    break;
  case CmdSig::BOOK_DELETE:
    bid = BooksToRemove()(m_books_.data(), m_labels_.data(), m_tid_, book_i);
    break;
  case CmdSig::BOOK_RENAME:
    bid = BooksToRename()(m_books_.data(), m_labels_.data(), m_tid_,
                          book_i, arg.toString());
    break;
  case CmdSig::BOOK_MOVE:
    bid = BooksToMove()(m_books_.data(), m_tid_, book_i, arg.toInt());
    break;
  case CmdSig::BOOK_CHANGE:
    bid = bookIdFetched()(m_books_.data(), m_tid_, book_i, -1);
    break;
  default:
    break;
  }
  m_bid_ = bid;  // To merge
  return true;
}

auto Core::UpdateMemoData(T_bid bid, T_text text) -> bool
{
  // TODO: define ToMerge
  auto memos = memosUpdated()(m_memos_.data(), bid, text);
  if (memos.count() > 0) MemosToMerge()(m_memos_.data(), memos);
  return true;
}

/* slots */
void Core::Update(T_cmd cmd, T_index index, const T_text& text, T_arg arg)
{
  UpdateData(cmd, index, text, arg);
  if (m_tid_ > 0 && OutputData(m_tid_, m_bid_)) {
    // NOTE: notify to do output
    T_text msg;
    switch (cmd) {
    case CmdSig::TAB_ADD: msg = "Added new Tab ...";
      break;
    default: msg = "Nothing any ...";
      break;
    }
    emit statusUpdated(msg);
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
