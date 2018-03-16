/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "books.h"
#include "core.h"

namespace Nmemo {

/* utils */
static bool TabVerify(const Core* c)
{
  return c->m_tid > 0;
}

static T_tid TabCurrentId(const Core* c)
{
  return c->m_tid;
}

/* process: books */
namespace Books {

namespace Id {

auto Fetch(const Core* c, const T_tid tid, const T_book_i book_i) -> T_bid
{
  return Utl::Map::List::Value::Fetch<T_tid, T_bid>(c->m_bidsset.data(),
                                             tid, book_i, -1);
}

auto Last(const Core* c, const T_tid tid) -> T_bid
{
  return Utl::Map::List::Value::Fetch<T_tid, T_bid>(c->m_bidsset.data(),
                                             tid,
                                             c->m_bidsset->value(tid).count() - 1, -1);
}

auto New(Core* c) -> T_bid
{
  return Utl::ID::Allocate(c->m_idpool.data(), &c->m_next_id);
}

}  // ns Books::Id

namespace Index {

auto Fetch(const Core* c, const T_tid tid, const T_bid bid) -> T_book_i
{
  return Utl::Map::List::Index::Fetch(c->m_bidsset.data(), tid, bid);
}

}  // ns Books::Index

namespace Ids {

auto Fetch(const Core* c, const T_tid tid) -> T_bids
{
  return Utl::Map::List::Fetch<T_tid, T_bid>(c->m_bidsset.data(),
                                             tid);
}

auto Add(const Core* c, const T_tid tid, const T_bid bid) -> T_bids
{
  return Utl::Map::List::Add<T_tid, T_bid>(c->m_bidsset.data(), tid, bid);
}

auto Delete(Core* c, const T_tid tid, const T_bid bid) -> T_bids
{
  Utl::ID::Release(c->m_idpool.data(), bid);
  return Utl::Map::List::Delete<T_tid, T_bid>(c->m_bidsset.data(), tid, bid);
}

auto Move(const Core* c, const T_tid tid, const T_index from, const T_index to) -> T_bids
{
  return Utl::Map::List::Move<T_tid, T_bid>(c->m_bidsset.data(), tid, from, to);
}

auto Sort(const Core* c, const T_tid tid, const T_order order) -> T_bids
{
  auto bids = Fetch(c, tid);
  auto names = Books::Names::Fetch(c, tid);
  T_strs combined;
  for (int i = 0, size = bids.count(); i < size; ++i) {
    combined << names.at(i) + QString("///") + QString::number(bids.at(i));
  }
  if (order == Qt::AscendingOrder) {
    std::sort(combined.begin(), combined.end());
  } else {
    std::sort(combined.begin(), combined.end(), std::greater<QString>());
  }
  T_bids ids;
  for (int i = 0, size = combined.count(); i < size; ++i) {
    ids << combined.at(i).section("///", 1, 1).toInt();
  }
  return ids;
}

auto Merge(Core* c, const T_tid tid, T_bids bids) -> bool
{
  return Utl::Map::List::Merge<T_tid, T_bid>(c->m_bidsset.data(), tid, bids);
}

}  // ns Books::Ids

namespace CurrentId {

auto Fetch(const Core* c, const T_tid tid) -> T_bid
{
  return Utl::Map::Fetch<T_tid, T_bid>(c->m_bidset.data(), tid, -1);
}

}  // ns Books::CurrentId

namespace CurrentIds {

auto Add(const Core* c, const T_tid tid, const T_bid bid) -> T_bidset
{
  return Utl::Map::Add<T_tid, T_bid>(c->m_bidset.data(), tid, bid);
}

auto Delete(const Core* c, const T_tid tid) -> T_bidset
{
  return Utl::Map::Delete<T_tid, T_bid>(c->m_bidset.data(), tid);
}

auto Edit(const Core* c, const T_tid tid, const T_bid bid) -> T_bidset
{
  return Utl::Map::Edit<T_tid, T_bid>(c->m_bidset.data(), tid, bid);
}

auto Merge(Core* c, T_bidset bidset) -> bool
{
  return Utl::Map::Merge<T_tid, T_bid>(c->m_bidset.data(), bidset);
}

}  // ns Books::CurrentIds

namespace CurrentIndex {

auto Fetch(const Core* c) -> T_book_i
{
  return Books::Index::Fetch(c, TabCurrentId(c),
                             Books::CurrentId::Fetch(c, TabCurrentId(c)));
}

}  // ns Books::CurrentIndex

namespace Names {

auto Fetch(const Core* c, const T_tid tid) -> T_booknames
{
  auto tmp = Books::Ids::Fetch(c, tid);
  return Utl::Map::Filter<T_bid, T_name>(c->m_labels.data(),
                                         &tmp);
}

auto Add(const Core* c, const T_bid bid, const T_name& name) -> T_strset
{
  return Utl::Map::Add<T_bid, T_name>(c->m_labels.data(), bid, name);
}

auto Delete(Core* c, const T_bid bid) -> T_strset
{
  return Utl::Map::Delete<T_bid, T_name>(c->m_labels.data(), bid);
}

auto Edit(const Core* c, const T_bid bid, const T_name& name) -> T_strset
{
  return Utl::Map::Edit<T_bid, T_name>(c->m_labels.data(), bid, name);
}

auto Merge(Core* c, T_strset strset) -> bool
{
  return Utl::Map::Merge<T_bid, T_name>(c->m_labels.data(), strset);
}

}  // ns Books::Names

namespace Data {

auto Update(Core* c, const T_cmd cmd, const T_book_i book_i, const T_arg arg) -> bool
{
  if (!TabVerify(c)) return false;

  T_tid tid_ = TabCurrentId(c);
  T_bid bid_ = -1;
  switch (cmd) {
  case Cmd::BOOK_ADD:
    bid_ = Id::New(c);
    Ids::Merge(c, tid_, Ids::Add(c, tid_, bid_));
    Names::Merge(c, Names::Add(c, bid_, arg.toString()));
    break;
  case Cmd::BOOK_DELETE:
    bid_ = Id::Fetch(c, tid_, book_i);
    Ids::Merge(c, tid_, Ids::Delete(c, tid_, bid_));
    Names::Merge(c, Names::Delete(c, bid_));
    break;
  case Cmd::BOOK_CHANGE:
    break;
  case Cmd::BOOK_MOVE:
    Ids::Merge(c, tid_, Ids::Move(c, tid_, book_i, arg.toInt()));
    break;
  case Cmd::BOOK_RENAME:
    bid_ = Id::Fetch(c, tid_, book_i);
    Names::Merge(c, Names::Edit(c, bid_, arg.toString()));
    break;
  case Cmd::BOOK_SORT:
    Ids::Merge(c, tid_, Ids::Sort(c, tid_, static_cast<Qt::SortOrder>(arg.toInt())));
    break;
  default:
    return false;
  }
  return true;
}

}  // ns Books::Data

namespace Status {

auto Update(Core* c, const T_cmd cmd, const T_book_i book_i, const T_arg arg) -> bool
{
  Q_UNUSED(arg);
  if (!TabVerify(c)) return false;

  T_tid tid_ = TabCurrentId(c);
  T_bid bid_ = -1;
  switch (cmd) {
  case Cmd::BOOK_ADD:
    CurrentIds::Merge(c, CurrentIds::Edit(c, tid_, Id::Last(c, tid_)));
    break;
  case Cmd::BOOK_DELETE:
    CurrentIds::Merge(c, CurrentIds::Delete(c, tid_));
    break;
  case Cmd::BOOK_CHANGE:
    bid_ = Id::Fetch(c, tid_, book_i);
    CurrentIds::Merge(c, CurrentIds::Edit(c, tid_, bid_));
    break;
  case Cmd::BOOK_MOVE:
  case Cmd::BOOK_RENAME:
  case Cmd::BOOK_SORT:
  default:
    return false;
  }
  return true;
}

}  // ns Books::Status
}  // ns Books

}  // ns Nmemo
