/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "itempool.h"

#include <QDebug>

namespace NMEMO {

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
auto ItemPool::operator()(const QString& text, const QVariant& val) -> QListWidgetItem*
{
  QListWidgetItem* item = pool_->isEmpty() ? new QListWidgetItem(nullptr, ++next_uid_):
                                             pool_->pop();
  item->setText(text);
  item->setData(Qt::UserRole, val);
  return item;
}

auto ItemPool::Release(QListWidgetItem* item) -> void
{
  pool_->push(item);
}

auto ItemPool::Reset(QListWidget* list) -> void
{
  while (list->count()) {
    pool_->push(list->takeItem(0));
  }
  next_uid_ = 0;
}

}  // namespace NMEMO
