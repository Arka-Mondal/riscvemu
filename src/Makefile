# Makefile for the riscemu
#
#  Copyright (c) 2024, Arka Mondal. All rights reserved.
#  Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

CC := gcc
CFLAGS := -Wall -Wextra
OBJECTS = cpu.o bus.o dram.o util.o main.o

riscv : $(OBJECTS)
	$(CC) -o $@ $^

main.o : main.c
	$(CC) -o $@ -c $< $(CFLAGS)

cpu.o : cpu.c cpu.h
	$(CC) -o $@ -c $< $(CFLAGS)

bus.o : bus.c bus.h
	$(CC) -o $@ -c $< $(CFLAGS)

dram.o : dram.c dram.h
	$(CC) -o $@ -c $< $(CFLAGS)

util.o : util.c util.h
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY : clean
clean :
	rm -vf $(OBJECTS) riscv
