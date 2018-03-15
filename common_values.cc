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
const int MICRO = 0;

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

namespace PROP {

const int BOOKLIST_MAX_WIDTH = 120;
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

/* for books */
const QString DEFAULT_BOOK_NAME = "New Book";
const QString GET_BOOK_TITLE = "Book Name";
const QString GET_BOOK_CAPTION = "Input a book name: ";

/* statusbar */
const int STATUS_MESSAGE_TIME = 3000;
}  // ns Nmemo::VALUE

}  // ns Nmemo
