/*
  Copyright (c) 2024, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef _RISCVEMU_CPU_H
#define _RISCVEMU_CPU_H

#include <stddef.h>
#include <stdint.h>

struct riscv_cpu {

  // 32 general purpose registers
  const uint64_t x0;
  uint64_t x1;
  uint64_t x2;
  uint64_t x3;
  uint64_t x4;
  uint64_t x5;
  uint64_t x6;
  uint64_t x7;
  uint64_t x8;
  uint64_t x9;
  uint64_t x10;
  uint64_t x11;
  uint64_t x12;
  uint64_t x13;
  uint64_t x14;
  uint64_t x15;
  uint64_t x16;
  uint64_t x17;
  uint64_t x18;
  uint64_t x19;
  uint64_t x20;
  uint64_t x21;
  uint64_t x22;
  uint64_t x23;
  uint64_t x24;
  uint64_t x25;
  uint64_t x26;
  uint64_t x27;
  uint64_t x28;
  uint64_t x29;
  uint64_t x30;
  uint64_t x31;

  // program counter
  uint64_t pc;

  // bust connector
  struct bus * bus;

  // cpu state
  uint8_t panic;
};

int riscv_cpu_init(struct riscv_cpu * const restrict, struct bus * const);
int riscv_cpu_deinit(struct riscv_cpu * const restrict);

#endif /* _RISCVEMU_CPU_H */
