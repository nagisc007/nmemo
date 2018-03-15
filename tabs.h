/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef TABS_H
#define TABS_H

#include "common_values.h"
#include "utils.h"

namespace Nmemo {
/* forward declaration */
class Core;

/* process: tab */
namespace Tabs {
namespace Id {

T_tid Fetch(const Core*, const T_tab_i);
T_tid Last(const Core*);
T_tid New(Core*);

}  // ns Tabs::Id
namespace CurrentId {

T_tid Fetch(const Core*);
bool Merge(Core*, const T_tid);

}  // ns Tabs::CurrentId
namespace Index {

T_tab_i Fetch(const Core*, const T_tid);

}  // ns Tabs::Index
namespace Name {

T_path New(const Core*);

}  // ns Tabs::Name
namespace Stat {

T_stat New(const Core*);

}  // ns Tabs::Stat
namespace Ids {

T_tids Fetch(const Core*);
T_tids Add(const Core*, const T_tid);
T_tids Delete(Core*, const T_tid);
T_tids Move(const Core*, const T_index, const T_index);
bool Merge(Core*, T_tids);

}  // ns Tabs::Ids
namespace Names {

T_tabnames Fetch(const Core*);
T_tabnames Add(const Core*, const T_tid, const T_path&);
T_tabnames Delete(const Core*, const T_tid);
T_tabnames Edit(const Core*, const T_tid, const T_path&);
bool Merge(Core*, T_tabnames);

}  // ns Tabs::Names
namespace Stats {

T_stats Fetch(const Core*);
T_statset Add(const Core*, const T_tid, const T_stat);
T_statset Delete(const Core*, const T_tid);
T_statset Edit(const Core*, const T_tid, const T_stat);
bool Merge(Core*, T_statset);

}  // ns Tabs::Stats
namespace Data {

bool Update(Core*, T_cmd, T_tab_i, T_arg);

}  // ns Tabs::Data
namespace Status {

bool Update(Core*, T_cmd, T_tab_i, T_arg);

}  // ns Tabs::Status
}  // ns Tabs

}  // ns Nmemo

#endif // TABS_H
