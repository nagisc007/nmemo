/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "tabs.h"
#include "core.h"

namespace Nmemo {

/* utils */
static bool BookIdsReleaseWith(Core* c, const T_tid tid)
{
  if (!c->m_bidsset->contains(tid)) return false;
  auto bids = c->m_bidsset->value(tid);
  for (int i = 0, size = bids.count(); i < size; ++i) {
    Utl::ID::Release(c->m_idpool.data(), bids.at(i));
  }
  return true;
}

/* process: tabs */
namespace Tabs {
namespace Id {

auto Fetch(const Core* c, const T_tab_i tab_i) -> T_tid
{
  return Utl::List::Fetch<T_tid>(c->m_tids.data(), tab_i, -1);
}

auto Last(const Core* c) -> T_tid
{
  return Utl::List::Fetch<T_tid>(c->m_tids.data(), c->m_tids->count() - 1, -1);
}

auto New(Core* c) -> T_tid
{
  return Utl::ID::Allocate(c->m_idpool.data(), &c->m_next_id);
}

}  // ns Tabs::Id
namespace CurrentId {

auto Fetch(const Core* c) -> T_tid
{
  return c->m_tid;
}

auto Merge(Core* c, const T_tid tid) -> bool
{
  c->m_tid = tid;
  return true;
}

}  // ns Tabs::CurrentId
namespace Index {

auto Fetch(const Core* c, const T_tid tid) -> T_tab_i
{
  return Utl::List::Index::Fetch<T_tid>(c->m_tids.data(), tid);
}

}  // ns Tabs::Index
namespace Name {

auto New(const Core* c) -> T_path
{
  Q_UNUSED(c);
  return T_path(VALUE::DEFAULT_FILENAME);
}

}  // ns Tabs::Name
namespace Stat {

auto New(const Core* c) -> T_stat
{
  Q_UNUSED(c);

  return false;
}

}  // ns Tabs::Stat
namespace Ids {

auto Fetch(const Core* c) -> T_tids*
{
  return c->m_tids.data();
}

auto Add(const Core* c, const T_tid tid) -> T_tids
{
  return Utl::List::Add<T_tid>(c->m_tids.data(), tid);
}

auto Delete(Core* c, const T_tid tid) -> T_tids
{
  Utl::ID::Release(c->m_idpool.data(), tid);
  BookIdsReleaseWith(c, tid);
  return Utl::List::Delete<T_tid>(c->m_tids.data(), tid);
}

auto Move(const Core* c, const T_index from, const T_index to) -> T_tids
{
  return Utl::List::Move<T_tid>(c->m_tids.data(), from, to);
}

auto Merge(Core* c, T_tids tids) -> bool
{
  return Utl::List::Merge<T_tid>(c->m_tids.data(), tids);
}

}  // ns Tabs::Ids
namespace Names {

auto Fetch(const Core* c) -> T_tabnames
{
  return Utl::Map::Filter<T_tid, T_path>(c->m_labels.data(), c->m_tids.data());
}

auto Add(const Core* c, const T_tid tid, const T_path& path) -> T_strset
{
  return Utl::Map::Add<T_tid, T_path>(c->m_labels.data(), tid, path);
}

auto Delete(const Core* c, const T_tid tid) -> T_strset
{
  return Utl::Map::Delete<T_tid, T_path>(c->m_labels.data(), tid);
}

auto Edit(const Core* c, const T_tid tid, const T_path& path) -> T_strset
{
  return Utl::Map::Edit<T_tid, T_path>(c->m_labels.data(), tid, path);
}

auto Merge(Core* c, T_strset strset) -> bool
{
  return Utl::Map::Merge<T_tid, T_path>(c->m_labels.data(), strset);
}

}  // ns Tabs::Names
namespace Stats {

auto Fetch(const Core* c) -> T_stats
{
  return Utl::Map::Filter<T_tid, T_stat>(c->m_statset.data(), c->m_tids.data());
}

auto Add(const Core* c, T_tid tid, T_stat stat) -> T_statset
{
  return Utl::Map::Add<T_tid, T_stat>(c->m_statset.data(), tid, stat);
}

auto Delete(const Core* c, T_tid tid) -> T_statset
{
  return Utl::Map::Delete<T_tid, T_stat>(c->m_statset.data(), tid);
}

auto Edit(const Core* c, const T_tid tid, const T_stat stat) -> T_statset
{
  return Utl::Map::Edit<T_tid, T_stat>(c->m_statset.data(), tid, stat);
}

auto Merge(Core* c, T_statset statset) -> bool
{
  return Utl::Map::Merge<T_tid, T_stat>(c->m_statset.data(), statset);
}

}  // ns Tabs::Stats
namespace Data {

auto Update(Core* c, T_cmd cmd, T_tab_i tab_i , T_arg arg) -> bool
{
  T_tid tid_ = -1;
  switch (cmd) {
  case Cmd::TAB_ADD:
    tid_ = Id::New(c);
    Ids::Merge(c, Ids::Add(c, tid_));
    Names::Merge(c, Names::Add(c, tid_, arg.toString()));
    Stats::Merge(c, Stats::Add(c, tid_, Stat::New(c)));
    break;
  case Cmd::TAB_DELETE:
    tid_ = Id::Fetch(c, tab_i);
    Ids::Merge(c, Ids::Delete(c, tid_));
    Names::Merge(c, Names::Delete(c, tid_));
    Stats::Merge(c, Stats::Delete(c, tid_));
    break;
  case Cmd::TAB_CHANGE:
    break;
  case Cmd::TAB_MOVE:
    Ids::Merge(c, Ids::Move(c, tab_i, arg.toInt()));
    break;
  case Cmd::TAB_RENAME:
    tid_ = Id::Fetch(c, tab_i);
    Names::Merge(c, Names::Edit(c, tid_, arg.toString()));
    break;
  default:
    return false;
  }
  return true;
}

}  // ns Tabs::Data
namespace Status {

auto Update(Core* c, T_cmd cmd, T_tab_i tab_i, T_arg arg) -> bool
{
  Q_UNUSED(arg);

  switch (cmd) {
  case Cmd::TAB_ADD:
    CurrentId::Merge(c, Id::Last(c));
    break;
  case Cmd::TAB_DELETE:
    CurrentId::Merge(c, Id::Fetch(c, tab_i - 1));
    break;
  case Cmd::TAB_CHANGE:
    CurrentId::Merge(c, Id::Fetch(c, tab_i));
    break;
  case Cmd::TAB_MOVE:
  case Cmd::TAB_RENAME:
  default:
    return false;
  }
  return true;
}

}  // ns Tabs::Status
}  // ns Tabs


}  // ns Nmemo
