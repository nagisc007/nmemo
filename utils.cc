/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "utils.h"

namespace Utl {

/* process: command */
namespace Cmd {

auto Exists(const T_cmd a, const T_cmd b) -> bool
{
  return (static_cast<int>(a) & static_cast<int>(b)) == static_cast<int>(b);
}

}  // ns Utl::Cmd

/* process: id */
namespace ID {

auto Allocate(QStack<T_id>* pool, T_id* next_id) -> T_id
{
  return pool->isEmpty() ? ++(*next_id): pool->pop();
}

auto Release(QStack<T_id>* pool, const T_id id) -> bool
{
  pool->push(id);
  return true;
}

}  // ns Utl::ID

/* process: Path */
namespace Path {
namespace Load {

auto Input(QWidget* parent, const T_caption& caption, const T_path& path,
           const T_filter& filter, T_filter* selected) -> T_path
{
  return QFileDialog::getOpenFileName(parent, caption, path, filter, selected);
}

}  // ns Utl::Path::Load
namespace Save {

auto Input(QWidget* parent, const T_caption& caption, const T_path& path,
           const T_filter& filter, T_filter* selected) -> T_path
{
  return QFileDialog::getSaveFileName(parent, caption, path, filter, selected);
}

}  // ns Utl::Path::Save
}  // ns Utl::Path

/* process: Name */
namespace Name {

auto Input(QWidget* parent, const T_title& title, const T_caption& caption,
           const T_text& text) -> T_name
{
  return QInputDialog::getText(parent, title, caption, QLineEdit::Normal, text);
}

}  // ns Utl::Name

/* process: List */
namespace List {

template<typename T>
auto Valid(const QList<T>* li, const int index) -> int
{
  return index >= 0 && li->count() > 0 && index < li->count() ? index: -1;
}
template int Valid<int>(const QList<int>*, const int);

template<typename T>
auto Fetch(const QList<T>* li, const int index, const T def_val) -> T
{
  auto va_index = Valid<T>(li, index);
  return va_index >= 0 ? li->at(va_index): def_val;
}
template int Fetch<int>(const QList<int>*, const int, const int);

template<typename T>
auto Add(const QList<T>* li, const T v) -> QList<T>
{
  return QList<T>(*li) + QList<T>{v};
}
template QList<int> Add<int>(const QList<int>* li, const int v);

template<typename T>
auto Delete(const QList<T>* li, const T val) -> QList<T>
{
  auto tmp = QList<T>(*li);
  if (tmp.contains(val)) tmp.removeAll(val);
  return tmp;
}
template QList<int> Delete<int>(const QList<int>*, const int);

template<typename T>
auto Move(const QList<T>* li, const int from, const int to) -> QList<T>
{
  auto tmp = QList<T>(*li);
  tmp.move(from, to);
  return tmp;
}
template QList<int> Move<int>(const QList<int>*, const int, const int);

template<typename T>
auto Merge(QList<T>* base, QList<T>& updated) -> bool
{
  base->swap(updated);
  return true;
}
template bool Merge<int>(QList<int>*, QList<int>&);

namespace Index {

template<typename T>
auto Fetch(const QList<T>* li, const T val) -> int
{
  return li->indexOf(val);
}
template int Fetch<int>(const QList<int>*, const int);

}  // ns Utl::List::Index
}  // ns Utl::List

/* process: Map */
namespace Map {

template<typename S, typename T>
auto Filter(const QMap<S, T>* map, const QList<S>* li) -> QList<T>
{
  QList<T> tmp;
  for (int i = 0, size = li->count(); i < size; ++i) {
    if (map->contains(li->at(i))) {
      tmp << map->value(li->at(i));
    }
  }
  return tmp;
}
template QList<QString> Filter<int, QString>(const QMap<int, QString>*, const QList<int>*);
template QList<bool> Filter<int, bool>(const QMap<int, bool>*, const QList<int>*);

template<typename S, typename T>
auto Add(const QMap<S, T>* map, const S key, const T val) -> QMap<S, T>
{
  auto tmp = QMap<S, T>(*map);
  tmp[key] = val;
  return tmp;
}
template QMap<int, int> Add<int, int>(const QMap<int, int>*, const int, const int);
template QMap<int, bool> Add<int, bool>(const QMap<int, bool>*, const int, const bool);
template QMap<int, QString> Add<int, QString>(const QMap<int, QString>*,
                                              const int, const QString);

template<typename S, typename T>
auto Delete(const QMap<S, T>* map, const S key) -> QMap<S, T>
{
  auto tmp = QMap<S, T>(*map);
  tmp.remove(key);
  return tmp;
}

template QMap<int, int> Delete<int, int>(const QMap<int, int>*, const int);
template QMap<int, bool> Delete<int, bool>(const QMap<int, bool>*, const int);
template QMap<int, QString> Delete<int, QString>(const QMap<int, QString>*, const int);

template<typename S, typename T>
auto Edit(const QMap<S, T>* map, const S key, const T val) -> QMap<S, T>
{
  auto tmp = QMap<S, T>(*map);
  tmp[key] = val;
  return tmp;
}

template QMap<int, int> Edit<int, int>(const QMap<int, int>*, const int, const int);
template QMap<int, bool> Edit<int, bool>(const QMap<int, bool>*, const int, const bool);
template QMap<int, QString> Edit<int, QString>(const QMap<int, QString>*,
                                               const int, const QString);

template<typename S, typename T>
auto Merge(QMap<S, T>* base, QMap<S, T>& updated) -> bool
{
  base->swap(updated);
  return true;
}
template bool Merge<int, int>(QMap<int, int>*, QMap<int, int>&);
template bool Merge<int, bool>(QMap<int, bool>*, QMap<int, bool>&);
template bool Merge<int, QString>(QMap<int, QString>*, QMap<int, QString>&);

}  // ns Utl::Map

/* process: Widget */
namespace Widget {

namespace Names {

template<>
auto Merge<QTabBar>(QTabBar* tabbar, const T_strs* strs) -> bool
{
  int tab_size = tabbar->count();
  int str_size = strs->count();
  int i = 0;
  for (int size = qMin(tab_size, str_size); i < size; ++i) {
    tabbar->setTabText(i, strs->at(i));
  }
  if (tab_size > str_size) {
    for (; i < tab_size; ++i) {
      tabbar->removeTab(i);
    }
  } else {
    for (; i < str_size; ++i) {
      tabbar->addTab(strs->at(i));
    }
  }
  return true;
}
template bool Merge<QTabBar>(QTabBar*, const T_strs* strs);

template<>
auto Merge<QListWidget>(QListWidget* w, const T_strs* strs) -> bool
{
  // NOTE:
  //  The ListWidget bug to remove and add.
  //  If the reason is found, fix it.
  w->clear();
  w->addItems(*strs);
  return true;
}
template bool Merge<QListWidget>(QListWidget*, const T_strs* strs);

}  // ns Utl::Widget::Names

}  // ns Utl::Widget

}  // ns Utl
