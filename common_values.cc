/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "common_values.h"

namespace APP {

namespace VERSION {

const int MAJOR = 1;
const int MINOR = 2;
const int MICRO = 1;

}  // ns APP::VERSION

namespace VALUE {

const QString AUTHORS = "N.T.Works";
const QString NAME = "Nmemo";
const QString VERSION = QString("%1.%2.%3")
    .arg(QString::number(VERSION::MAJOR))
    .arg(QString::number(VERSION::MINOR))
    .arg(QString::number(VERSION::MICRO));
const QString DESCRIPTION = "Nmemo is a simple memo editor by Qt.";
const QString LICENSE = "GNU GENERAL PUBLIC LICENSE Version 3";
const QString COPYRIGHT = "Copyright (c) 2018";

}  // ns APP::VALUE

}  // ns APP

namespace Nmemo {

namespace ERR {

const QString PREFIX = "__ERR";

}  // ns Nmemo::ERR

namespace PROP {

const int LISTVIEW_MAX_WIDTH = 120;
const qreal EDIT_TAB_DISTANCE = 40;  // pixels

}  // ns Nmemo::PROP

namespace VALUE {

/* save info */
const QString SAVE_PREFIX = "__NMEMO_DATA__";
const QString SAVE_VERSION = "1";

/* for files */
const QString FILE_EXT = "memo";
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

/* statusbar */
const int STATUS_MESSAGE_TIME = 3000;
}  // ns Nmemo::VALUE

}  // ns Nmemo
