/*
  Copyright (c) 2024, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef _RISCVEMU_DRAM_H
#define _RISCVEMU_DRAM_H

#include <stddef.h>
#include <stdint.h>

#define DRAM_SIZE 1048576
#define DRAM_BASE 0x80000000

struct dram {
  uint8_t * mem;
  uint8_t flags;
};

int dram_init(struct dram * const restrict, void *);
int dram_deinit(struct dram * const restrict);
uint64_t dram_load(const struct dram * const restrict, uint64_t, uint64_t);
int dram_store(struct dram * const restrict, uint64_t, uint64_t, uint64_t);

#endif /* _RISCVEMU_DRAM_H */
