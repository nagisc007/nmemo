/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "utils.h"

namespace Utl {

/* process: command */
namespace Cmd {

auto Exists(const T_cmd a, const T_cmd b) -> bool
{
  return (static_cast<int>(a) & static_cast<int>(b)) == static_cast<int>(b);
}

}  // ns Utl::Cmd

/* process: Path */
namespace Path {
namespace Load {

auto Input(QWidget* parent, const T_caption& caption, const T_path& path,
           const T_filter& filter, T_filter* selected) -> T_path
{
  return QFileDialog::getOpenFileName(parent, caption, path, filter, selected);
}

}  // ns Utl::Path::Load
namespace Save {

auto Input(QWidget* parent, const T_caption& caption, const T_path& path,
           const T_filter& filter, T_filter* selected) -> T_path
{
  return QFileDialog::getSaveFileName(parent, caption, path, filter, selected);
}

}  // ns Utl::Path::Save
}  // ns Utl::Path

/* process: Name */
namespace Name {

auto Input(QWidget* parent, const T_title& title, const T_caption& caption,
           const T_text& text) -> T_name
{
  return QInputDialog::getText(parent, title, caption, QLineEdit::Normal, text);
}

}  // ns Utl::Name

/* process: Widget */
namespace Widget {

namespace Names {

template<>
auto Merge<QTabBar>(QTabBar*, T_strs strs) -> T_strs
{
  return T_strs();
}
template T_strs Merge<QTabBar>(QTabBar*, T_strs strs);

template<>
auto Merge<QListWidget>(QListWidget*, T_strs strs) -> T_strs
{
  return T_strs();
}
template T_strs Merge<QListWidget>(QListWidget*, T_strs strs);

}  // ns Utl::Widget::Names

}  // ns Utl::Widget

}  // ns Utl
