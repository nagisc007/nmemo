/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "tabs.h"

namespace Nmemo {
/* process: tabs */
namespace Tabs {
namespace Id {

auto Fetch(const Core* c, const T_tab_i tab_i) -> T_tid
{
  return -1;
}

auto Last(const Core* c) -> T_tid
{
  return -1;
}

auto New(Core* c) -> T_tid
{
  return -1;
}

}  // ns Tabs::Id
namespace CurrentId {

auto Fetch(const Core* c) -> T_tid
{
  return -1;
}

auto Merge(Core* c, const T_tid tid) -> bool
{
  return true;
}

}  // ns Tabs::CurrentId
namespace Index {

auto Fetch(const Core* c, const T_tid tid) -> T_tab_i
{
  return -1;
}

}  // ns Tabs::Index
namespace Name {

auto New(const Core* c) -> T_path
{
  return T_path();
}

}  // ns Tabs::Name
namespace Stat {

auto New(const Core* c) -> T_stat
{
  return true;
}

}  // ns Tabs::Stat
namespace Ids {

auto Fetch(const Core* c) -> T_tids
{
  return T_tids();
}

auto Add(const Core* c, const T_tid tid) -> T_tids
{
  return T_tids();
}

auto Delete(Core* c, const T_tid tid) -> T_tids
{
  return T_tids();
}

auto Move(const Core* c, const T_index from, const T_index to) -> T_tids
{
  return T_tids();
}

auto Merge(Core* c, T_tids tids) -> bool
{
  // merge tids
  return true;
}

}  // ns Tabs::Ids
namespace Names {

auto Fetch(const Core* c) -> T_tabnames
{
  return T_tabnames();
}

auto Add(const Core* c, const T_tid tid, const T_path& path) -> T_tabnames
{
  return T_tabnames();
}

auto Delete(const Core* c, const T_tid tid) -> T_tabnames
{
  return T_tabnames();
}

auto Edit(const Core* c, const T_tid tid, const T_path& path) -> T_tabnames
{
  return T_tabnames();
}

auto Merge(Core* c, T_tabnames tabnames) -> bool
{
  return true;
}

}  // ns Tabs::Names
namespace Stats {

auto Fetch(const Core* c) -> T_stats
{
  return T_stats();
}

auto Add(const Core* c, T_tid tid, T_stat stat) -> T_statset
{
  return T_statset();
}

auto Delete(const Core* c, T_tid tid) -> T_statset
{
  return T_statset();
}

auto Edit(const Core* c, const T_tid tid, const T_stat stat) -> T_statset
{
  return T_statset();
}

auto Merge(Core* c, T_statset statset) -> bool
{
  return true;
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
