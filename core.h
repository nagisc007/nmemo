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
#include <QStack>
#include <QTextEdit>

namespace NMEMO {

class Core : public QObject
{
  Q_OBJECT
public:
  explicit Core(QObject *parent = nullptr);
  ~Core();
  // constatns
  struct CoreValues {
    static const QString UNDEFINED_FNAME;
  };
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
  void SaveToFile(QWidget*, bool is_new = true);
  void SaveToFileInternal(const QString&);
  void SortItems(int order = 0);

signals:
  void bookTitleChanged(QListWidgetItem*);
  void filenameChanged(const QString&);

public slots:
  void OnChangeBook(QListWidgetItem*);
  void OnChangeBookAsIndex(int);
  void OnRequestChangeTitle(QListWidgetItem*);
private:
  int next_uid_;
  int pre_uid_;
  QString filename_;
  QScopedPointer<QTextEdit> editor_;
  QScopedPointer<QListWidget> list_;
  QScopedPointer<QMap<QString, QString>> datapack_;
  QScopedPointer<QStack<QListWidgetItem*>> item_pool_;
};

}  // namespace NMEMO

#endif // CORE_H
