/*
  Copyright (c) 2024, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <string.h>
#include "cpu.h"
#include "bus.h"
#include "dram.h"

// temporary
struct riscv_cpu * this_cpu = NULL;       // later will be replaced by thread-local storage

int riscv_cpu_init(struct riscv_cpu * const restrict cpu, struct bus * const bus)
{
  if (cpu == NULL || bus == NULL)
    return -1;

  this_cpu = cpu;

  memset(cpu, 0x0, sizeof(struct riscv_cpu));   // initialize the registers to 0

  cpu->x2 = DRAM_BASE + DRAM_SIZE;              // initialize the stack pointer
  cpu->pc = DRAM_BASE;                          // set the program counter to the base address

  cpu->bus = bus;                               // connect the cpu to the bus

  return 0;
}

int riscv_cpu_deinit(struct riscv_cpu * const restrict cpu)
{
  if (cpu == NULL)
    return -1;

  this_cpu = NULL;
  cpu->bus = NULL;                              // disconnects the bus from the cpu

  return 0;
}
