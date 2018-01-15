/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#ifndef CORE_H
#define CORE_H

#include <QListView>
#include <QObject>
#include <QStringList>
#include <QStringListModel>
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
  bool SetListView(QListView*);

signals:

public slots:

private:
  QScopedPointer<QTextEdit> editor_;
  QScopedPointer<QListView> list_view_;
  QScopedPointer<QStringListModel> model_;
  QScopedPointer<QStringList> titles_;
};

}  // namespace NMEMO

#endif // CORE_H
