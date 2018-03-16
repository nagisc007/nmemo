/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "memos.h"
#include "core.h"

namespace Nmemo {

/* utils */
static T_bid BookCurrentId(const Core* c)
{
  return c->m_bidset->value(c->m_tid);
}

namespace Memos {

namespace Memo {

auto Fetch(const Core* c, const T_bid bid) -> T_memo
{
  return Utl::Map::Fetch<T_bid, T_memo>(c->m_memos.data(), bid, "");
}

}  // ns Memos::Memo

namespace Memos {

auto Add(const Core* c, const T_bid bid, const T_memo& memo) -> T_memos
{
  return Utl::Map::Add<T_bid, T_memo>(c->m_memos.data(), bid, memo);
}

auto Delete(Core* c, const T_bid bid) -> T_memos
{
  return Utl::Map::Delete<T_bid, T_memo>(c->m_memos.data(), bid);
}

auto Edit(const Core* c, const T_bid bid, const T_memo& memo) -> T_memos
{
  return Utl::Map::Edit<T_bid, T_memo>(c->m_memos.data(), bid, memo);
}

auto Merge(Core* c, T_memos memos) -> bool
{
  return Utl::Map::Merge<T_bid, T_memo>(c->m_memos.data(), memos);
}

}  // ns Memos::Memos

namespace Enabled {

auto Fetch(const Core* c) -> T_stat
{
  return c->m_editor_enabled;
}

auto Merge(Core* c, const T_stat stat) -> bool
{
  if (c->m_editor_enabled == stat) return false;
  c->m_editor_enabled = stat;
  return true;
}

}  // ns Memos::Enabled

namespace Cache {

auto Fetch(const Core* c) -> T_memo
{
  return c->m_text;
}

auto Merge(Core* c, const T_memo& memo) -> bool
{
  c->m_text = memo;
  return true;
}

}  // ns Memos::Cache

namespace Data {

auto Update(Core* c, const T_cmd cmd, const T_memo& memo) -> bool
{
  if (!CoreUtl::BookVerify(c)) return false;

  T_bid bid_ = BookCurrentId(c);
  switch (cmd) {
  case Cmd::MEMO_ADD:
    Memos::Merge(c, Memos::Add(c, bid_, memo));
    break;
  case Cmd::MEMO_DELETE:
    Memos::Merge(c, Memos::Delete(c, bid_));
    break;
  case Cmd::MEMO_EDIT:
    Memos::Merge(c, Memos::Edit(c, bid_, memo));
    break;
  default:
    return false;
  }
  return true;
}

}  // ns Memos::Data

namespace Status {

auto Update(Core* c, const T_cmd cmd) -> bool
{
  switch (cmd) {
  case Cmd::MEMO_ADD:
    break;
  case Cmd::MEMO_DELETE:
    break;
  case Cmd::MEMO_EDIT:
    break;
  default:
    return false;
  }
  return true;
}

}  // ns Memos::Status

}  // ns Memos

}  // ns Nmemo
