/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2018 by N.T.WORKS                                       *
 *                                                                         *
 *   Licensed under GPLv3                                                  *
 *                                                                         *
 ***************************************************************************/
#ifndef GPU_H
#define GPU_H

#include "common_types.h"
#include "common_values.h"
#include "vram.h"

#include <QObject>

namespace GPU {

class Core : public QObject
{
  Q_OBJECT
public:
  explicit Core(QObject *parent = nullptr);
  // members
  Vram vram;
  // streams
  void ToReset();
  T_gpu_result ToFlushData();
  T_gpu_result ToFileTabData(T_gpu_addr, int, T_strs, T_states);
  T_gpu_result ToBookTabData(T_gpu_addr, int, T_strs, T_states);
  T_gpu_result ToPageListData(T_gpu_addr, int, T_strs, T_states);
  T_gpu_result ToEditorData(T_gpu_addr, bool, int, int, const T_str&);
  T_gpu_result ToToolBarData(T_gpu_addr);
  T_gpu_result ToStatusBarData(T_gpu_addr, const T_str&);
  T_gpu_result ToWindowData(T_gpu_addr, const T_str&);

signals:
  void ToDev(T_dev_addr, T_ivec, T_strs, T_states);
  void ToError(T_gpu_result);

public slots:
  void FromCpu(T_gpu_addr, T_ivec, T_strs);
  void FromIrq(T_gpu_irq);
};

}  // ns GPU

#endif // GPU_H
