#ifndef UEXE_H
#define UEXE_H

#include <stdio.h>
#include <uarray.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "umemory.h"

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


#endif