/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#ifndef CORE_H
#define CORE_H

#include "itempool.h"

#include <QListWidget>
#include <QMap>
#include <QObject>
#include <QTextEdit>

namespace NMEMO {

/* utils */
struct ItemGetter {
  QListWidgetItem* operator()(int, QListWidget*);
};

class Core : public QObject
{
  Q_OBJECT
public:
  explicit Core(QObject *parent = nullptr);
  ~Core();
  // constatns
  struct Values {
    static const QString UNDEFINED_FNAME;
    static const QString DEFAULT_BOOK_NAME;
    static const QVariant DEFAULT_VALUE;
  };
  // methods: base
  bool SetEditor(QTextEdit*);
  bool SetList(QListWidget*);
  // methods: features
  void PreSaveContext();
  void SaveToFileInternal(const QString&);

signals:
  void bookTitleChanged(QListWidgetItem*);
  void filenameChanged(const QString&, bool is_modified = false);

public slots:
  void OnAddItem();
  void OnChangeBook(QListWidgetItem*);
  void OnChangeBookAsIndex(int);
  void OnChangeText();
  void OnDeleteItem();
  void OnInsertItem();
  void OnLoadFile(QWidget*);
  void OnRequestChangeTitle(QListWidgetItem*);
  void OnReset();
  void OnSaveToFile(QWidget*, bool is_new = true);
  void OnSortItems(int order = 0);

private:
  int uid_cache_;
  bool is_editing_;
  QString filename_;
  QScopedPointer<QTextEdit> editor_;
  QScopedPointer<QListWidget> list_;
  QScopedPointer<QMap<QString, QString>> datapack_;
  QScopedPointer<ItemPool> item_pool_;
};

}  // namespace NMEMO

#endif // CORE_H
