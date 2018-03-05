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

// for application
namespace APP {

struct Version {
  static const int MAJOR;
  static const int MINOR;
  static const int MICRO;
};

struct Values {
  static const QString NAME;
  static const QString VERSION;
  static const QString AUTHORS;
  static const QString DESCRIPTION;
  static const QString LICENSE;
  static const QString COPYRIGHT;
};

}  // namespace APP

// for common
namespace Nmemo {

/* values */
struct Props
{
  static const int BOOKLIST_MAX_WIDTH;
  static const qreal EDIT_TAB_DISTANCE;
};

struct Values
{
  static const int STATUS_MESSAGE_TIME;
  static const QString DEFAULT_BOOK_NAME;
  static const QString DEFAULT_FILENAME;
  static const QString FILE_EXT;
  static const QString GET_BOOK_TITLE;
  static const QString GET_BOOK_CAPTION;
  static const QString LOAD_FILE_CAPTION;
  static const QString SAVE_FILE_CAPTION;
  static const QString FILE_FILTER;
  static const QString DEFAULT_SELECTED_FILTER;
  static const QString SAVE_PREFIX;
  static const QString SAVE_VERSION;
};

}  // namespace Nmemo

#endif // COMMON_VALUES_H
