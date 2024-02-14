/*
  Copyright (c) 2024, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef _RISCVEMU_UTIL_H
#define _RISCVEMU_UTIL_H

#ifdef __GNUC__
#define NORETURN __attribute__ ((__noreturn__))
#else
#define NORETURN
#endif

void hart_panic(const char * restrict format, ...) NORETURN ;

#endif
