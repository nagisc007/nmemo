/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "utils.h"

#include <QDebug>

namespace NMEMO {

/* values */
const QString Values::DEFAULT_BOOK_NAME = "New Book";
const QString Values::DEFAULT_BOOK_TEXT = "new text";

/* utils */
auto ItemFindById::operator ()(const QListWidget* list, int item_id) -> QListWidgetItem*
{
  for (int i = 0, size = list->count(); i < size; ++i) {
    if (list->item(i)->type() == item_id) return list->item(i);
  }
  return nullptr;
}

auto ItemGenerator::operator ()(const QString& label, const QString& text,
                                int item_id) -> QListWidgetItem*
{
  auto item = new QListWidgetItem(nullptr, item_id);
  item->setText(label);
  item->setData(Qt::UserRole, text);
  return item;
}

/* class */
ItemPool::ItemPool():
  next_uid_(0),
  pool_(new QStack<QListWidgetItem*>())
{
  qDebug() << "ItemPool: construct";
}

ItemPool::~ItemPool()
{
  if (pool_) {
    pool_->clear();
    pool_.reset();
  }
  qDebug() << "ItemPool: destruct";
}

/* methods: features */
auto ItemPool::operator()(GenerateFnc fn) -> QListWidgetItem*
{
  return !pool_->isEmpty() ?
        pool_->pop():
        fn(Values::DEFAULT_BOOK_NAME, Values::DEFAULT_BOOK_TEXT, ++next_uid_);
}

auto ItemPool::Release(QListWidgetItem* item) -> bool
{
  if (!item) return false;
  pool_->push(item);
  return true;
}

auto ItemPool::ReleaseAll(QListWidget* list) -> bool
{
  while (list->count()) {
    pool_->push(list->takeItem(0));
  }
  next_uid_ = 0;
  return true;
}

}  // namespace NMEMO
