/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef BOOKS_H
#define BOOKS_H

#include "common_types.h"
#include "utils.h"

namespace Nmemo {

/* forward declaration */
class Core;

/* process: books */
namespace Books {

namespace Id {

T_bid Fetch(const Core*, const T_tid, const T_book_i);
T_bid Last(const Core*, const T_tid);
T_bid New(Core*);

}  // ns Books::Id
namespace Index {

T_book_i Valid(const Core*, const T_tid, const T_book_i);
T_book_i Fetch(const Core*, const T_tid, const T_bid);

}  // ns Books::Index
namespace Ids {

T_bids Fetch(const Core*, const T_tid);
T_bids Add(const Core*, const T_tid, const T_bid);
T_bids Delete(Core*, const T_tid, const T_bid);
T_bids Move(const Core*, const T_tid, const T_index, const T_index);
T_bids Sort(const Core*, const T_tid, const T_order);
bool Merge(Core*, const T_tid, T_bids);

}  // ns Books::Ids
namespace CurrentId {

T_bid Fetch(const Core*, const T_tid);

}  // ns Books::CurrentId
namespace CurrentIds {

T_bidset Add(const Core*, const T_tid, const T_bid);
T_bidset Delete(const Core*, const T_tid);
T_bidset Edit(const Core*, const T_tid, const T_bid);
bool Merge(Core*, T_bidset);

}  // ns Books::CurrentIds
namespace CurrentIndex {

T_book_i Fetch(const Core*);

}  // ns Books::CurrentIndex
namespace Names {

T_booknames Fetch(const Core*, const T_tid);
T_strset Add(const Core*, const T_bid, const T_name&);
T_strset Delete(Core*, const T_bid);
T_strset Edit(const Core*, const T_bid, const T_name&);
bool Merge(Core*, T_strset);

}  // ns Books::Names
namespace Data {

bool Update(Core*, const T_cmd, const T_book_i, const T_arg);

}  // ns Books::Data
namespace Status {

bool Update(Core*, const T_cmd, const T_book_i, const T_arg);

}  // ns Books::Status
}  // ns Books

}  // ns Nmemo
#endif // BOOKS_H
