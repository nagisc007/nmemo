/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "memowidget.h"

namespace Nmemo {

/* class: MemoWidget */
MemoWidget::MemoWidget(QWidget *parent) : QWidget(parent),
  booklist(new QListWidget(parent)),
  editor(new QTextEdit(parent)),
  tabbar(new QTabBar(parent))
{
  // props
  tabbar->setMovable(true);
  tabbar->setTabsClosable(true);
  booklist->setMaximumWidth(Props::BOOKLIST_MAX_WIDTH);
  editor->setReadOnly(true);
  editor->setTabStopDistance(Props::EDIT_TAB_DISTANCE);
  // layout
  auto mainLayout = new QVBoxLayout(this);
  auto subLayout = new QHBoxLayout(this);
  setLayout(mainLayout);
  mainLayout->addWidget(tabbar.data());
  mainLayout->addLayout(subLayout);
  subLayout->addWidget(editor.data());
  subLayout->addWidget(booklist.data());
  qDebug() << "MemoWidget: constructed";
}

MemoWidget::~MemoWidget()
{
  if (booklist) {
    booklist->clear();
    booklist.reset();
  }
  if (editor) editor.reset();
  if (tabbar) tabbar.reset();
  qDebug() << "MemoWidget: destructed";
}

}  // namespace Nmemo
