/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef MEMOS_H
#define MEMOS_H

#include "common_types.h"
#include "utils.h"

namespace Nmemo {

/* forward declaration */
class Core;

/* process: memos */
namespace Memos {

namespace Memo {

T_memo Fetch(const Core*, const T_bid);

}  // ns Memos::Memo

namespace Memos {

T_memos Add(const Core*, const T_bid, const T_memo&);
T_memos Delete(Core*, const T_bid);
T_memos Edit(const Core*, const T_bid, const T_memo&);
bool Merge(Core*, T_memos);

}  // ns Memos::Memos

namespace Enabled {

T_stat Fetch(const Core*);
bool Merge(Core*, const T_stat);

}  // ns Memos::Enabled

namespace Cache {

T_memo Fetch(const Core*);
bool Merge(Core*, const T_memo&);

}  // ns Memos::Cache

namespace Data {

bool Update(Core*, const T_cmd, const T_memo&);

}  // ns Memos::Data
namespace Status {

bool Update(Core*, const T_cmd);

}  // ns Memos::Status

}  // ns Memos

}  // ns Nmemo

#endif // MEMOS_H
