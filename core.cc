/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "core.h"

namespace Nmemo {

/* class: Core */
Core::Core(QObject* parent): QObject(parent),
  m_tids(new T_ids()),
  m_statset(new T_statset()),
  m_bidsset(new T_bidsset()),
  m_labels(new T_strset()),
  m_memos(new T_memos()),
  m_tid(-1),
  m_text(""),
  m_bidset(new T_bidset()),
  m_editor_enabled(false),
  m_next_id(0),
  m_idpool(new QStack<T_id>())
{
  qDebug() << "Core: constructed";
}

Core::~Core()
{
  if (m_tids) {
    m_tids->clear();
    m_tids.reset();
  }
  if (m_statset) {
    m_statset->clear();
    m_statset.reset();
  }
  if (m_bidsset) {
    m_bidsset->clear();
    m_bidsset.reset();
  }
  if (m_labels) {
    m_labels->clear();
    m_labels.reset();
  }
  if (m_memos) {
    m_memos->clear();
    m_memos.reset();
  }
  if (m_bidset) {
    m_bidset->clear();
    m_bidset.reset();
  }
  if (m_idpool) {
    m_idpool->clear();
    m_idpool.reset();
  }
  qDebug() << "Core: destructed";
}

/* process: output */
void Core::OutputTabBar()
{
  emit asTabBarData(Cmd::INDEX_NAMES,
                    Tabs::Index::Fetch(this, m_tid),
                    Tabs::Names::Fetch(this),
                    Tabs::Stats::Fetch(this));
}

void Core::OutputBookList()
{
  auto tmp = Books::Names::Fetch(this, m_tid);
  emit asBookListData(Cmd::INDEX_NAMES,
                      Books::Index::Fetch(this, m_tid,
                                          Books::CurrentId::Fetch(this, m_tid)),
                      Books::Names::Fetch(this, m_tid));
}

void Core::OutputEditor()
{
  auto bid_ = Books::CurrentId::Fetch(this, m_tid);
  emit asEditorData(Cmd::MEMO, false, Memos::Memo::Fetch(this, bid_));
}

/* slots */
void Core::ToTabData(T_cmd cmd, T_tab_i tab_i, T_arg arg)
{
  Memos::Data::Update(this, Cmd::MEMO_EDIT, m_text);
  switch (cmd) {
  case Cmd::TAB_ADD:
    Tabs::Data::Update(this, cmd, tab_i, arg);
    Tabs::Status::Update(this, cmd, tab_i, arg);
    Books::Status::Update(this, Cmd::BOOK_CHANGE,
                          -1,
                          QVariant(0));
    OutputTabBar();
    OutputBookList();
    OutputEditor();
    break;
  case Cmd::TAB_DELETE:
    Tabs::Data::Update(this, cmd, tab_i, arg);
    Tabs::Status::Update(this, cmd, tab_i, arg);
    Books::Status::Update(this, Cmd::BOOK_CHANGE,
                          Books::CurrentIndex::Fetch(this),
                          QVariant(0));
    OutputTabBar();
    OutputBookList();
    OutputEditor();
    break;
  case Cmd::TAB_CHANGE:
    Tabs::Status::Update(this, cmd, tab_i, arg);
    Books::Status::Update(this, Cmd::BOOK_CHANGE,
                          Books::CurrentIndex::Fetch(this),
                          QVariant(0));
    OutputTabBar();
    OutputBookList();
    OutputEditor();
    break;
  case Cmd::TAB_MOVE:
    Tabs::Data::Update(this, cmd, tab_i, arg);
    OutputTabBar();
    break;
  case Cmd::TAB_RENAME:
    Tabs::Data::Update(this, cmd, tab_i, arg);
    OutputTabBar();
    break;
  default:
    break;
  }
}

void Core::ToBookData(T_cmd cmd, T_book_i book_i, T_arg arg)
{
  Memos::Data::Update(this, Cmd::MEMO_EDIT, m_text);
  switch (cmd) {
  case Cmd::BOOK_ADD:
    Books::Data::Update(this, cmd, book_i, arg);
    Books::Status::Update(this, cmd, book_i, arg);
    Memos::Data::Update(this, Cmd::MEMO_ADD, VALUE::DEFAULT_MEMO_TEXT);
    OutputBookList();
    OutputEditor();
    break;
  case Cmd::BOOK_DELETE:
    Books::Data::Update(this, cmd, book_i, arg);
    Books::Status::Update(this, cmd, book_i, arg);
    Memos::Data::Update(this, Cmd::MEMO_DELETE, "");
    OutputBookList();
    OutputEditor();
    break;
  case Cmd::BOOK_CHANGE:
    Books::Status::Update(this, cmd, book_i, arg);
    OutputBookList();
    OutputEditor();
    break;
  case Cmd::BOOK_MOVE:
    Books::Data::Update(this, cmd, book_i, arg);
    OutputBookList();
    break;
  case Cmd::BOOK_RENAME:
    Books::Data::Update(this, cmd, book_i, arg);
    OutputBookList();
    break;
  case Cmd::BOOK_SORT:
    Books::Data::Update(this, cmd, book_i, arg);
    OutputBookList();
    break;
  default:
    break;
  }
}

void Core::ToMemoData(T_cmd cmd, const T_text& text, T_stat stat)
{
  Q_UNUSED(cmd);
  Q_UNUSED(stat);

  if (!CoreUtl::BookVerify(this)) return;

  Memos::Cache::Merge(this, text);
}

void Core::ToFileData(T_cmd, T_arg)
{
  Files::Data::Update(this);
}

/* process: utils */
namespace CoreUtl {

auto TabVerify(const Core* c) -> bool
{
  return c->m_tid > 0;
}

auto BookVerify(const Core* c) -> bool
{
  auto cur_tid = c->m_tid;
  return cur_tid > 0 && c->m_bidset->contains(cur_tid) && c->m_bidset->value(cur_tid) > 0;
}

}  // ns CoreUtl

}  // namespace Nmemo
