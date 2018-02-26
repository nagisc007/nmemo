/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef UTILS_H
#define UTILS_H

#include "common_types.h"

#include <QDebug>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QMutex>
#include <QStack>
#include <QTabBar>
#include <QTextEdit>

namespace Utl {

/* values */
extern int next_id;
extern QStack<int> id_pool;

/* utils: bits */
struct hasCmd
{
  bool operator ()(CmdSig, CmdSig);
};

/* utils: for id */
struct GetId
{
  int operator ()();
};

struct ReleaseId
{
  int operator ()(int);
};

/* utils: QList */

/* utils: QListWidget */
struct OverrideListWidget
{
  void operator ()(QListWidget*, const QStringList&);
};

/* utils: QTabBar */
struct OverrideTabBar
{
  void operator ()(QTabBar*, const QStringList&);
};

/* operation: for tab */
struct tabIdFrom
{
  int operator ()(CmdSig, const T_ids*, int);
};

struct GetTabIdToRead
{
  int operator ()(CmdSig, const T_ids*, int, int);
};

struct GetTabIdToWrite
{
  int operator ()(CmdSig, const T_ids*, int, int);
};

struct GetTabNameToWrite
{
  QString operator ()(CmdSig, QVariant);
};

/* operation: for book */
struct bookIdFrom
{
  int operator ()(CmdSig, const T_idpack*, int, int);
};

struct GetBookIdToRead
{
  int operator ()(CmdSig, const T_idpack*, int, int, int);
};

struct GetBookIdToWrite
{
  int operator ()(CmdSig, int);
};

struct GetBookNameToWrite
{
  QString operator ()(CmdSig, QVariant);
};

}  // namespace Utl

#endif // UTILS_H
