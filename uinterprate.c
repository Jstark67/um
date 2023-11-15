#include "uinterprate.h"

const int OP_MASK = 0xF0000000;
const int RA_MASK = 0x000001c0;
const int RB_MASK = 0x00000038;
const int RC_MASK = 0x00000007;
const int LV_MASK = 0x0d000000;
const int VAL_MASK = 0x01ffffff;

/*Getopcode:
retrieves the opcode 
Args: uint32_t instruction
Return: the opcode as int
Expects the opcode to be in range [0,13]
Note: CRE if out of range
*/
int Getopcode(uint32_t inst)
{
        int rtn = (inst & OP_MASK)>>28;
        assert(rtn >= 0 && rtn < 14);
        return rtn;
}
/*
setReg:
Takes a non-loadval instruction and set the corresponding registers 
Args: the instruction as uint 32, register pointers (uint32 pointers) by 
reference
Return: none
*/
void setRef(uint32_t inst, uint32_t *ra,uint32_t *rb,uint32_t *rc)
{
        *ra = (inst & RA_MASK) >> 6;
        *rb = (inst & RB_MASK) >> 3;
        *rc = inst & RC_MASK;
}
/*

setLoad: 
Takes a loadval instruction and set the corresponding register and values
Args: the instruction as uint 32, register pointer (uint32 pointers) by reference
Return: uint32 value
Expects: instruction has valid opcode 13, register pointer not null
Note: CRE if out of range or register null
*/
uint32_t setLoad(uint32_t inst, uint32_t *ra)
{
        *ra = (inst & LV_MASK) >> 25;
        return inst & VAL_MASK;
}
