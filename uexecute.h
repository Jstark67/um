#ifndef UEXE_H
#define UEXE_H

#include <stdio.h>
#include <uarray.h>
#include <assert.h>
#include <stdint.h>

void move(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void segL(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void segS(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void add(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void mult(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void div(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void nand(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void halt(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void map(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void unmap(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void out(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void in(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void loadP(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void LV(uint32_t *ra, uint32_t *rb, uint32_t *rc);


#endif