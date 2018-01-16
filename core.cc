/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#include "core.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QInputDialog>
namespace NMEMO {


Core::Core(QObject *parent) : QObject(parent),
  next_uid_(0),
  pre_uid_(0),
  editor_(nullptr),
  list_(nullptr)
{
  qDebug() << "Core: construct";
  // connects
  connect(this, &Core::bookTitleChanged, this, &Core::OnChangeBook);
}

Core::~Core()
{
  if (list_) list_.reset();
  if (editor_) editor_.reset();
  qDebug() << "Core: destruct";
}

auto Core::AddItem() -> void
{
  auto item = ItemConstructed();
  list_->addItem(item);
  OnChangeBook(item);
}

auto Core::DeleteItem() -> void
{
  auto item = list_->takeItem(list_->currentRow());
  delete item;
  OnChangeBook(list_->currentItem());
}

auto Core::InsertItem() -> void
{
  auto item = ItemConstructed();
  list_->insertItem(list_->currentRow(), item);
  OnChangeBook(item);
}

auto Core::ItemByUid(int uid) -> QListWidgetItem*
{
  for (int i = 0, size = list_->count(); i < size; ++i) {
    if (list_->item(i)->type() == uid) {
      return list_->item(i);
    }
  }
  return nullptr;
}

auto Core::ItemConstructed() -> QListWidgetItem*
{
  QListWidgetItem* item = new QListWidgetItem(nullptr, ++next_uid_);
  item->setText("New Book");
  item->setData(Qt::UserRole, QVariant("Input new text!"));
  return item;
}

auto Core::SetEditor(QTextEdit* editor) -> bool
{
  Q_ASSERT(editor);

  editor_.reset(editor);
  if (editor_.isNull()) return false;
  return true;
}

auto Core::SetList(QListWidget* view) -> bool
{
  Q_ASSERT(view);

  list_.reset(view);
  if (list_.isNull()) return false;
  // connects
  connect(list_.data(), &QListWidget::itemClicked, this, &Core::OnChangeBook);
  connect(list_.data(), &QListWidget::itemDoubleClicked, this, &Core::OnRequestChangeTitle);
  return true;
}

/*
 * slots
 */
void Core::OnRequestChangeTitle(QListWidgetItem* item)
{
  Q_ASSERT(item);

  auto txt = QInputDialog::getText(nullptr, "Change book title", "Input book title:", QLineEdit::Normal, item->text());
  if (txt == "") return;
  item->setText(txt);
  OnChangeBook(item);
}

void Core::OnChangeBook(QListWidgetItem* item)
{
  if (!item) return;
  if (pre_uid_ > 0) {
    auto item = ItemByUid(pre_uid_);
    if (item) {
      item->setData(Qt::UserRole, QVariant(editor_->toPlainText()));
    }
  }
  editor_->setPlainText(item->data(Qt::UserRole).toString());
  pre_uid_ = item->type();
}

}  // namespace NMEMO
