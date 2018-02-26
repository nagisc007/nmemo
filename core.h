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
  void tabOutputted(int, T_labels);
  void booksOutputted(int, T_labels);
  void editorOutputted(bool, const QString&);

public slots:
  void Update(CmdSig, int, QVariant, const QString&);
};

}  // namespace Nmemo

#endif // CORE_H
