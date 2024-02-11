/*
  Copyright (c) 2024, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include "cpu.h"
#include "bus.h"
#include "dram.h"

int bus_init(struct bus * const restrict bus, struct dram * const dram)
{
  if (bus == NULL || dram == NULL)
    return -1;

  bus->dram = dram;

  return 0;
}

uint64_t bus_load(const struct bus * const restrict bus, uint64_t addr, uint64_t size)
{
  extern struct riscv_cpu * this_cpu;

  if (bus == NULL)
  {
    this_cpu->panic = 0x1;
    return (uint64_t) -1;
  }

  return dram_load(bus->dram, addr, size);
}

int bus_store(struct bus * const restrict bus, uint64_t addr, uint64_t size, uint64_t value)
{
  if (bus == NULL)
    return -1;

  return dram_store(bus->dram, addr, size, value);
}

int bus_deinit(struct bus * const restrict bus)
{
  if (bus == NULL)
    return -1;

  bus->dram = NULL;

  return 0;
}
