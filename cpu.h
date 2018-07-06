/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef CPU_H
#define CPU_H

#include "common_types.h"
#include "common_values.h"
#include "ram.h"

#include <QObject>

namespace CPU {

using T_submits = int;

class Core : public QObject
{
  Q_OBJECT
public:
  explicit Core(QObject *parent = nullptr);
  // members
  Ram ram;
  // streams
  void ToReset();
  T_cpu_result ToProcess(T_cpu_addr, int, const T_str&);
  T_cpu_result ToGpuData(T_cpu_addr);
  T_cpu_result ToGpuDataOfWindow(T_submits);
  T_cpu_result ToGpuDataOfFileTab(T_submits);
  T_cpu_result ToGpuDataOfBookTab(T_submits);
  T_cpu_result ToGpuDataOfPageList(T_submits);
  T_cpu_result ToGpuDataOfEditor(T_submits);
  T_cpu_result ToGpuDataOfStatusBar(T_submits, T_cpu_addr);
  T_cpu_result ToGpuDataOfToolBar(T_submits);
  T_cpu_result ToCreateFile(const T_str&);
  T_cpu_result ToChangeFile(T_index);
  T_cpu_result ToCloseFile(T_index);
  T_cpu_result ToOpenFile(const T_str&);
  T_cpu_result ToMoveFile(T_index, T_index);
  T_cpu_result ToRenameFile(T_index, const T_str&);
  T_cpu_result ToSaveFile(T_index, const T_str&);
  T_cpu_result ToAddBook(T_id, const T_str&);
  T_cpu_result ToChangeBook(T_id, T_index);
  T_cpu_result ToDeleteBook(T_id, T_index);
  T_cpu_result ToMoveBook(T_id, T_index, T_index);
  T_cpu_result ToRenameBook(T_id, T_index ,const T_str&);
  T_cpu_result ToSortBooks(T_id, T_order);
  T_cpu_result ToAddPage(T_id, T_id, const T_str&, const T_str&);
  T_cpu_result ToChangePage(T_id, T_id, T_index);
  T_cpu_result ToDeletePage(T_id, T_id, T_index);
  T_cpu_result ToMovePage(T_id, T_id, T_index, T_index);
  T_cpu_result ToRenamePage(T_id, T_id, T_index, const T_str&);
  T_cpu_result ToSortPages(T_id, T_id, T_order);
  T_cpu_result ToModifyText(T_id, T_id, T_id, const T_str&);

signals:
  void ToError(T_cpu_result);
  void ToGpu(T_gpu_addr, T_ivec i = T_ivec(), T_strs s = T_strs());
  void ToGpuIrq(T_gpu_irq);
  void RequestFileName();

public slots:
  void FromDev(T_cpu_addr, int, const T_str&);
  void FromIrq(T_cpu_irq);
};

}  // ns CPU

#endif // CPU_H
