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
int next_id = 0;
QStack<int> id_pool;

/* utils: bits */
auto hasCmd::operator ()(CmdSig a, CmdSig b) -> bool
{
  return (static_cast<int>(a) & static_cast<int>(b)) != 0;
}

/* utils: for id */
auto GetId::operator ()() -> int
{
  return id_pool.isEmpty() ? ++next_id: id_pool.pop();
}

auto ReleaseId::operator ()(int released) -> int
{
  id_pool.push(released);
  return released;
}

/* utils: QList */
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
auto OverrideList<T>::operator ()(QList<T>* list, QList<T>& updated) -> void
{
  list->swap(updated);
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
QMap<S, QList<T>> listMapRemoved<S, T>::operator ()(const QMap<S, QList<T>>* map,
                                                    S key, T val)
{
  auto tmp = QMap<S, QList<T>>(*map);
  if (tmp.contains(key)) tmp[key].removeAll(val);
  return tmp;
}

template<typename S, typename T>
QMap<S, QList<T>> listMapRemovedList<S, T>::operator ()(const QMap<S, QList<T>>* map,
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

template<typename T>
auto strMapAdded<T>::operator ()(const QMap<T, QString>* map,
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

template<typename S, typename T>
auto OverrideListMap<S, T>::operator ()(QMap<S, QList<T>>* map,
                                        QMap<S, QList<T>>& updated) ->  void
{
  map->swap(updated);
}

template<typename T>
auto OverrideStringMap<T>::operator ()(QMap<T, QString>* map,
                                       QMap<T, QString>& updated) -> void
{
  map->swap(updated);
}

/* utils: QStringList */
auto strListDerivedIds::operator ()(const T_labels* labels,
                                    const T_ids* ids) -> QStringList
{
  QStringList slist;
  for (int i = 0, size = ids->count(); i < size; ++i) {
    if (labels->contains(ids->at(i))) slist << labels->value(ids->at(i));
  }
  return slist;
}

/* utils: QListWidget */
auto OverrideListWidget::operator ()(QListWidget* list, const QStringList& slist) -> void
{
  // NOTE:
  //  The ListWidget bug to remove and add.
  //  If the reason is found, fix it.
  list->clear();
  list->addItems(slist);
}

/* utils: QTabBar */
auto OverrideTabBar::operator ()(QTabBar* tab, const QStringList& slist) -> void
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
}

/* operation: for tab */
auto pathsOperated::operator ()(CmdSig cmd, const T_labels* m_labels,
                                int tid, const QString& tname) -> T_labels
{
  return hasCmd()(cmd, CmdSig::TAB) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          strMapAdded<int>()(m_labels, tid, tname):
          hasCmd()(cmd, CmdSig::DELETE) ?
            strMapRemoved<int>()(m_labels, tid):
            T_labels(*m_labels):
            T_labels(*m_labels);
}

auto tabIdFrom::operator ()(CmdSig cmd, const T_ids* tabs, int index) -> int
{
  return index >= 0 && tabs->count() > 0 && index < tabs->count() ?
        hasCmd()(cmd, CmdSig::TAB_DELETE) ?
          index - 1 < tabs->count() ?
            tabs->at(index - 1):
            -1:
            tabs->at(index):
          -1;
}

auto tabIndexFrom::operator ()(CmdSig cmd, const T_ids* tabs, int tid) -> int
{
  return tid > 0 && tabs->count() > 0 && tabs->contains(tid) ?
        hasCmd()(cmd, CmdSig::TAB_ADD) ?
          tabs->count() - 1:
            tabs->value(tid):
            -1;
}

auto tabsOperated::operator ()(CmdSig cmd, const T_ids* m_tabs,
                               int tid, int index, QVariant arg) -> T_ids
{
  return hasCmd()(cmd, CmdSig::TAB) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          listAdded<int>()(m_tabs, tid):
          hasCmd()(cmd, CmdSig::DELETE) ?
            listRemoved<int>()(m_tabs, tid):
            hasCmd()(cmd, CmdSig::MOVE) ?
              listMoved<int>()(m_tabs, index, arg.toInt()):
              T_ids(*m_tabs):
              T_ids(*m_tabs);
}

auto GetTabIdToRead::operator ()(CmdSig cmd, const T_ids* tabs, int index, int tid) -> int
{
  return hasCmd()(cmd, CmdSig::TAB) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          GetId()():
          tabIdFrom()(cmd, tabs, index):
          tid;
}

auto GetTabIdToWrite::operator ()(CmdSig cmd, const T_ids* tabs, int index, int tid) -> int
{
  return hasCmd()(cmd, CmdSig::TAB) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          tid:
          hasCmd()(cmd, CmdSig::DELETE) ?
            tabIdFrom()(cmd, tabs, index):
            -1:
            -1;
}

auto GetTabNameToWrite::operator ()(CmdSig cmd, QVariant arg) -> QString
{
  return hasCmd()(cmd, CmdSig::TAB) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          QString("Undefined"):
          hasCmd()(cmd, CmdSig::RENAME) ?
            arg.toString():
            QString(""):
            QString("");
}

auto OperateTabData::operator ()(CmdSig cmd, T_ids* m_tabs, T_labels* m_paths,
                                 int tid_r, int tid_w, int index,
                                 const QString& tname, QVariant arg) -> QList<QVariant>
{
  QList<QVariant> result;
  auto tabs = tabsOperated()(cmd, m_tabs, tid_w, index, arg);
  auto paths = pathsOperated()(cmd, m_paths, tid_w, tname);
  auto tnames = strListDerivedIds()(&paths, &tabs);
  auto tab_i = tabIndexFrom()(cmd, &tabs, tid_r);
  OverrideList<int>()(m_tabs, tabs);
  OverrideStringMap<int>()(m_paths, paths);
  result << QVariant(tab_i);
  result << QVariant(tnames);
  return result;
}

/* operation: for book */
auto bookIdFrom::operator ()(CmdSig cmd, const T_idpack* books,
                             int tid, int index) -> int
{
  return index >= 0 && books->contains(tid) &&
      books->value(tid).count() > 0 && index < books->value(tid).count() ?
        hasCmd()(cmd, CmdSig::BOOK_DELETE) ?
          index - 1 < books->value(tid).count() ?
            books->value(tid).at(index - 1):
            -1:
            books->value(tid).at(index):
            -1;
}

auto bookIndexFrom::operator ()(CmdSig cmd, const T_idpack* books,
                                int tid, int bid) -> int
{
  return tid > 0 && books->contains(tid) &&
      books->value(tid).count() > 0 && books->value(tid).contains(bid) ?
        hasCmd()(cmd, CmdSig::BOOK_ADD) ?
          books->value(tid).count() - 1:
          books->value(tid).indexOf(bid):
          -1;
}

auto booksOperated::operator ()(CmdSig cmd, const T_idpack* m_books,
                                int tid_r, int tid_w,
                                int bid_r, int bid_w,
                                int index, QVariant arg) -> T_idpack
{
  return hasCmd()(cmd, CmdSig::BOOK) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          listMapAdded<int, int>()(m_books, tid_w, bid_w):
        hasCmd()(cmd, CmdSig::DELETE) ?
          listMapRemoved<int, int>()(m_books, tid_w, bid_w):
        hasCmd()(cmd, CmdSig::MOVE) ?
          listMapMoved<int, int>()(m_books, tid_w, index, arg.toInt()):
          T_idpack(*m_books):
        hasCmd()(cmd, CmdSig::TAB_DELETE) ?
        listMapRemovedList<int, int>()(m_books, tid_w):
          T_idpack(*m_books);
}

auto labelsOperated::operator ()(CmdSig cmd, const T_labels* m_labels,
                                 int tid_r, int tid_w,
                                 int bid_r, int bid_w, const QString& bname) -> T_labels
{
  return hasCmd()(cmd, CmdSig::BOOK) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          strMapAdded<int>()(m_labels, bid_w, bname):
          hasCmd()(cmd, CmdSig::DELETE) ?
            strMapRemoved<int>()(m_labels, bid_w):
            T_labels(*m_labels):
            T_labels(*m_labels);
}

auto GetBookIdToRead::operator ()(CmdSig cmd, const T_idpack* books,
                                  int tid, int index, int book_i) -> int
{
  return hasCmd()(cmd, CmdSig::BOOK) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          GetId()():
          bookIdFrom()(cmd, books, tid, index):
          bookIdFrom()(cmd, books, tid, book_i);
}

auto GetBookIdToWrite::operator ()(CmdSig cmd, int bid) -> int
{
  return hasCmd()(cmd, CmdSig::BOOK) ?
        hasCmd()(cmd, CmdSig::ADD) ?
          bid:
          -1:
          -1;
}

auto GetBookNameToWrite::operator ()(CmdSig cmd, QVariant arg) -> QString
{
  return hasCmd()(cmd, CmdSig::BOOK) ?
        hasCmd()(cmd, CmdSig::ADD_RENAME) ?
          arg.toString():
          QString(""):
          QString("");
}

auto OperateBookData::operator ()(CmdSig cmd, T_idpack* m_books, T_labels* m_labels,
                                  int tid_r, int tid_w,
                                  int bid_r, int bid_w, int index,
                                  const QString& bname, QVariant arg) -> QList<QVariant>
{
  QList<QVariant> result;
  auto books = booksOperated()(cmd, m_books,
                               tid_r, tid_w, bid_r, bid_w, index, arg);
  auto labels = labelsOperated()(cmd, m_labels,
                                 tid_r, tid_w, bid_r, bid_w, bname);
  auto bnames = strListDerivedIds()(&labels, &books[tid_r]);
  auto book_i = bookIndexFrom()(cmd, &books, tid_r, bid_r);
  OverrideListMap<int, int>()(m_books, books);
  OverrideStringMap<int>()(m_labels, labels);
  result << QVariant(book_i);
  result << QVariant(bnames);
  return result;
}

/* operation: memo */
auto memosOperated::operator ()(CmdSig cmd, const T_labels* m_memos,
                                int bid_w, const QString& text) -> T_labels
{
  return hasCmd()(cmd, CmdSig::BOOK) ?
        hasCmd()(cmd, CmdSig::DELETE) ?
          strMapRemoved<int>()(m_memos, bid_w):
          bid_w > 0 ?
            strMapAdded<int>()(m_memos, bid_w, text):
            T_labels(*m_memos):
            bid_w > 0 ?
              strMapAdded<int>()(m_memos, bid_w, text):
              T_labels(*m_memos);
}

auto OperateMemoData::operator ()(CmdSig cmd, T_labels* m_memos,
                                  int bid_r, int bid_w,
                                  const QString& text) -> QList<QVariant>
{
  QList<QVariant> result;
  auto memos = memosOperated()(cmd, m_memos, bid_w, text);
  auto stat = memos.contains(bid_r) ? true: false;
  auto s_text = stat ?
        memos.value(bid_r):
        "Please create new book!";
  result << QVariant(stat);
  result << QVariant(s_text);
  return result;
}

/* declare templates */
template struct listAdded<int>;
template struct listRemoved<int>;
template struct listMoved<int>;
template struct listMapAdded<int, int>;
template struct listMapRemoved<int, int>;
template struct listMapRemovedList<int, int>;
template struct listMapMoved<int, int>;
template struct strMapAdded<int>;
template struct strMapRemoved<int>;
template struct OverrideList<int>;
template struct OverrideListMap<int, int>;
template struct OverrideStringMap<int>;

}  // namespace Utl

