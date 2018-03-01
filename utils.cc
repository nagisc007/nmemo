/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "utils.h"

namespace Utl {

/* functors: ID */
IdUnit::IdUnit():
  next_id(0),
  pool(new QStack<T_id>())
{}

IdUnit::~IdUnit()
{
  if (pool) {
    pool->clear();
    pool.reset();
  }
}

auto IdUnit::operator ()() -> T_id
{
  return pool->isEmpty() ? ++next_id: pool->pop();
}

auto IdUnit::operator ()(T_id id) -> T_id
{
  pool->push(id);
  return id;
}

/* functors: bits */
auto hasCmd::operator ()(CmdSig a, CmdSig b) -> bool
{
  return (static_cast<int>(a) & static_cast<int>(b)) != 0;
}

/* functors: QInputDialog */
auto BookNameToGet::operator ()(QWidget* parent, const QString& text) -> QString
{
  return QInputDialog::getText(parent,
                               "Book name.",
                               "Input a book name:",
                               QLineEdit::Normal, text);
}

/* functors: QList */
template<typename T>
auto listToAdd<T>::operator ()(const QList<T>* list, T val) -> QList<T>
{
  return (*list) + QList<T>{val};
}

template<typename T>
auto listToRemove<T>::operator ()(const QList<T>* list, T val) -> QList<T>
{
  auto tmp = QList<T>(*list);
  tmp.removeAll(val);
  return tmp;
}

template<typename T>
auto listToMove<T>::operator ()(const QList<T>* list, int from, int to) -> QList<T>
{
  auto tmp = QList<T>(*list);
  tmp.move(from, to);
  return tmp;
}

template<typename T>
auto listValToFetch<T>::operator ()(const QList<T>* list, int index, T defval) -> T
{
  return index >= 0 && list->count() > 0 && index < list->count() ?
        list->at(index): defval;
}

template<typename T>
auto listIndexToFetch<T>::operator ()(const QList<T>* list, T val) -> int
{
  return list->contains(val) ? list->indexOf(val): -1;
}

template<typename T>
auto ListToOverride<T>::operator ()(QList<T>* list, QList<T>& updated) -> bool
{
  list->swap(updated);
  return true;
}

/* utils: QMap */
template<typename S, typename T>
QMap<S, QList<T>> listMapToAdd<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                                  S key, T val)
{
  auto tmp = QMap<S, QList<T>>(*map);
  tmp[key].append(val);
  return tmp;
}

template<typename S, typename T>
QMap<S, QList<T>> listMapToUpdate<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                                    S key, int index, T val)
{
  auto tmp = QMap<S, QList<T>>(*map);
  if (index >= 0 && tmp.contains(key) && index < tmp[key].count()) {
    tmp[key][index] = val;
  } else {
    tmp[key].append(val);
  }
  return tmp;
}

template<typename S, typename T>
QMap<S, QList<T>> listMapToRemove<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                                    S key, T val)
{
  auto tmp = QMap<S, QList<T>>(*map);
  if (tmp.contains(key)) tmp[key].removeAll(val);
  return tmp;
}

template<typename S, typename T>
QMap<S, QList<T>> listMapToRemoveByKey<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                                        S key)
{
  auto tmp = QMap<S, QList<T>>(*map);
  if (tmp.contains(key)) tmp.remove(key);
  return tmp;
}

template<typename S, typename T>
QMap<S, QList<T>> listMapToMove<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                                  S key, int from, int to)
{
  auto tmp = QMap<S, QList<T>>(*map);
  if (tmp.contains(key)) tmp[key].move(from, to);
  return tmp;
}

template<typename S, typename T>
auto listMapValToFetch<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                        S key, int index, T defval) -> T
{
  return index >= 0 && map->count() > 0 && map->contains(key) &&
      map->value(key).count() > 0 && index < map->value(key).count() ?
        map->value(key).at(index): defval;
}

template<typename S, typename T>
auto listMapListToFetch<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                        S key) -> QList<T>
{
  return map->count() > 0 && map->contains(key) ?
        map->value(key): QList<T>{};
}

template<typename S, typename T>
auto ListMapToOverride<S, T>::operator ()(QMap<S, QList<T>>* map,
                                        QMap<S, QList<T>>& updated) ->  bool
{
  map->swap(updated);
  return true;
}

/* functors: QMap(T, QString) */
template<typename T>
auto strMapToUpdate<T>::operator ()(const QMap<T, QString>* map,
                                 T key, const QString& val) -> QMap<T, QString>
{
  auto tmp = QMap<T, QString>(*map);
  tmp[key] = val;
  return tmp;
}

template<typename T>
auto strMapToRemove<T>::operator ()(const QMap<T, QString>* map,
                                   T key) -> QMap<T, QString>
{
  auto tmp = QMap<T, QString>(*map);
  if (tmp.contains(key)) tmp.remove(key);
  return tmp;
}

template<typename T>
auto StrMapToOverride<T>::operator ()(QMap<T, QString>* map,
                                       QMap<T, QString>& updated) -> bool
{
  map->swap(updated);
  return true;
}

template<typename T>
auto strListFromMapToConv<T>::operator ()(const QMap<T, QString>* labels,
                                    const QList<T>* ids) -> QStringList
{
  QStringList slist;
  for (int i = 0, size = ids->count(); i < size; ++i) {
    if (labels->contains(ids->at(i))) slist << labels->value(ids->at(i));
  }
  return slist;
}

template<typename T>
auto strMapValToFetch<T>::operator ()(const QMap<T, QString>* map,
                                    T key) -> QString
{
  return map->value(key);
}

/* utils: QListWidget */
auto ListWidgetToOverride::operator ()(QListWidget* list, const QStringList& slist) -> bool
{
  // NOTE:
  //  The ListWidget bug to remove and add.
  //  If the reason is found, fix it.
  list->clear();
  list->addItems(slist);
  return true;
}

/* utils: QTabBar */
auto TabBarToOverride::operator ()(QTabBar* tab, const QStringList& slist) -> bool
{
  auto tab_size = tab->count();
  auto slist_size = slist.count();
  int i = 0;
  for (int size = qMin(tab_size, slist_size); i < size; ++i) {
    tab->setTabText(i, slist.at(i));
  }
  if (tab_size > slist_size) {
    for (; i < tab_size; ++i) {
      tab->removeTab(i);
    }
  } else {
    for (; i < slist_size; ++i) {
      tab->addTab(slist.at(i));
    }
  }
  return true;
}

/* declare templates */
template struct listToAdd<int>;
template struct listToRemove<int>;
template struct listToMove<int>;
template struct listValToFetch<int>;
template struct listIndexToFetch<int>;
template struct ListToOverride<int>;
template struct listMapToAdd<int, int>;
template struct listMapToUpdate<int, int>;
template struct listMapToRemove<int, int>;
template struct listMapToRemoveByKey<int, int>;
template struct listMapToMove<int, int>;
template struct listMapValToFetch<int, int>;
template struct listMapListToFetch<int, int>;
template struct ListMapToOverride<int, int>;
template struct strMapToUpdate<int>;
template struct strMapToRemove<int>;
template struct StrMapToOverride<int>;
template struct strListFromMapToConv<int>;
template struct strMapValToFetch<int>;

}  // namespace Utl

