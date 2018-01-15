/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#include "core.h"

#include <QDebug>

namespace NMEMO {


Core::Core(QObject *parent) : QObject(parent),
  editor_(new QTextEdit()),
  list_view_(new QListView()),
  model_(new QStringListModel()),
  titles_(new QStringList())
{
  qDebug() << "Core: construct";
}

Core::~Core()
{
  if (titles_) {
    titles_->clear();
    titles_.reset();
  }
  if (model_) model_.reset();
  if (list_view_) list_view_.reset();
  if (editor_) editor_.reset();
  qDebug() << "Core: destruct";
}

auto Core::SetEditor(QTextEdit* editor) -> bool
{
  editor_.reset(editor);
  if (editor_.isNull()) return false;
  return true;
}

auto Core::SetListView(QListView* view) -> bool
{
  list_view_.reset(view);
  if (list_view_.isNull()) return false;
  model_->setStringList(titles_.operator*());
  list_view_->setModel(model_.take());
  return true;
}

}  // namespace NMEMO
