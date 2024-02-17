/*
  Copyright (c) 2024, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <assert.h>
#include <string.h>
#include "cpu.h"
#include "bus.h"
#include "dram.h"
#include "util.h"

// temporary
struct riscv_cpu * this_cpu = NULL;       // later will be replaced by thread-local storage

static uint64_t riscv_cpu_load(const struct riscv_cpu * const restrict cpu,
                                uint64_t addr, uint64_t size)
{
  if (cpu == NULL)
  {
    this_cpu->panic = 0x1;
    return (uint64_t) -1;
  }

  return (uint64_t) bus_load(cpu->bus, addr, size);
}

static int riscv_cpu_store(struct riscv_cpu * const restrict cpu,
                            uint64_t addr, uint64_t size, uint64_t value)
{
  if (cpu == NULL)
    return -1;

  return bus_store(cpu->bus, addr, size, value);
}

int riscv_cpu_init(struct riscv_cpu * const restrict cpu, struct bus * const bus)
{
  if (cpu == NULL || bus == NULL)
    return -1;

  this_cpu = cpu;

  memset(cpu, 0x0, sizeof(struct riscv_cpu));   // initialize the registers to 0

  cpu->registers[x2] = DRAM_BASE + DRAM_SIZE;   // initialize the stack pointer
  cpu->pc = DRAM_BASE;                          // set the program counter to the base address

  cpu->bus = bus;                               // connect the cpu to the bus

  return 0;
}

uint32_t riscv_cpu_fetch(const struct riscv_cpu * const restrict cpu)
{
  if (cpu == NULL)
  {
    this_cpu->panic = 0x1;
    return (uint32_t) -1;
  }

  return (uint32_t) bus_load(cpu->bus, cpu->pc, 32);
}

int riscv_cpu_exec(struct riscv_cpu * const restrict cpu, uint32_t inst)
{
  unsigned int opcode;

  if (cpu == NULL)
    return -1;

  cpu->registers[x0] = 0;

  opcode = inst & 0x7f;

  switch (opcode)
  {
    case 0x13:
    case 0x1b:
      riscv_cpu_insti_exec(cpu, inst);
      break;
    case 0x33:
      riscv_cpu_instr_exec(cpu, inst);
      break;
    default:
      hart_panic("not implemented! %#04x\n", opcode);
  }

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


/*
 * Functions to decode a given instruction
 */

// rd is the destination register
uint64_t riscv_inst_rd(uint32_t inst)
{
  return (inst >> 7) & 0x1f;
}

// rs1 is one the source registers
uint64_t riscv_inst_rs1(uint32_t inst)
{
  return (inst >> 15) & 0x1f;
}

// rs2 is the other source register
uint64_t riscv_inst_rs2(uint32_t inst)
{
  return (inst >> 20) & 0x1f;
}

// functions to extract immediate value from different types of instructions

// I-type instruction
uint64_t riscv_insti_imm(uint32_t inst)
{
  return (uint64_t) (((int64_t) (int32_t) inst) >> 20);
}

// S-type instruction
uint64_t riscv_insts_imm(uint32_t inst)
{
  return ((uint64_t) (((int64_t) (int32_t) (inst & 0xfe000000)) >> 20))
          | ((inst >> 7) & 0x1f);
}

// B-type instruction
uint64_t riscv_instb_imm(uint32_t inst)
{
  return (((int64_t) (int32_t) (inst & 0x80000000)) >> 19)
          | ((inst << 4) & 0x800)
          | ((inst >> 20) & 0x7e0)
          | ((inst >> 7) & 0x1e);
}

// U-type instruction
uint64_t riscv_instu_imm(uint32_t inst)
{
  return (int64_t) (int32_t) (inst & 0xfffff000);
}

// J-type instruction
uint64_t riscv_instj_imm(uint32_t inst)
{
  return (((int64_t) (int32_t) (inst & 0x80000000)) >> 11)
          | ((inst >> 20) & 0x7fe)
          | ((inst >> 9) & 0x800)
          | (inst & 0xff000);
}

