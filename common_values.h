/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef COMMON_VALUES_H
#define COMMON_VALUES_H

#include <QString>

/* values: Application */
namespace APP {

namespace VERSION {

extern const int MAJOR;
extern const int MINOR;
extern const int MICRO;

}  // ns APP::VERSION

namespace VALUE {

extern const QString NAME;
extern const QString VERSION;
extern const QString AUTHORS;
extern const QString DESCRIPTION;
extern const QString LICENSE;
extern const QString COPYRIGHT;

}  // ns APP::VALUE

}  // ns APP

/* values: Common */
namespace Nmemo {

namespace PROP {

extern const int LISTVIEW_MAX_WIDTH;
extern const qreal EDIT_TAB_DISTANCE;

}  // ns Nmemo::PROP

namespace VALUE {

extern const int STATUS_MESSAGE_TIME;
extern const QString CAUTION_EDITOR_MESSAGE;
extern const QString DEFAULT_EDITOR_TEXT;
extern const QString DEFAULT_FILENAME;
extern const QString DEFAULT_LISTITEM_NAME;
extern const QString FILE_EXT;
extern const QString GET_PAGE_TITLE;
extern const QString GET_PAGE_CAPTION;
extern const QString LOAD_FILE_CAPTION;
extern const QString SAVE_FILE_CAPTION;
extern const QString FILE_FILTER;
extern const QString DEFAULT_SELECTED_FILTER;
extern const QString SAVE_PREFIX;
extern const QString SAVE_VERSION;

}  // ns Nmemo::VALUE

}  // ns Nmemo

#endif // COMMON_VALUES_H
