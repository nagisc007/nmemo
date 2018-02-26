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
#include <QPair>
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
template<typename T>
struct listAdded
{
  QList<T> operator ()(const QList<T>*, T);
};

template<typename T>
struct listRemoved
{
  QList<T> operator ()(const QList<T>*, T);
};

template<typename T>
struct listMoved
{
  QList<T> operator ()(const QList<T>*, int, int);
};

template<typename T>
struct OverrideList
{
  void operator ()(QList<T>*, QList<T>&);
};

/* utils: QMap */
template<typename S, typename T>
struct listMapAdded
{
  QMap<S, QList<T>> operator ()(const QMap<S, QList<T>>*, S, T);
};

template<typename S, typename T>
struct listMapRemoved
{
  QMap<S, QList<T>> operator ()(const QMap<S, QList<T>>*, S, T);
};

template<typename S, typename T>
struct listMapMoved
{
  QMap<S, QList<T>> operator ()(const QMap<S, QList<T>>*, S, int, int);
};

template<typename T>
struct strMapAdded
{
  QMap<T, QString> operator ()(const QMap<T, QString>*, T, const QString&);
};

template<typename T>
struct strMapRemoved
{
  QMap<T, QString> operator ()(const QMap<T, QString>*, T);
};

template<typename S, typename T>
struct OverrideListMap
{
  void operator ()(QMap<S, QList<T>>*, QMap<S, QList<T>>&);
};

template<typename T>
struct OverrideStringMap
{
  void operator ()(QMap<T, QString>*, QMap<T, QString>&);
};

/* utils: QStringList */
struct strListDerivedIds
{
  QStringList operator ()(const T_labels*, const T_ids*);
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

/* operation: for tab */
struct pathsOperated
{
  T_labels operator ()(CmdSig, const T_labels*, int, const QString&);
};
struct tabIdFrom
{
  int operator ()(CmdSig, const T_ids*, int);
};

struct tabIndexFrom
{
  int operator ()(CmdSig, const T_ids*, int);
};

struct tabsOperated
{
  T_ids operator ()(CmdSig, const T_ids*, int, int, QVariant);
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

struct OperateTabData
{
  QList<QVariant> operator ()(CmdSig, T_ids*, T_labels*,
                              int, int, int, const QString&, QVariant);
};

/* operation: for book */
struct bookIdFrom
{
  int operator ()(CmdSig, const T_idpack*, int, int);
};

struct bookIndexFrom
{
  int operator ()(CmdSig, const T_idpack*, int, int);
};

struct booksOperated
{
  T_idpack operator ()(CmdSig, const T_idpack*,
                       int, int, int, int, int, QVariant);
};

struct RemovedBooks
{
  T_ids operator ()(CmdSig, T_idpack*, int);
};

struct labelsOperated
{
  T_labels operator ()(CmdSig, const T_labels*,
                       int, int, int, int, const QString&);
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

struct OperateBookData
{
  QList<QVariant> operator ()(CmdSig, T_idpack*, T_labels*,
                              int, int,
                              int, int, int, const QString&, QVariant);
};

}  // namespace Utl

#endif // UTILS_H
