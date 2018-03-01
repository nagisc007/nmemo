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
#include <QInputDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QMutex>
#include <QPair>
#include <QStack>
#include <QTabBar>
#include <QTextEdit>

namespace Utl {

/* functors: ID */
struct IdUnit
{
  IdUnit();
  ~IdUnit();
  T_id operator ()();  // to Get
  T_id operator ()(T_id);  // to Release
  T_id next_id;
  QScopedPointer<QStack<T_id>> pool;
};

/* functors: bits */
struct hasCmd
{
  bool operator ()(T_cmd, T_cmd);
};

/* functors: QInputDialog */
struct BookNameToGet
{
  QString operator ()(QWidget*, const QString&);
};

/* functors: QList */
template<typename T>
struct listToAdd
{
  QList<T> operator ()(const QList<T>*, T);
};

template<typename T>
struct listToRemove
{
  QList<T> operator ()(const QList<T>*, T);
};

template<typename T>
struct listToMove
{
  QList<T> operator ()(const QList<T>*, int, int);
};

template<typename T>
struct listValToFetch
{
  T operator ()(const QList<T>*, int, T);
};

template<typename T>
struct listIndexToFetch
{
  int operator ()(const QList<T>*, T);
};

template<typename T>
struct ListToOverride
{
  bool operator ()(QList<T>*, QList<T>&);
};

/* functors: QMap(S, T) */
template<typename S, typename T>
struct listMapToAdd
{
  QMap<S, QList<T>> operator ()(const QMap<S, QList<T>>*, S, T);
};

template<typename S, typename T>
struct listMapToUpdate
{
  QMap<S, QList<T>> operator ()(const QMap<S, QList<T>>*, S, int, T);
};

template<typename S, typename T>
struct listMapToRemove
{
  QMap<S, QList<T>> operator ()(const QMap<S, QList<T>>*, S, T);
};

template<typename S, typename T>
struct listMapToRemoveByKey
{
  QMap<S, QList<T>> operator ()(const QMap<S, QList<T>>*, S);
};

template<typename S, typename T>
struct listMapToMove
{
  QMap<S, QList<T>> operator ()(const QMap<S, QList<T>>*, S, int, int);
};

template<typename S, typename T>
struct listMapValToFetch
{
  T operator ()(const QMap<S, QList<T>>*, S, int, T);
};

template<typename S, typename T>
struct listMapListToFetch
{
  QList<T> operator ()(const QMap<S, QList<T>>*, S);
};

template<typename S, typename T>
struct ListMapToOverride
{
  bool operator ()(QMap<S, QList<T>>*, QMap<S, QList<T>>&);
};

/* functors: QMap(T, QString) */
template<typename T>
struct strMapToUpdate
{
  QMap<T, QString> operator ()(const QMap<T, QString>*, T, const QString&);
};

template<typename T>
struct strMapToRemove
{
  QMap<T, QString> operator ()(const QMap<T, QString>*, T);
};

template<typename T>
struct StrMapToOverride
{
  bool operator ()(QMap<T, QString>*, QMap<T, QString>&);
};

template<typename T>
struct strListFromMapToConv
{
  QStringList operator ()(const QMap<T, QString>*, const QList<T>*);
};

template<typename T>
struct strMapValToFetch
{
  QString operator ()(const QMap<T, QString>*, T);
};

/* functors: QListWidget */
struct ListWidgetToOverride
{
  bool operator ()(QListWidget*, const QStringList&);
};

/* functors: QTabBar */
struct TabBarToOverride
{
  bool operator ()(QTabBar*, const QStringList&);
};

}  // namespace Utl

#endif // UTILS_H
