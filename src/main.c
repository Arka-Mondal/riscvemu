/*
  Copyright (c) 2024, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdio.h>
#include "cpu.h"
#include "bus.h"
#include "dram.h"

int main(int argc, char * argv[])
{
  struct riscv_cpu cpu1;
  struct bus bus;
  struct dram mem;

  dram_init(&mem, NULL);
  bus_init(&bus, &mem);
  riscv_cpu_init(&cpu1, &bus);

  cpu1.registers[x1] = 5;

  printf("%#lx\n", cpu1.registers[x1]);

  riscv_cpu_exec(&cpu1, 0x3e800093);
  riscv_cpu_exec(&cpu1, 0x00108133);

  printf("%#lx\n", cpu1.registers[x1]);
  printf("%#lx\n", cpu1.registers[x2]);

  riscv_cpu_deinit(&cpu1);
  bus_deinit(&bus);
  dram_deinit(&mem);

  return 0;
}
