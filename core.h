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
#include <QMap>
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
  void ClearItems();
  void DeleteItem();
  void InsertItem();
  QListWidgetItem* ItemConstructed();
  QListWidgetItem* ItemByUid(int);
  void LoadFromFile(QWidget*);
  void Reset();
  void SaveToFile(QWidget*);
  void SortItems(int order = 0);

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
  QMap<QString, QString> datapack_;
};

}  // namespace NMEMO

#endif // CORE_H
