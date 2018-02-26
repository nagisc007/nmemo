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
#include <QTabBar>
#include <QTextEdit>

namespace Utl {

/* utils: bits */
struct hasCmd
{
  bool operator ()(CmdSig, CmdSig);
};

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

}  // namespace Utl

#endif // UTILS_H
