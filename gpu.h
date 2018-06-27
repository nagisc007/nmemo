/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef GPU_H
#define GPU_H

#include "common_types.h"
#include "common_values.h"

#include <QObject>

namespace GPU {

class Core : public QObject
{
  Q_OBJECT
public:
  explicit Core(QObject *parent = nullptr);

signals:

public slots:
};

}  // ns GPU

#endif // GPU_H
