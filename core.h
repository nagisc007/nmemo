/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef CORE_H
#define CORE_H

#include "utils.h"

#include <QObject>

namespace Nmemo {

/* class: Core */
class Core: public QObject
{
  Q_OBJECT
public:
  /* con[de]structor */
  explicit Core(QObject* parent = nullptr);
  ~Core();

signals:
  void tabOutputted(int, QStringList);
  void booksOutputted(int, QStringList);
  void editorOutputted(bool, const QString&);

public slots:
  void Update(CmdSig, int, QVariant, const QString&);

private:
  int m_tid_;
  int m_bid_;
  int m_book_i_;
  QScopedPointer<T_idpack> m_books_;
  QScopedPointer<T_labels> m_labels_;
  QScopedPointer<T_labels> m_memos_;
  QScopedPointer<T_labels> m_paths_;
  QScopedPointer<T_ids> m_tabs_;
};

}  // namespace Nmemo

#endif // CORE_H
