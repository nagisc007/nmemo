/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef INPUT_DEVICE_H
#define INPUT_DEVICE_H

#include "../../common_types.h"
#include "../../common_values.h"

#include <QObject>

namespace DEV {

class Input : public QObject
{
  Q_OBJECT
public:
  explicit Input(QObject *parent = nullptr);
  // test methods
  void Reset();
  void NewFile();
  void OpenFile(T_str);
  void CloseFile(T_index);
  void ChangeFile(T_index);
  void MoveFile(T_index, T_index);
  void RenameFile(T_index, T_str);
  void SaveFile(T_index);
  void SaveAsFile(T_index, T_str);
  void AddBook(T_str);
  void DeleteBook(T_index);
  void ChangeBook(T_index);
  void MoveBook(T_index, T_index);
  void RenameBook(T_index, T_str);
  void AddPage(T_str);
  void DeletePage(T_index);
  void ChangePage(T_index);
  void MovePage(T_index, T_index);
  void RenamePage(T_index, T_str);
  void ModifyText(T_str);

signals:
  void ToCpu(T_cpu_addr, int, T_str);
  void ToCpuIrq(T_cpu_irq);
};

}  // ns DEV

#endif // INPUT_DEVICE_H
