/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "utils.h"

namespace Utl {

/* process: control signal */
namespace Sig {

auto Combine(const T_sig a, const T_sig b) -> T_sig
{
  return static_cast<T_sig>((static_cast<int>(a) | static_cast<int>(b)));
}

auto Exists(const T_sig a, const T_sig b) -> bool
{
  return (static_cast<int>(a) & static_cast<int>(b)) == static_cast<int>(b);
}

}  // ns Utl::Sig

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

auto Filter(const T_path& path) -> T_name
{
  return QFileInfo(path).baseName();
}

auto FilterDirname(const T_path& path) -> T_dirname
{
  return QFileInfo(path).absoluteDir().dirName();
}

}  // ns Utl::Name

/* process: List */
namespace List {

template<typename T>
auto Fetch(const QList<T>* li, const int index, const T def_val) -> T
{
  auto va_index = Index::Valid<T>(li, index);
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
auto Valid(const QList<T>* li, const int index) -> int
{
  return index >= 0 && li->count() > 0 && index < li->count() ? index: -1;
}
template int Valid<int>(const QList<int>*, const int);

template<typename T>
auto Fetch(const QList<T>* li, const T val) -> int
{
  return li->indexOf(val);
}
template int Fetch<int>(const QList<int>*, const int);

}  // ns Utl::List::Index
}  // ns Utl::List

