/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef FILES_H
#define FILES_H

#include "common_types.h"
#include "utils.h"

namespace Nmemo {

/* forward declaration */
class Core;

/* process: file */
namespace Files {

namespace Data {

bool Update(Core*);

}  // ns Files::Data

namespace Status {

bool Update(Core*);

}  // ns Files::Status

}  // ns Files

}  // ns Nmemo

#endif // FILES_H
