/**************************************************************
 *
 *                     uinterprate.h
 *
 *     Assignment: HW6 um
 *     Authors:  David Chen and Sam Hu
 *     Date:  Nov 20th
 *
 *     summary:
 *      
 *     uinterprate.h contains the interace of opcode
 *     preprocess and interpretation operations functions. These
 *     functions serve as a prelinimary step to operation execution
 *
 **************************************************************/
#ifndef UINTER_H
#define UINTER_H


#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include "bitpack.h"



uint32_t getOpcode(uint32_t inst);
void setRef(uint32_t inst, uint32_t *ra,uint32_t *rb,uint32_t *rc);
uint32_t setLoad(uint32_t inst, uint32_t *ra);

#endif