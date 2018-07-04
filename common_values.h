/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef COMMON_VALUES_H
#define COMMON_VALUES_H

#include "common_types.h"

namespace APP {

extern const int VERSION_MAJOR;
extern const int VERSION_MINOR;
extern const int VERSION_MICRO;
extern const QString VERSION_STRING;

extern const QString NAME;
extern const QString AUTHORS;
extern const QString DESCRIPTION;
extern const QString LICENSE;
extern const QString COPYRIGHT;

}  // ns APP

namespace COMMON {

extern const QString FILE_EXTENSION;
extern const QString FILE_PREFIX;
extern const QString FILE_VERSION;
extern const QString SPLITTER;

}  // ns COMMON

namespace DEFAULT {

extern const QString WINDOW_TITLE;
extern const QString FILE_TITLE;
extern const QString BOOK_TITLE;
extern const QString PAGE_TITLE;
extern const QString PAGE_TEXT;
extern const QString PAGE_CAUTION_TEXT;
extern const int STATUS_MESSAGE_TIME;

}  // ns DEFAULT

namespace MSG {

extern const QString FILE_CHANGED;
extern const QString FILE_CLOSED;
extern const QString FILE_CREATED;
extern const QString FILE_MOVED;
extern const QString FILE_OPENED;
extern const QString FILE_RENAMED;
extern const QString FILE_SAVED;
extern const QString BOOK_ADDED;
extern const QString BOOK_CHANGED;
extern const QString BOOK_DELETED;
extern const QString BOOK_MOVED;
extern const QString BOOK_RENAMED;
extern const QString BOOKS_SORTED;
extern const QString PAGE_ADDED;
extern const QString PAGE_CHANGED;
extern const QString PAGE_DELETED;
extern const QString PAGE_MOVED;
extern const QString PAGE_RENAMED;
extern const QString PAGES_SORTED;

}  // ns MSG

/* values: Common */
namespace Nmemo {

namespace ERR {

extern const QString PREFIX;

}  // ns Nmemo::Err

namespace PROP {

extern const int LISTVIEW_MAX_WIDTH;
extern const qreal EDIT_TAB_DISTANCE;

}  // ns Nmemo::PROP

namespace VALUE {

extern const QString ASK_UNSAVED_TITLE;
extern const QString ASK_UNSAVED_MESSAGE;
extern const QString CAUTION_EDITOR_MESSAGE;
extern const QString DEFAULT_EDITOR_TEXT;
extern const QString DEFAULT_FILENAME;
extern const QString DEFAULT_LISTITEM_NAME;
extern const QString GET_PAGE_TITLE;
extern const QString GET_PAGE_CAPTION;
extern const QString LOAD_FILE_CAPTION;
extern const QString SAVE_FILE_CAPTION;
extern const QString FILE_FILTER;
extern const QString DEFAULT_SELECTED_FILTER;
extern const QColor TAB_MODIFIED_COLOR;
extern const QColor TAB_UNMODIFIED_COLOR;

}  // ns Nmemo::VALUE

}  // ns Nmemo

#endif // COMMON_VALUES_H
