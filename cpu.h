/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef CPU_H
#define CPU_H

#include "common_types.h"
#include "common_values.h"

#include <QObject>

namespace CPU {

class Core : public QObject
{
  Q_OBJECT
public:
  explicit Core(QObject *parent = nullptr);

signals:

public slots:
};

}  // ns CPU

#endif // CPU_H
