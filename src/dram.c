/*
  Copyright (c) 2024, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include "cpu.h"
#include "dram.h"

static uint64_t dram_load8(const struct dram * const restrict dram, uint64_t addr)
{
  return (uint64_t) dram->mem[addr - DRAM_BASE];
}

static uint64_t dram_load16(const struct dram * const restrict dram, uint64_t addr)
{
  return (uint64_t) dram->mem[addr - DRAM_BASE]
      |  (uint64_t) dram->mem[addr - DRAM_BASE + 1] << 8;
}

static uint64_t dram_load32(const struct dram * const restrict dram, uint64_t addr)
{
  return (uint64_t) dram->mem[addr - DRAM_BASE]
      |  (uint64_t) dram->mem[addr - DRAM_BASE + 1] << 8
      |  (uint64_t) dram->mem[addr - DRAM_BASE + 2] << 16
      |  (uint64_t) dram->mem[addr - DRAM_BASE + 3] << 24;
}

static uint64_t dram_load64(const struct dram * const restrict dram, uint64_t addr)
{
  return (uint64_t) dram->mem[addr - DRAM_BASE]
      |  (uint64_t) dram->mem[addr - DRAM_BASE + 1] << 8
      |  (uint64_t) dram->mem[addr - DRAM_BASE + 2] << 16
      |  (uint64_t) dram->mem[addr - DRAM_BASE + 3] << 24
      |  (uint64_t) dram->mem[addr - DRAM_BASE + 4] << 32
      |  (uint64_t) dram->mem[addr - DRAM_BASE + 5] << 40
      |  (uint64_t) dram->mem[addr - DRAM_BASE + 6] << 48
      |  (uint64_t) dram->mem[addr - DRAM_BASE + 7] << 56;
}

static void dram_store8(struct dram * const restrict dram, uint64_t addr, uint64_t value)
{
  dram->mem[addr - DRAM_BASE] = value & 0xff;
}

static void dram_store16(struct dram * const restrict dram, uint64_t addr, uint64_t value)
{
  dram->mem[addr - DRAM_BASE] = value & 0xff;
  dram->mem[addr - DRAM_BASE + 1] = (value >> 16) & 0xff;
}

static void dram_store32(struct dram * const restrict dram, uint64_t addr, uint64_t value)
{
  dram->mem[addr - DRAM_BASE] = value & 0xff;
  dram->mem[addr - DRAM_BASE + 1] = (value >> 8) & 0xff;
  dram->mem[addr - DRAM_BASE + 2] = (value >> 16) & 0xff;
  dram->mem[addr - DRAM_BASE + 3] = (value >> 24) & 0xff;
}

static void dram_store64(struct dram * const restrict dram, uint64_t addr, uint64_t value)
{
  dram->mem[addr - DRAM_BASE] = value & 0xff;
  dram->mem[addr - DRAM_BASE + 1] = (value >> 8) & 0xff;
  dram->mem[addr - DRAM_BASE + 2] = (value >> 16) & 0xff;
  dram->mem[addr - DRAM_BASE + 3] = (value >> 24) & 0xff;
  dram->mem[addr - DRAM_BASE + 4] = (value >> 32) & 0xff;
  dram->mem[addr - DRAM_BASE + 5] = (value >> 40) & 0xff;
  dram->mem[addr - DRAM_BASE + 6] = (value >> 48) & 0xff;
  dram->mem[addr - DRAM_BASE + 7] = (value >> 56) & 0xff;
}

int dram_init(struct dram * const restrict dram, void * mem_addr)
{
  void * mem;

  if (dram == NULL)
    return -1;

  if (mem_addr == NULL)
  {
    mem = malloc(DRAM_SIZE);
    if (mem == NULL)
      return -1;

    dram->flags = 0x1;
  }
  else
  {
    mem = mem_addr;
  }

  dram->mem = mem;

  return 0;
}

uint64_t dram_load(const struct dram * const restrict dram, uint64_t addr, uint64_t size)
{
  uint64_t data;
  extern struct riscv_cpu * this_cpu;

  if ((dram == NULL) || (dram->mem == NULL) || (addr < DRAM_BASE)
      || ((8 * (DRAM_SIZE + DRAM_BASE - addr)) < size))        // DRAM_SIZE - (addr - DRAM_BASE)
  {
    this_cpu->panic = 0x1;
    return (uint64_t) -1;
  }

  switch (size)
  {
    case 8:
      data = dram_load8(dram, addr);
      break;
    case 16:
      data = dram_load16(dram, addr);
      break;
    case 32:
      data = dram_load32(dram, addr);
      break;
    case 64:
      data = dram_load64(dram, addr);
      break;
    default:
      this_cpu->panic = 0x1;
      data = (uint64_t) -1;
  }

  return data;
}

int dram_store(struct dram * const restrict dram, uint64_t addr,
                uint64_t size, uint64_t value)
{
  int status;

  if ((dram == NULL) || (dram->mem == NULL) || (addr < DRAM_BASE)
      || ((8 * (DRAM_SIZE + DRAM_BASE - addr)) < size))        // DRAM_SIZE - (addr - DRAM_BASE)
    return -1;

  status = 0;

  switch (size)
  {
    case 8:
      dram_store8(dram, addr, value);
      break;
    case 16:
      dram_store16(dram, addr, value);
      break;
    case 32:
      dram_store32(dram, addr, value);
      break;
    case 64:
      dram_store64(dram, addr, value);
      break;
    default:
      status = -1;
      break;
  }

  return status;
}

int dram_deinit(struct dram * const restrict dram)
{
  if (dram == NULL)
    return -1;

  if (dram->flags & 0x1)
    free(dram->mem);

  dram->mem = NULL;
  dram->flags = 0;

  return 0;
}
