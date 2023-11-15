#ifndef UINTER_H
#define UINTER_H


#include <assert.h>
#include <stdio.h>
#include <stdint.h>


/*Getopcode:
retrieves the opcode 
Args: uint32_t instruction
Return: the opcode as int
Expects the opcode to be in range [0,13]
Note: CRE if out of range
setReg:
Takes a non-loadval instruction and set the corresponding registers 
Args: the instruction as uint 32, register pointers (uint32 pointers) by reference
Return: none
Expects: instruction has valid opcode [0,12]
Note: CRE if out of range
setLoad: 
Takes a loadval instruction and set the corresponding register and values
Args: the instruction as uint 32, register pointer (uint32 pointers) by reference
Return: uint32 value
Expects: instruction has valid opcode 13, register pointer not null
Note: CRE if out of range or register null
*/
int Getopcode(uint32_t inst);
void setRef(uint32_t inst, uint32_t *ra,uint32_t *rb,uint32_t *rc);
uint32_t setLoad(uint32_t inst, uint32_t *ra);

#endif