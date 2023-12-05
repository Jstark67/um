/**************************************************************
 *
 *                     uexecute.h
 *
 *     Assignment: HW6 um
 *     Authors:  David Chen and Sam Hu
 *     Date:  Nov 20th
 *
 *     summary:
 *     uexecute.h provides the interface of all the execution 
 *     operations. It also defines the structure of the machine
 *     and its registers
 *
 **************************************************************/
#ifndef UEXE_H
#define UEXE_H

#include <stdio.h>
#include <uarray.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "umemory.h"
#include "uinterprate.h"

#define CMD_SIZE 4
#define NUM_REG 8

/* structure that defines the machine */
typedef struct Machine {
        /* memory structure */
        Mem_T mem;
        /* 8 32 bit registers */
        uint32_t reg[NUM_REG];
        /* program counter */
        uint32_t program_counter;
} *Machine;

/* opcode for all executions */
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

/* Execution Commands */
void move(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void segL(Mem_T mem, uint32_t *ra, uint32_t *rb, uint32_t *rc);
void segS(Mem_T mem, uint32_t *ra, uint32_t *rb, uint32_t *rc);
void add(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void mult(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void divide(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void nand(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void halt(Mem_T *mem);
void map(Mem_T mem, uint32_t *rb, uint32_t *rc);
void unmap(Mem_T mem, uint32_t *rc);
void out(uint32_t *rc);
void in(uint32_t *rc);
int loadP(Mem_T mem, uint32_t *rb);
void lv(uint32_t value, uint32_t *rc);

/* Halt Helper function */
void halt_exit(Machine *mach);

#endif