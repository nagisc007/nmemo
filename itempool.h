/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef ITEMPOOL_H
#define ITEMPOOL_H

#include <QListWidgetItem>
#include <QStack>

namespace NMEMO {

class ItemPool
{
public:
  explicit ItemPool();
  ~ItemPool();
  // methods: features
  QListWidgetItem* operator()(const QString&, const QVariant&);
  void Release(QListWidgetItem*);
  void Reset(QListWidget*);
private:
  int next_uid_;
  QScopedPointer<QStack<QListWidgetItem*>> pool_;
};

}  // namespace NMEMO

#endif // ITEMPOOL_H
