/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#ifndef CORE_H
#define CORE_H

#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>
#include <QTextEdit>

namespace NMEMO {

class Core : public QObject
{
  Q_OBJECT
public:
  explicit Core(QObject *parent = nullptr);
  ~Core();
  // methods: base
  bool SetEditor(QTextEdit*);
  bool SetList(QListWidget*);
  // methods
  void AddItem();
  QListWidgetItem* ItemByUid(int);

signals:
  void bookTitleChanged(QListWidgetItem*);

public slots:
  void OnChangeBook(QListWidgetItem*);
  void OnRequestChangeTitle(QListWidgetItem*);
private:
  int next_uid_;
  int pre_uid_;
  QScopedPointer<QTextEdit> editor_;
  QScopedPointer<QListWidget> list_;
};

}  // namespace NMEMO

#endif // CORE_H
