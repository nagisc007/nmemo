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

/* process: control signal */
namespace Sig {

T_sig Combine(const T_sig, const T_sig);
bool Exists(const T_sig, const T_sig);

}  // ns Utl::Sig

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

T_name Input(QWidget*, const T_title&, const T_caption&, const T_text&);
T_name Filter(const T_path&);
T_dirname FilterDirname(const T_path&);

}  // ns Utl::Name

/* process: list */
namespace List {

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
int Valid(const QList<T>*, const int);

template<typename T>
int Fetch(const QList<T>*, const T);

}  // ns Utl::List::Index
}  // ns Utl::List

/* process: Hash */
namespace Hash {

template<typename S, typename T>
QList<T> Filter(const QHash<S, T>*, const QList<S>*);

template<typename S, typename T>
T Fetch(const QHash<S, T>*, const S, const T);

template<typename S, typename T>
QHash<S, T> Add(const QHash<S, T>*, const S, const T);

template<typename S, typename T>
QHash<S, T> Delete(const QHash<S, T>*, const S);

template<typename S, typename T>
QHash<S, T> Edit(const QHash<S, T>*, const S, const T);

template<typename S, typename T>
bool Merge(QHash<S, T>*, QHash<S, T>&);

namespace List {

template<typename S, typename T>
QList<T> Fetch(const QHash<S, QList<T>>*, const S);

template<typename S, typename T>
QList<T> Add(const QHash<S, QList<T>>*, const S, const T);

template<typename S, typename T>
QList<T> Delete(const QHash<S, QList<T>>*, const S, const T);

template<typename S, typename T>
QList<T> Move(const QHash<S, QList<T>>*, const S, const int, const int);

template<typename S, typename T>
bool Merge(QHash<S, QList<T>>*, const S, QList<T>&);

template<typename S, typename T>
QHash<S, QList<T>> DeleteAll(const QHash<S, QList<T>>*, const S);

template<typename S, typename T>
bool MergeAll(QHash<S, QList<T>>*, QHash<S, QList<T>>&);

namespace Index {

template<typename S, typename T>
int Valid(const QHash<S, QList<T>>*, const S, const int);

template<typename S, typename T>
int Fetch(const QHash<S, QList<T>>*, const S, const T);

}  // ns Utl::Hash::List::Index

namespace Value {

template<typename S, typename T>
T Fetch(const QHash<S, QList<T>>*, const S, const int, const T);

}  // ns Utl::Hash::List::Value

}  // ns Utl::Hash::List

}  // ns Utl::Hash

namespace Widget {
namespace Names {

template<typename T>
bool Merge(T*, const T_strs&);

}  // ns Utl::Widget::Names
}  // ns Utl::Widget

}  // ns Utl

#endif // UTILS_H
