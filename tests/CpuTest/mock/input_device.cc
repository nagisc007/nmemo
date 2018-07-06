/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#include "input_device.h"

namespace DEV {

// class: DEV::Input
Input::Input(QObject *parent) : QObject(parent)
{

}

// test methods
void Input::Reset()
{
  emit ToCpuIrq(CPU::Irq::RESET);
}

void Input::AddBook(T_str s)
{
  emit ToCpu(CPU::Addr::BOOK_ADD, 0, s);
}

void Input::AddPage(T_str s)
{
  emit ToCpu(CPU::Addr::PAGE_ADD, 0, s);
}

void Input::ChangeBook(T_index i)
{
  emit ToCpu(CPU::Addr::BOOK_CHANGE, i ,T_str());
}

void Input::ChangeFile(T_index i)
{
  emit ToCpu(CPU::Addr::FILE_CHANGE, i, T_str());
}

void Input::ChangePage(T_index i)
{
  emit ToCpu(CPU::Addr::PAGE_CHANGE, i ,T_str());
}

void Input::CloseFile(T_index i)
{
  emit ToCpu(CPU::Addr::FILE_CLOSE, i, T_str());
}

void Input::DeleteBook(T_index i)
{
  emit ToCpu(CPU::Addr::BOOK_DELETE, i, T_str());
}

void Input::DeletePage(T_index i)
{
  emit ToCpu(CPU::Addr::PAGE_DELETE, i, T_str());
}

void Input::ModifyText()
{
  emit ToCpu(CPU::Addr::TEXT_MODIFY, 0, T_str());
}

void Input::MoveBook(T_index from, T_index to)
{
  emit ToCpu(CPU::Addr::BOOK_MOVE, (from << 8) | to, T_str());
}

void Input::MoveFile(T_index from, T_index to)
{
  emit ToCpu(CPU::Addr::FILE_MOVE, (from << 8) | to, T_str());
}

void Input::MovePage(T_index from, T_index to)
{
  emit ToCpu(CPU::Addr::PAGE_MOVE, (from << 8) | to, T_str());
}

void Input::NewFile(T_str s)
{
  emit ToCpu(CPU::Addr::FILE_NEW, 0, s);
}

void Input::OpenFile(T_str s)
{
  emit ToCpu(CPU::Addr::FILE_OPEN, 0, s);
}

void Input::RenameBook(T_index i, T_str s)
{
  emit ToCpu(CPU::Addr::BOOK_RENAME, i, s);
}

void Input::RenameFile(T_index i, T_str s)
{
  emit ToCpu(CPU::Addr::FILE_RENAME, i, s);
}

void Input::RenamePage(T_index i, T_str s)
{
  emit ToCpu(CPU::Addr::PAGE_RENAME, i, s);
}

void Input::SaveFile(T_index i)
{
  emit ToCpu(CPU::Addr::FILE_SAVE, i, T_str());
}

void Input::SaveAsFile(T_index i, T_str path)
{
  emit ToCpu(CPU::Addr::FILE_SAVEAS, i, path);
}

void Input::UpdateText(T_str text)
{
  emit ToCpu(CPU::Addr::TEXT_UPDATE, 0, text);
}

}  // ns DEV
