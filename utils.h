/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef UTILS_H
#define UTILS_H

#include <QListWidgetItem>
#include <QStack>

namespace NMEMO {

using GenerateFnc = std::function<QListWidgetItem*(const QString&, const QString&, int)>;

/* values */
struct Values {
  static const QString DEFAULT_BOOK_NAME;
  static const QString DEFAULT_BOOK_TEXT;
};

/* utils */
struct FilenameValidator {
  QString operator ()(const QString&, const QString&);
};

struct ItemGenerator
{
  QListWidgetItem* operator ()(const QString&, const QString&, int);
};

struct ItemFindById {
  QListWidgetItem* operator ()(const QListWidget*, int);
};

class ItemPool
{
public:
  explicit ItemPool();
  ~ItemPool();
  // methods: features
  QListWidgetItem* operator()(GenerateFnc);
  bool Release(QListWidgetItem*);
  bool ReleaseAll(QListWidget*);
private:
  int next_uid_;
  QScopedPointer<QStack<QListWidgetItem*>> pool_;
};

}  // namespace NMEMO

#endif // UTILS_H