// execute I-type instructions
int riscv_cpu_insti_exec(struct riscv_cpu * const restrict cpu, uint32_t inst)
{
  uint32_t opcode = inst & 0x7f;
  uint32_t rd = riscv_inst_rd(inst);
  uint32_t funct3 = (inst >> 12) & 0x7;
  uint32_t rs1 = riscv_inst_rs1(inst);
  uint64_t imm = riscv_insti_imm(inst);

  switch (opcode)
  {
    case 0x13:
      switch (funct3) {
        case 0x0: // ADDI
          cpu->registers[rd] = cpu->registers[rs1] + imm;
          break;

        case 0x2: // SLTI
          cpu->registers[rd] = (int64_t) cpu->registers[rs1] < (int64_t) imm;
          break;

        case 0x3: // SLTIU
          cpu->registers[rd] = cpu->registers[rs1] < imm;
          break;

        case 0x4: // XORI
          cpu->registers[rd] = cpu->registers[rs1] ^ imm;
          break;

        case 0x6: // ORI
          cpu->registers[rd] = cpu->registers[rs1] | imm;
          break;

        case 0x7: // ANDI
          cpu->registers[rd] = cpu->registers[rs1] & imm;
          break;

        case 0x1: { // SLLI
          uint64_t shift = imm & 0x3f;
          assert((imm >> 6) == 0);
          cpu->registers[rd] = cpu->registers[rs1] << shift;
          break;
        }

        case 0x5: {
          uint64_t shift = imm & 0x3f;
          switch (imm >> 6) {
            case 0x00: // SRLI
              cpu->registers[rd] = cpu->registers[rs1] >> shift;
              break;

            case 0x20: // SRAI
              cpu->registers[rd] = (uint64_t) ((int64_t) cpu->registers[rs1] >> (int64_t) shift);
              break;

            default:
              hart_panic("not implemented! 0b0010011(%#03x)\n", funct3);
          }
          break;
        }

        default:
          hart_panic("not implemented! 0b0010011(%#03x)\n", funct3);
      }
      break;

    case 0x1b:
      switch (funct3) {
        case 0x0: // ADDIW
          cpu->registers[rd] = (int64_t) (int32_t) ((cpu->registers[rs1] + imm) & 0xffffffff);
          break;

        case 0x1: { // SLLIW
          uint32_t shift = imm & 0x3f;
          assert(~shift >> 5 & 1); // Assert if the shamt[5] is 0.
          cpu->registers[rd] = (int64_t) (int32_t) ((uint32_t) cpu->registers[rs1] << shift);
          break;
        }

        case 0x5: {
          uint32_t shift = imm & 0x3f;
          assert(~shift >> 5 & 1); // Assert if the shamt[5] is 0.

          switch (imm >> 6) {
            case 0x00: // SRLIW
              cpu->registers[rd] = (int64_t) (int32_t) ((uint32_t) cpu->registers[rs1] >> shift);
              break;

            case 0x20: // SRAIW
              cpu->registers[rd] = (int64_t) (int32_t) ((int32_t) (uint32_t) cpu->registers[rs1] >> shift);
              break;

            default:
              hart_panic("not implemented! 0b0011011(%#03x)\n", funct3);
          }
          break;
        }

        default:
          hart_panic("not implemented! 0b0011011(%#03x)\n", funct3);
      }
      break;

    default:
      hart_panic("not implemented! %#04x\n", opcode);
  }

  return 0;
}

// execute R-type instructions
int riscv_cpu_instr_exec(struct riscv_cpu * const restrict cpu, uint32_t inst)
{
  unsigned int funct3, funct7;

  funct3 = (inst >> 12) & 0x7;
  funct7 = (inst >> 25) & 0x7f;

  switch (funct3)
  {
    case 0x0:
      (funct7 == 0x0) ? riscv_cpu_add_exec(cpu, inst)
      : hart_panic("not implemented! 0x0110011(%#x:%#x)\n", funct3,funct7);
      break;
    default:
      hart_panic("not implemented! 0x0110011(%#x)\n", funct3);
  }

  return 0;
}

// execute instruction - ADDI
int riscv_cpu_addi_exec(struct riscv_cpu * const restrict cpu, uint32_t inst)
{
  uint64_t rd, rs1;

  rd = riscv_inst_rd(inst);
  rs1 = riscv_inst_rs1(inst);
  cpu->registers[rd] = (int64_t) cpu->registers[rs1] + (int64_t) riscv_insti_imm(inst);

  return 0;
}

// execute instruction - ADD
int riscv_cpu_add_exec(struct riscv_cpu * const restrict cpu, uint32_t inst)
{
  uint64_t rd, rs1, rs2;

  rd = riscv_inst_rd(inst);
  rs1 = riscv_inst_rs1(inst);
  rs2 = riscv_inst_rs2(inst);

  cpu->registers[rd] = (int64_t) cpu->registers[rs1] + (int64_t) cpu->registers[rs2];

  return 0;
}
