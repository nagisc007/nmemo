/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "common_values.h"

namespace APP {

/* version */
const int Version::MAJOR = 1;
const int Version::MINOR = 1;
const int Version::MICRO = 0;

/* application info */
const QString Values::AUTHORS = "N.T.Works";
const QString Values::NAME = "Nmemo";
const QString Values::VERSION = QString("%1.%2.%3")
    .arg(QString::number(Version::MAJOR))
    .arg(QString::number(Version::MINOR))
    .arg(QString::number(Version::MICRO));
const QString Values::DESCRIPTION = "Nmemo is a simple memo editor by Qt.";
const QString Values::LICENSE = "GNU GENERAL PUBLIC LICENSE Version 3";
const QString Values::COPYRIGHT = "Copyright (c) 2018";

}  // namespace APP

namespace Nmemo {

/* save info */
const QString Values::SAVE_PREFIX = "__NMEMO_DATA__";
const QString Values::SAVE_VERSION = "1";

/* for files */
const QString Values::FILE_EXT = "memo";
const QString Values::DEFAULT_FILENAME = "Undefined";
const QString Values::LOAD_FILE_CAPTION = "Load file";
const QString Values::SAVE_FILE_CAPTION = "Save file";
const QString Values::FILE_FILTER = "Memo file (*.memo);;All Files (*)";
const QString Values::DEFAULT_SELECTED_FILTER = "Memo file (*.memo)";

/* for books */
const QString Values::DEFAULT_BOOK_NAME = "New Book";
const QString Values::GET_BOOK_TITLE = "Book Name";
const QString Values::GET_BOOK_CAPTION = "Input a book name: ";

/* statusbar */
const int Values::STATUS_MESSAGE_TIME = 3000;


}  // namespace Nmemo
