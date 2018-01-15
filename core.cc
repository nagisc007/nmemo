/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#include "core.h"

#include <QDebug>

namespace NMEMO {


Core::Core(QObject *parent) : QObject(parent)
{
  qDebug() << "Core: construct";
}

Core::~Core()
{
  qDebug() << "Core: destruct";
}

}  // namespace NMEMO
