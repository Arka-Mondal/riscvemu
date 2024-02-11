/*
  Copyright (c) 2024, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef _RISCVEMU_BUS_H
#define _RISCVEMU_BUS_H

#include <stddef.h>
#include <stdint.h>

struct bus {
  struct dram * dram;
};

int bus_init(struct bus * const restrict, struct dram * const);
int bus_deinit(struct bus * const restrict);
uint64_t bus_load(const struct bus * const restrict, uint64_t, uint64_t);
int bus_store(struct bus * const restrict, uint64_t, uint64_t, uint64_t);

#endif /* _RISCVEMU_BUS_H */
