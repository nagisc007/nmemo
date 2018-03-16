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
#include "common_values.h"

#include <QColor>
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

/* process: command */
namespace Cmd {

bool Exists(const T_cmd, const T_cmd);

}  // ns Utl::Cmd
/* process: id */
namespace ID {

T_id Allocate(QStack<T_id>*, T_id*);
bool Release(QStack<T_id>*, const T_id);

}  // ns Utl::ID
namespace Path {
namespace Load {

T_path Input(QWidget*,const T_caption&, const T_path&,
                          const T_filter&, T_filter*);
}  // ns Utl::Path::Load
namespace Save {

T_path Input(QWidget*,const T_caption&, const T_path&,
                          const T_filter&, T_filter*);
}  // ns Utl::Path::Save
}  // ns Utl::Path
namespace Name {

T_name Input(QWidget*,
                          const T_title&, const T_caption&, const T_text&);

}  // ns Utl::Name

/* process: list */
namespace List {

template<typename T>
int Valid(const QList<T>*, const int);

template<typename T>
T Fetch(const QList<T>*, const int, const T);

template<typename T>
QList<T> Add(const QList<T>*, const T);

template<typename T>
QList<T> Delete(const QList<T>*, const T);

template<typename T>
QList<T> Move(const QList<T>*, const int, const int);

template<typename T>
bool Merge(QList<T>*, QList<T>&);

namespace Index {

template<typename T>
int Fetch(const QList<T>*, const T);

}  // ns Utl::List::Index
}  // ns Utl::List

/* process: Map */
namespace Map {

template<typename S, typename T>
QList<T> Filter(const QMap<S, T>*, const QList<S>*);

template<typename S, typename T>
T Fetch(const QMap<S, T>*, const S, const T);

template<typename S, typename T>
QMap<S, T> Add(const QMap<S, T>*, const S, const T);

template<typename S, typename T>
QMap<S, T> Delete(const QMap<S, T>*, const S);

template<typename S, typename T>
QMap<S, T> Edit(const QMap<S, T>*, const S, const T);

template<typename S, typename T>
bool Merge(QMap<S, T>*, QMap<S, T>&);

namespace List {

template<typename S, typename T>
QList<T> Fetch(const QMap<S, QList<T>>*, const S);

template<typename S, typename T>
QList<T> Add(const QMap<S, QList<T>>*, const S, const T);

template<typename S, typename T>
QList<T> Delete(const QMap<S, QList<T>>*, const S, const T);

template<typename S, typename T>
QList<T> Move(const QMap<S, QList<T>>*, const S, const int, const int);

template<typename S, typename T>
bool Merge(QMap<S, QList<T>>*, const S, QList<T>&);

namespace Index {

template<typename S, typename T>
int Fetch(const QMap<S, QList<T>>*, const S, const T);

}  // ns Utl::Map::List::Index

namespace Value {

template<typename S, typename T>
T Fetch(const QMap<S, QList<T>>*, const S, const int, const T);

}  // ns Utl::Map::List::Value

}  // ns Utl::Map::List

}  // ns Utl::Map

namespace Widget {
namespace Names {

template<typename T>
bool Merge(T*, const T_strs*);

}  // ns Utl::Widget::Names
}  // ns Utl::Widget

}  // ns Utl

#endif // UTILS_H
