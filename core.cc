/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "core.h"

namespace Core {

/* class: System */
System::System(QObject* parent): QObject(parent),
  // memory
  m_bids(new T_ids()),
  m_pathset(new T_strset()),
  m_pidsset(new T_pidsset()),
  m_nameset(new T_strset()),
  m_noteset(new T_strset()),
  // register
  r_bid(-1),
  r_note(""),
  r_pidset(new T_pidset()),
  // utils
  u_nextid(0),
  u_idpool(new QStack<T_id>())
{
  qDebug() << "System: constructed";
}

System::~System()
{
  if (m_bids) {
    m_bids->clear();
    m_bids.reset();
  }
  if (m_pathset) {
    m_pathset->clear();
    m_pathset.reset();
  }
  if (m_pidsset) {
    m_pidsset->clear();
    m_pidsset.reset();
  }
  if (m_nameset) {
    m_nameset->clear();
    m_nameset.reset();
  }
  if (m_noteset) {
    m_noteset->clear();
    m_noteset.reset();
  }
  if (r_pidset) {
    r_pidset->clear();
    r_pidset.reset();
  }
  if (u_idpool) {
    u_idpool->clear();
    u_idpool.reset();
  }
  qDebug() << "System: destructed";
}

/* process: output */
void System::OutToTabBar()
{
  emit asTabBarData(Cmd::TAB_ALL,
                    QVariant(0),
                    QVariant(0),
                    QVariant(0));
}

void System::OutToPageList()
{
  emit asPageListData(Cmd::LIST_ALL,
                      QVariant(0),
                      QVariant(0));
}

void System::OutToEditor()
{
  emit asEditorData(Cmd::EDITOR_TEXT, QVariant(0), QVariant(0));
}

void System::OutToStatusBar()
{

}

void System::OutToTitleBar()
{

}

/* slots */
void System::ToSystemData(T_cmd cmd, T_arg arg0, T_arg arg1, T_arg arg2)
{
  Q_UNUSED(cmd);
  Q_UNUSED(arg0);
  Q_UNUSED(arg1);
  Q_UNUSED(arg2);
}

/* process: Core Process */
namespace CP {

/* CP: Book */
namespace Book {

}  // ns CP::Book

/* CP: Page */
namespace Page {

}  // ns CP::Page

/* CP: Note */
namespace Note {

}  // ns CP::Note
}  // ns CP

}  // namespace Nmemo
