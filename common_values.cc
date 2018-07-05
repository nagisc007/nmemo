/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "common_values.h"

namespace APP {

const int VERSION_MAJOR = 1;
const int VERSION_MINOR = 2;
const int VERSION_MICRO = 2;
const QString VERSION_STRING = QString("%1.%2.%3")
    .arg(QString::number(VERSION_MAJOR))
    .arg(QString::number(VERSION_MINOR))
    .arg(QString::number(VERSION_MICRO));

const QString AUTHORS = "N.T.Works";
const QString NAME = "Nmemo";
const QString DESCRIPTION = "Nmemo is a simple memo editor by Qt.";
const QString LICENSE = "GNU GENERAL PUBLIC LICENSE Version 3";
const QString COPYRIGHT = "Copyright (c) 2018";

}  // ns APP

namespace COMMON {

const QString FILE_EXTENSION = "memo";
const QString FILE_PREFIX = "__NMEMO_DATA__";
const QString FILE_VERSION = "2";
const QString FILE_BASE_VERSION = "1";
const QString SPLITTER = "///";

}  // ns COMMON

namespace DEFAULT {

const QString WINDOW_TITLE = "Nmemo";
const QString FILE_TITLE = "NewFile";
const QString BOOK_TITLE = "NewBook";
const QString PAGE_TITLE = "NewPage";
const QString PAGE_TEXT = "New memo";
const QString PAGE_CAUTION_TEXT = "(Please create a new page!!)";
const int STATUS_MESSAGE_TIME = 3000;  // ms

}  // ns DEFAULT

namespace MSG {

const QString FILE_CHANGED = "The file changed ...";
const QString FILE_CLOSED = "The file closed ...";
const QString FILE_CREATED = "New file created ...";
const QString FILE_OPENED = "The file opened ...";
const QString FILE_MOVED = "The file moved ...";
const QString FILE_RENAMED = "The file renamed ...";
const QString FILE_SAVED = "The file saved ...";
const QString BOOKS_SORTED = "Books sorted ...";
const QString BOOK_ADDED = "New book added ...";
const QString BOOK_CHANGED = "The book changed ...";
const QString BOOK_DELETED = "The book deleted ...";
const QString BOOK_MOVED = "The book moved ...";
const QString BOOK_RENAMED = "The book renamed ...";
const QString PAGES_SORTED = "Pages sorted ...";
const QString PAGE_ADDED = "New page added ...";
const QString PAGE_CHANGED = "The page changed ...";
const QString PAGE_DELETED = "The page deleted ...";
const QString PAGE_MOVED = "The page moved ...";
const QString PAGE_RENAMED = "The page renamed ...";

}  // ns MSG

namespace Nmemo {

namespace ERR {

const QString PREFIX = "__ERR";

}  // ns Nmemo::ERR

namespace PROP {

const int LISTVIEW_MAX_WIDTH = 120;
const qreal EDIT_TAB_DISTANCE = 40;  // pixels

}  // ns Nmemo::PROP

namespace VALUE {

/* for files */
const QString DEFAULT_FILENAME = "Undefined";
const QString LOAD_FILE_CAPTION = "Load file";
const QString SAVE_FILE_CAPTION = "Save file";
const QString FILE_FILTER = "Memo file (*.memo);;All Files (*)";
const QString DEFAULT_SELECTED_FILTER = "Memo file (*.memo)";
const QString ASK_UNSAVED_TITLE = "Unsaved file exits";
const QString ASK_UNSAVED_MESSAGE = "The file is unsaved. Save this file?";

/* for tabbar */
const QColor TAB_MODIFIED_COLOR = QColor::fromRgb(0,0,0);
const QColor TAB_UNMODIFIED_COLOR = QColor::fromRgb(255,0,0);

/* for items */
const QString DEFAULT_LISTITEM_NAME = "New Page";
const QString GET_PAGE_TITLE = "Page Name";
const QString GET_PAGE_CAPTION = "Input a page name: ";

/* for editors */
const QString CAUTION_EDITOR_MESSAGE = "Please create a new page!!";
const QString DEFAULT_EDITOR_TEXT = "new note";

}  // ns Nmemo::VALUE

}  // ns Nmemo
