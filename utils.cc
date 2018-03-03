/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "utils.h"

namespace Utl {

/* values */
T_id next_id = 0;
QStack<T_id> idpool;

/* functors: ID */
auto idGenerated::operator ()() -> T_id
{
  return idpool.isEmpty() ? ++next_id: idpool.pop();
}

auto idReleased::operator ()(T_id id) -> T_id
{
  idpool.push(id);
  return id;
}

/* functors: bits */
auto hasCmd::operator ()(T_cmd a, T_cmd b) -> bool
{
  return (static_cast<int>(a) & static_cast<int>(b)) != 0;
}

/* functors: QInputDialog */
auto bookNameToGet::operator ()(QWidget* parent, const QString& text) -> T_name
{
  return QInputDialog::getText(parent,
                               "Book name.",
                               "Input a book name:",
                               QLineEdit::Normal, text);
}

/* functors: QList */
template<typename T>
auto listAdded<T>::operator ()(const QList<T>* list, T val) -> QList<T>
{
  return (*list) + QList<T>{val};
}

template<typename T>
auto listRemoved<T>::operator ()(const QList<T>* list, T val) -> QList<T>
{
  auto tmp = QList<T>(*list);
  tmp.removeAll(val);
  return tmp;
}

template<typename T>
auto listMoved<T>::operator ()(const QList<T>* list, int from, int to) -> QList<T>
{
  auto tmp = QList<T>(*list);
  tmp.move(from, to);
  return tmp;
}

template<typename T>
auto listValFetched<T>::operator ()(const QList<T>* list, int index, T defval) -> T
{
  return index >= 0 && list->count() > 0 && index < list->count() ?
        list->at(index): defval;
}

template<typename T>
auto listIndexFetched<T>::operator ()(const QList<T>* list, T val) -> int
{
  return list->contains(val) ? list->indexOf(val): -1;
}

template<typename T>
auto ListToMerge<T>::operator ()(QList<T>* list, QList<T>& updated) -> bool
{
  list->swap(updated);
  return true;
}

/* utils: QMap */
template<typename S, typename T>
QMap<S, QList<T>> listMapAdded<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                                  S key, T val)
{
  auto tmp = QMap<S, QList<T>>(*map);
  tmp[key].append(val);
  return tmp;
}

template<typename S, typename T>
QMap<S, QList<T>> listMapUpdated<S, T>::operator ()(const QMap<S, QList<T>>* map,
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
QMap<S, QList<T>> listMapRemoved<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                                    S key, T val)
{
  auto tmp = QMap<S, QList<T>>(*map);
  if (tmp.contains(key)) tmp[key].removeAll(val);
  return tmp;
}

template<typename S, typename T>
QMap<S, QList<T>> listMapRemovedKey<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                                        S key)
{
  auto tmp = QMap<S, QList<T>>(*map);
  if (tmp.contains(key)) tmp.remove(key);
  return tmp;
}

template<typename S, typename T>
QMap<S, QList<T>> listMapMoved<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                                  S key, int from, int to)
{
  auto tmp = QMap<S, QList<T>>(*map);
  if (tmp.contains(key)) tmp[key].move(from, to);
  return tmp;
}

template<typename S, typename T>
auto listMapValFetched<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                        S key, int index, T defval) -> T
{
  return index >= 0 && map->count() > 0 && map->contains(key) &&
      map->value(key).count() > 0 && index < map->value(key).count() ?
        map->value(key).at(index): defval;
}

template<typename S, typename T>
auto listMapIndexFetched<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                        S key, T val) -> int
{
  return map->count() > 0 && map->contains(key) ?
        map->value(key).indexOf(val): -1;
}

template<typename S, typename T>
auto listMapListFetched<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                        S key) -> QList<T>
{
  return map->count() > 0 && map->contains(key) ?
        map->value(key): QList<T>{};
}

template<typename S, typename T>
auto ListMapToMerge<S, T>::operator ()(QMap<S, QList<T>>* map,
                                        QMap<S, QList<T>>& updated) ->  bool
{
  map->swap(updated);
  return true;
}

/* functors: QMap(T, QString) */
template<typename T>
auto strMapUpdated<T>::operator ()(const QMap<T, QString>* map,
                                 T key, const QString& val) -> QMap<T, QString>
{
  auto tmp = QMap<T, QString>(*map);
  tmp[key] = val;
  return tmp;
}

template<typename T>
auto strMapRemoved<T>::operator ()(const QMap<T, QString>* map,
                                   T key) -> QMap<T, QString>
{
  auto tmp = QMap<T, QString>(*map);
  if (tmp.contains(key)) tmp.remove(key);
  return tmp;
}

template<typename T>
auto StrMapToMerge<T>::operator ()(QMap<T, QString>* map,
                                       QMap<T, QString>& updated) -> bool
{
  map->swap(updated);
  return true;
}

template<typename T>
auto strListConvertedFromMap<T>::operator ()(const QMap<T, QString>* labels,
                                    const QList<T>* ids) -> QStringList
{
  QStringList slist;
  for (int i = 0, size = ids->count(); i < size; ++i) {
    if (labels->contains(ids->at(i))) slist << labels->value(ids->at(i));
  }
  return slist;
}

template<typename T>
auto strMapValFetched<T>::operator ()(const QMap<T, QString>* map,
                                    T key) -> QString
{
  return map->value(key);
}

/* utils: QListWidget */
auto ListWidgetToMerge::operator ()(QListWidget* list, const QStringList& slist) -> bool
{
  // NOTE:
  //  The ListWidget bug to remove and add.
  //  If the reason is found, fix it.
  list->clear();
  list->addItems(slist);
  return true;
}

/* utils: QTabBar */
auto TabBarToMerge::operator ()(QTabBar* tab, const QStringList& slist) -> bool
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
template struct listAdded<int>;
template struct listRemoved<int>;
template struct listMoved<int>;
template struct listValFetched<int>;
template struct listIndexFetched<int>;
template struct ListToMerge<int>;
template struct listMapAdded<int, int>;
template struct listMapUpdated<int, int>;
template struct listMapRemoved<int, int>;
template struct listMapRemovedKey<int, int>;
template struct listMapMoved<int, int>;
template struct listMapValFetched<int, int>;
template struct listMapIndexFetched<int, int>;
template struct listMapListFetched<int, int>;
template struct ListMapToMerge<int, int>;
template struct strMapUpdated<int>;
template struct strMapRemoved<int>;
template struct StrMapToMerge<int>;
template struct strListConvertedFromMap<int>;
template struct strMapValFetched<int>;

}  // namespace Utl

