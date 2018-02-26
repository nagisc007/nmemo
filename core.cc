/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "core.h"

namespace Nmemo {

/* class: Core */
Core::Core(QObject* parent): QObject(parent)
{
  qDebug() << "Core: constructed";
}

Core::~Core()
{
  qDebug() << "Core: destructed";
}

/* slots */
void Core::Update(CmdSig, int, QVariant, const QString &)
{

}

}  // namespace Nmemo
