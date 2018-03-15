/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef UTILS_H
#define UTILS_H

#include "common_types.h"

#include <QColor>
#include <QDebug>
#include <QDataStream>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QFontMetrics>
#include <QInputDialog>
#include <QIODevice>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QMutex>
#include <QPair>
#include <QStack>
#include <QTabBar>
#include <QTextEdit>

namespace Utl {

/* process: command */
namespace Cmd {

bool Exists(const T_cmd, const T_cmd);

}  // ns Utl::Cmd
namespace Path {
namespace Load {

T_path Input(QWidget*,const T_caption&, const T_path&,
                          const T_filter&, T_filter*);
}  // ns Utl::Path::Load
namespace Save {

T_path Input(QWidget*,const T_caption&, const T_path&,
                          const T_filter&, T_filter*);
}  // ns Utl::Path::Save
}  // ns Utl::Path
namespace Name {

T_name Input(QWidget*,
                          const T_title&, const T_caption&, const T_text&);

}  // ns Utl::Name

namespace Widget {
namespace Names {

template<typename T>
T_strs Merge(T*, T_strs);

}  // ns Utl::Widget::Names
}  // ns Utl::Widget

}  // ns Utl

#endif // UTILS_H
