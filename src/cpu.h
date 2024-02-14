/*
  Copyright (c) 2024, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef _RISCVEMU_CPU_H
#define _RISCVEMU_CPU_H

#include <stddef.h>
#include <stdint.h>

enum register_names {
  x0,   x1,  x2,  x3,  x4,  x5,  x6,  x7,  x8,  x9, x10, x11, x12, x13, x14, x15,
  x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30, x31
};

struct riscv_cpu {

  // 32 general purpose registers
  uint64_t registers[32];

  // program counter
  uint64_t pc;

  // bust connector
  struct bus * bus;

  // cpu state
  uint8_t panic;
};

int riscv_cpu_init(struct riscv_cpu * const restrict, struct bus * const);
uint32_t riscv_cpu_fetch(const struct riscv_cpu * const restrict);
int riscv_cpu_exec(struct riscv_cpu * const restrict, uint32_t);
int riscv_cpu_deinit(struct riscv_cpu * const restrict);

uint64_t riscv_inst_rd(uint32_t);
uint64_t riscv_inst_rs1(uint32_t);
uint64_t riscv_inst_rs2(uint32_t);
uint64_t riscv_insti_imm(uint32_t);
uint64_t riscv_insts_imm(uint32_t);
uint64_t riscv_instb_imm(uint32_t);
uint64_t riscv_instu_imm(uint32_t);
uint64_t riscv_instj_imm(uint32_t);

int riscv_cpu_insti_exec(struct riscv_cpu * const restrict, uint32_t);
int riscv_cpu_instr_exec(struct riscv_cpu * const restrict, uint32_t);

int riscv_cpu_addi_exec(struct riscv_cpu * const restrict, uint32_t);
int riscv_cpu_add_exec(struct riscv_cpu * const restrict, uint32_t);

#endif /* _RISCVEMU_CPU_H */
