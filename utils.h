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
#include <QDataStream>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QFontMetrics>
#include <QInputDialog>
#include <QIODevice>
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

/* values */
extern T_id next_id;
extern QStack<T_id> idpool;

/* functors: QString */
struct fileNameValidated
{
  T_fname operator ()(const T_fname&, const T_ext&);
};

/* functors: ID */
struct idGenerated
{
  T_id operator ()();
};

struct idReleased
{
  T_id operator ()(T_id);
};

/* functors: CmdSig */
struct hasCmd
{
  bool operator ()(T_cmd, T_cmd);
};

struct haveCmds
{
  bool operator ()(T_cmd, T_cmd, T_cmd);
};

/* functors: QFileInfo */
struct baseNameFetched
{
  T_name operator ()(const T_fname&);
};

/* functors: QInputDialog */
struct NameToGet
{
  T_name operator ()(QWidget*, const T_title&, const T_caption&, const T_text&);
};

/* functors: QFileDialog */
struct LoadNameToGet
{
  T_fname operator ()(QWidget*, const T_caption&, const T_fname&,
                      const T_filter&, T_filter*);
};

struct SaveNameToGet
{
  T_fname operator ()(QWidget*, const T_caption&, const T_fname&,
                      const T_filter&, T_filter*);
};

/* functors: QList */
template<typename T>
struct listIndexValidated
{
  int operator ()(const QList<T>*, int, int);
};

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
struct listValFetched
{
  T operator ()(const QList<T>*, int, T);
};

template<typename T>
struct listIndexFetched
{
  int operator ()(const QList<T>*, T);
};

template<typename T>
struct ListToMerge
{
  bool operator ()(QList<T>*, QList<T>&);
};

/* functors: QMap(S, T) */
template<typename S, typename T>
struct listMapAdded
{
  QMap<S, QList<T>> operator ()(const QMap<S, QList<T>>*, S, T);
};

template<typename S, typename T>
struct listMapUpdated
{
  QMap<S, QList<T>> operator ()(const QMap<S, QList<T>>*, S, int, T);
};

template<typename S, typename T>
struct listMapRemoved
{
  QMap<S, QList<T>> operator ()(const QMap<S, QList<T>>*, S, T);
};

template<typename S, typename T>
struct listMapRemovedKey
{
  QMap<S, QList<T>> operator ()(const QMap<S, QList<T>>*, S);
};

template<typename S, typename T>
struct listMapMoved
{
  QMap<S, QList<T>> operator ()(const QMap<S, QList<T>>*, S, int, int);
};

template<typename S, typename T>
struct listMapValFetched
{
  T operator ()(const QMap<S, QList<T>>*, S, int, T);
};

template<typename S, typename T>
struct listMapIndexFetched
{
  int operator ()(const QMap<S, QList<T>>*, S, T);
};

template<typename S, typename T>
struct listMapListFetched
{
  QList<T> operator ()(const QMap<S, QList<T>>*, S);
};

template<typename S, typename T>
struct ListMapToMerge
{
  bool operator ()(QMap<S, QList<T>>*, QMap<S, QList<T>>&);
};

/* functors: QMap(T, QString) */
template<typename T>
struct strMapUpdated
{
  QMap<T, QString> operator ()(const QMap<T, QString>*, T, const QString&);
};

template<typename T>
struct strMapRemoved
{
  QMap<T, QString> operator ()(const QMap<T, QString>*, T);
};

template<typename T>
struct StrMapToMerge
{
  bool operator ()(QMap<T, QString>*, QMap<T, QString>&);
};

template<typename T>
struct strListConvertedFromMap
{
  QStringList operator ()(const QMap<T, QString>*, const QList<T>*);
};

template<typename T>
struct strMapValFetched
{
  QString operator ()(const QMap<T, QString>*, T);
};

/* functors: QListWidget */
struct ListWidgetToMerge
{
  bool operator ()(QListWidget*, const QStringList&);
};

/* functors: QTabBar */
struct TabBarToMerge
{
  bool operator ()(QTabBar*, const QStringList&);
};

}  // namespace Utl

#endif // UTILS_H
