/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "books.h"

namespace Nmemo {

/* process: books */
namespace Books {

namespace Id {

auto Fetch(const Core* c, const T_tid tid, const T_book_i book_i) -> T_bid
{
  return -1;
}

}  // ns Books::Id

namespace Index {

auto Fetch(const Core* c, const T_tid tid, const T_bid bid) -> T_book_i
{
  return -1;
}

}  // ns Books::Index

namespace Ids {

auto Fetch(const Core* c, const T_tid tid) -> T_bids
{
  return T_bids();
}

auto Add(const Core* c, const T_tid tid, const T_bid bid) -> T_bids
{
  return T_bids();
}

auto Delete(Core* c, const T_tid tid, const T_bid bid) -> T_bids
{
  return T_bids();
}

auto Move(const Core* c, const T_tid tid, const T_index from, const T_index to) -> T_bids
{
  return T_bids();
}

auto Merge(Core* c, const T_tid tid, T_bids bids) -> bool
{
  return true;
}

}  // ns Books::Ids

namespace CurrentId {

auto Fetch(const Core* c, const T_tid tid) -> T_bid
{
  return -1;
}

}  // ns Books::CurrentId

namespace CurrentIds {

auto Add(const Core* c, const T_tid tid, const T_bid bid) -> T_bidset
{
  return T_bidset();
}

auto Delete(const Core* c, const T_tid tid) -> T_bidset
{
  return T_bidset();
}

auto Edit(const Core* c, const T_tid tid, const T_bid bid) -> T_bidset
{
  return T_bidset();
}

auto Merge(Core* c, T_bidset bidset) -> bool
{
  return true;
}

}  // ns Books::CurrentIds

namespace Names {

auto Fetch(const Core* c, const T_tid tid) -> T_booknames
{
  return T_booknames();
}

auto Add(const Core* c, const T_bid bid, const T_name& name) -> T_booknames
{
  return T_booknames();
}

auto Delete(Core* c, const T_bid bid) -> T_booknames
{
  return T_booknames();
}

auto Edit(const Core* c, const T_bid bid, const T_name& name) -> T_booknames
{
  return T_booknames();
}

auto Merge(Core* c, T_booknames booknames) -> bool
{
  return true;
}

}  // ns Books::Names

namespace Data {

auto Update(Core* c, const T_cmd cmd, const T_book_i book_i, const T_arg arg) -> bool
{
  return true;
}

}  // ns Books::Data

namespace Status {

auto Update(Core* c, const T_cmd cmd, const T_book_i book_i, const T_arg arg) -> bool
{
  return true;
}

}  // ns Books::Status
}  // ns Books

}  // ns Nmemo
