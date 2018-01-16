/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv2 or any later version                             *
 *                                                                         *
 ***************************************************************************/
#include "core.h"

#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QInputDialog>
#include <QMessageBox>

namespace NMEMO {

/* Constants */
const QString Core::CoreValues::UNDEFINED_FNAME = "Undefined";


Core::Core(QObject *parent) : QObject(parent),
  next_uid_(0),
  pre_uid_(0),
  filename_(CoreValues::UNDEFINED_FNAME),
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
  if (editor_->isReadOnly()) editor_->setReadOnly(false);

  auto item = ItemConstructed();
  list_->addItem(item);
  OnChangeBook(item);
}

auto Core::ClearItems() -> void
{
  while (list_->count()) {
    auto item = list_->takeItem(0);
    delete item;
  }
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

auto Core::LoadFromFile(QWidget* win) -> void
{
  QString filename = QFileDialog::getOpenFileName(win, "Open file", "", "Memo file (*.memo);;All Files (*)");

  if (filename.isEmpty()) return;

  datapack_.clear();

  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::information(win, "Core: cannot open file!", file.errorString());
    return;
  }

  QDataStream in(&file);
  in.setVersion(QDataStream::Qt_5_10);
  in >> datapack_;

  if (datapack_.isEmpty()) {
    QMessageBox::information(win, "No content in file!", "Cannot find any contents!");
    return;
  }
  Reset();
  QMap<QString, QString>::const_iterator it = datapack_.constBegin();
  while (it != datapack_.constEnd()) {
    auto item = ItemConstructed();
    item->setText(it.key().section(":", 1,1));
    item->setData(Qt::UserRole, QVariant(it.value()));
    list_->addItem(item);
    ++it;
  }
  list_->setCurrentRow(0);
  OnChangeBook(list_->item(0));
  filename_ = filename;
  emit filenameChanged(QFileInfo(filename).baseName());
}

auto Core::Reset() -> void
{
  ClearItems();
  next_uid_ = 0;
  pre_uid_ = 0;
  editor_->clear();
  editor_->setReadOnly(true);
}

auto Core::SaveToFile(QWidget* win, bool is_new) -> void
{
  QString filename;
  if (!is_new) {
    filename = filename_;
  }
  if (is_new || filename == CoreValues::UNDEFINED_FNAME) {
    filename = QFileDialog::getSaveFileName(win, "Save file", "", "Memo file (*.memo);;All Files (*)");
  }

  SaveToFileInternal(filename);
}

auto Core::SaveToFileInternal(const QString& filename) -> void
{
  if (filename.isEmpty()) return;
  datapack_.clear();
  for (int i = 0, size = list_->count(); i < size; ++i) {
    auto item = list_->item(i);
    if (!item) continue;
    QString prefix = QString::number(item->type());
    datapack_.insert(prefix + ":" + item->text(), item->data(Qt::UserRole).toString());
  }

  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::information(nullptr, "Core: cannot open file!", file.errorString());
    return;
  }

  QDataStream out(&file);
  out.setVersion(QDataStream::Qt_5_10);
  out << datapack_;

  filename_ = filename;
  emit filenameChanged(QFileInfo(filename).baseName());
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

auto Core::SortItems(int order) -> void
{
  list_->sortItems(order ? Qt::DescendingOrder:
                           Qt::AscendingOrder);
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
