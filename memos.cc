/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "memos.h"

namespace Nmemo {

namespace Memos {

namespace Memo {

auto Fetch(const Core* c, const T_bid bid) -> T_memo
{
  return T_memo();
}

}  // ns Memos::Memo

namespace Memos {

auto Add(const Core* c, const T_bid bid, const T_memo& memo) -> T_memos
{
  return T_memos();
}

auto Delete(Core* c, const T_bid bid) -> T_memos
{
  return T_memos();
}

auto Edit(const Core* c, const T_bid bid, const T_memo& memo) -> T_memos
{
  return T_memos();
}

auto Merge(Core* c, T_memos memos) -> bool
{
  return true;
}

}  // ns Memos::Memos

namespace Enabled {

auto Fetch(const Core* c) -> T_stat
{
  return true;
}

auto Merge(Core* c, const T_stat stat) -> bool
{
  return true;
}

}  // ns Memos::Enabled

namespace Data {

auto Update(Core* c) -> bool
{
  return true;
}

}  // ns Memos::Data

namespace Status {

auto Update(Core* c) -> bool
{
  return true;
}

}  // ns Memos::Status

}  // ns Memos

}  // ns Nmemo
