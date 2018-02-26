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
  m_tid_(-1),
  m_bid_(-1),
  m_book_i_(-1),
  m_books_(new T_idpack()),
  m_labels_(new T_labels()),
  m_memos_(new T_labels()),
  m_paths_(new T_labels()),
  m_tabs_(new T_ids())
{
  qDebug() << "Core: constructed";
}

Core::~Core()
{
  if (m_books_) {
    m_books_->clear();
    m_books_.reset();
  }
  if (m_labels_) {
    m_labels_->clear();
    m_labels_.reset();
  }
  if (m_memos_) {
    m_memos_->clear();
    m_memos_.reset();
  }
  if (m_paths_) {
    m_paths_->clear();
    m_paths_.reset();
  }
  if (m_tabs_) {
    m_tabs_->clear();
    m_tabs_.reset();
  }
  qDebug() << "Core: destructed";
}

/* slots */
void Core::Update(CmdSig cmd, int index, QVariant arg, const QString &)
{
  auto tid_r = Utl::GetTabIdToRead()(cmd, m_tabs_.data(), index, m_tid_);
  if (tid_r < 0) return;

  auto tid_w = Utl::GetTabIdToWrite()(cmd, m_tabs_.data(), index, tid_r);
  auto tname = Utl::GetTabNameToWrite()(cmd, arg);
  auto bid_r = Utl::GetBookIdToRead()(cmd, m_books_.data(), tid_r, index, m_book_i_);
  auto bid_w = Utl::GetBookIdToWrite()(cmd, bid_r);
  auto bname = Utl::GetBookNameToWrite()(cmd, arg);

  auto tab_res = Utl::OperateTabData()(cmd, m_tabs_.data(), m_paths_.data(),
                                       tid_r, tid_w, index, tname, arg);
  auto book_res = Utl::OperateBookData()(cmd, m_books_.data(), m_labels_.data(),
                                         tid_r, tid_w, bid_r, bid_w, index, bname, arg);
  qDebug() << "tid:" << tid_r << "|" << tid_w;
  qDebug() << "tname:" << tname;
  qDebug() << "bid:" << bid_r << "|" << bid_w;
  qDebug() << "bname:" << bname;
  qDebug() << "tab:: tab_i: " << tab_res.at(0).toInt();
  qDebug() << "tab:: tnames: " << tab_res.at(1).toStringList().count();
  qDebug() << "book:: book_i: " << book_res.at(0).toInt();
  qDebug() << "book:: bnames: " << book_res.at(1).toStringList().count();
  qDebug() << "book:: bremoves: " << (book_res.count() - 2);
}

}  // namespace Nmemo