/* process: Hash */
namespace Hash {

template<typename S, typename T>
auto Filter(const QHash<S, T>* map, const QList<S>* li) -> QList<T>
{
  QList<T> tmp;
  for (int i = 0, size = li->count(); i < size; ++i) {
    if (map->contains(li->at(i))) {
      tmp << map->value(li->at(i));
    }
  }
  return tmp;
}
template QList<QString> Filter<int, QString>(const QHash<int, QString>*, const QList<int>*);
template QList<bool> Filter<int, bool>(const QHash<int, bool>*, const QList<int>*);
template QList<EditMode> Filter<int, EditMode>(const QHash<int, EditMode>*, const QList<int>*);

template<typename S, typename T>
auto Fetch(const QHash<S, T>* map, const S key, const T def_val) -> T
{
  return map->value(key, def_val);
}
template int Fetch<int, int>(const QHash<int, int>*, const int, const int);
template QString Fetch<int, QString>(const QHash<int, QString>*, const int, const QString);
template EditMode Fetch<int, EditMode>(const QHash<int, EditMode>*, const int, const EditMode);

template<typename S, typename T>
auto Add(const QHash<S, T>* map, const S key, const T val) -> QHash<S, T>
{
  auto tmp = QHash<S, T>(*map);
  tmp[key] = val;
  return tmp;
}
template QHash<int, int> Add<int, int>(const QHash<int, int>*, const int, const int);
template QHash<int, bool> Add<int, bool>(const QHash<int, bool>*, const int, const bool);
template QHash<int, QString> Add<int, QString>(const QHash<int, QString>*,
                                              const int, const QString);
template QHash<int, EditMode> Add<int, EditMode>(const QHash<int, EditMode>*, const int,
                                                 const EditMode);

template<typename S, typename T>
auto Delete(const QHash<S, T>* map, const S key) -> QHash<S, T>
{
  auto tmp = QHash<S, T>(*map);
  tmp.remove(key);
  return tmp;
}

template QHash<int, int> Delete<int, int>(const QHash<int, int>*, const int);
template QHash<int, bool> Delete<int, bool>(const QHash<int, bool>*, const int);
template QHash<int, QString> Delete<int, QString>(const QHash<int, QString>*, const int);
template QHash<int, EditMode> Delete<int, EditMode>(const QHash<int, EditMode>*, const int);

template<typename S, typename T>
auto Edit(const QHash<S, T>* map, const S key, const T val) -> QHash<S, T>
{
  auto tmp = QHash<S, T>(*map);
  tmp[key] = val;
  return tmp;
}

template QHash<int, int> Edit<int, int>(const QHash<int, int>*, const int, const int);
template QHash<int, bool> Edit<int, bool>(const QHash<int, bool>*, const int, const bool);
template QHash<int, QString> Edit<int, QString>(const QHash<int, QString>*,
                                               const int, const QString);
template QHash<int, EditMode> Edit<int, EditMode>(const QHash<int, EditMode>*, const int,
                                                  const EditMode);
template<typename S, typename T>
auto Merge(QHash<S, T>* base, QHash<S, T>& updated) -> bool
{
  base->swap(updated);
  return true;
}
template bool Merge<int, int>(QHash<int, int>*, QHash<int, int>&);
template bool Merge<int, bool>(QHash<int, bool>*, QHash<int, bool>&);
template bool Merge<int, QString>(QHash<int, QString>*, QHash<int, QString>&);
template bool Merge<int, EditMode>(QHash<int, EditMode>*, QHash<int, EditMode>&);

namespace List {

template<typename S, typename T>
auto Fetch(const QHash<S, QList<T>>* map, const S key) -> QList<T>
{
  return map->contains(key) ? map->value(key): QList<T>();
}
template QList<int> Fetch<int, int>(const QHash<int, QList<int>>*, const int);

template<typename S, typename T>
auto Add(const QHash<S, QList<T>>* map, const S key, const T val) -> QList<T>
{
  auto tmp = map->contains(key) ? QList<T>(map->value(key)): QList<T>();
  return tmp + QList<T>{val};
}
template QList<int> Add<int, int>(const QHash<int, QList<int>>*, const int, const int);

template<typename S, typename T>
auto Delete(const QHash<S, QList<T>>* map, const S key, const T val) -> QList<T>
{
  auto tmp = map->contains(key) ? QList<T>(map->value(key)): QList<T>();
  tmp.removeAll(val);
  return tmp;
}
template QList<int> Delete<int, int>(const QHash<int, QList<int>>*, const int, const int);

template<typename S, typename T>
auto Move(const QHash<S, QList<T>>* map, const S key,
          const int from, const int to) -> QList<T>
{
  auto tmp = map->contains(key) ? QList<T>(map->value(key)): QList<T>();
  tmp.move(from, to);
  return tmp;
}
template QList<int> Move<int, int>(const QHash<int, QList<int>>*, const int,
                                   const int, const int);

template<typename S, typename T>
auto Merge(QHash<S, QList<T>>* map, const S key, QList<T>& updated) -> bool
{
  if (map->contains(key)) {
    (*map)[key].swap(updated);
  } else {
    map->insert(key, updated);
  }
  return true;
}
template bool Merge<int, int>(QHash<int, QList<int>>*, const int, QList<int>&);

template<typename S, typename T>
QHash<S, QList<T>> DeleteAll(const QHash<S, QList<T>>* map, const S key)
{
  auto tmp = QHash<S, QList<T>>(*map);
  if (tmp.contains(key)) {
    tmp.remove(key);
  }
  return tmp;
}
template QHash<int, QList<int>> DeleteAll(const QHash<int, QList<int>>*, const int);

template<typename S, typename T>
auto MergeAll(QHash<S, QList<T>>* map, QHash<S, QList<T>>& updated) -> bool
{
  map->swap(updated);
  return true;
}
template bool MergeAll<int, int>(QHash<int, QList<int>>*, QHash<int, QList<int>>&);

namespace Index {

template<typename S, typename T>
auto Valid(const QHash<S, QList<T>>* map, const S key, const int index) -> int
{
  if (!map->contains(key)) return -1;
  return index >= 0 && map->value(key).count() > 0 && index < map->value(key).count() ?
        index: -1;
}
template int Valid<int, int>(const QHash<int, QList<int>>*, const int, const int);

template<typename S, typename T>
auto Fetch(const QHash<S, QList<T>>* map, const S key, const T val) -> int
{
  if (!map->contains(key)) return -1;
  auto tmp = map->value(key);
  return tmp.indexOf(val);
}
template int Fetch<int, int>(const QHash<int, QList<int>>*, const int, const int);

}  // ns Utl::Map::List::Index

namespace Value {

template<typename S, typename T>
auto Fetch(const QHash<S, QList<T>>* map, const S key, const int index, const T def_val) -> T
{
  if (!map->contains(key)) return def_val;
  auto tmp = map->value(key);
  return (index >= 0 && tmp.count() > 0 && index < tmp.count()) ?
        tmp.at(index): def_val;
}
template int Fetch<int, int>(const QHash<int, QList<int>>*, const int, const int, const int);

}  // ns Utl::Map::List::Value

}  // ns Utl::Map::List

}  // ns Utl::Map

/* process: Widget */
namespace Widget {

namespace Names {

template<>
auto Merge<QTabBar>(QTabBar* tabbar, const T_strs& strs) -> bool
{
  int tab_size = tabbar->count();
  int str_size = strs.count();
  int i = 0;
  for (int size = qMin(tab_size, str_size); i < size; ++i) {
    tabbar->setTabText(i, strs.at(i));
  }
  if (tab_size > str_size) {
    for (; i < tab_size; ++i) {
      tabbar->removeTab(i);
    }
  } else {
    for (; i < str_size; ++i) {
      tabbar->addTab(strs.at(i));
    }
  }
  return true;
}

template<>
auto Merge<QListWidget>(QListWidget* w, const T_strs& strs) -> bool
{
  // NOTE:
  //  The ListWidget bug to remove and add.
  //  If the reason is found, fix it.
  w->clear();
  w->addItems(strs);
  return true;
}

}  // ns Utl::Widget::Names

}  // ns Utl::Widget

}  // ns Utl
