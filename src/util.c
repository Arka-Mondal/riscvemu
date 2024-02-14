/*
  Copyright (c) 2024, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void hart_panic(const char * restrict format, ...)
{
  va_list arglist;

  va_start(arglist, format);
  vfprintf(stderr, format, arglist);
  va_end(arglist);

  exit(EXIT_FAILURE);
}
