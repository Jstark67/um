/**************************************************************
 *
 *                     uinterprate.c
 *
 *     Assignment: HW6 um
 *     Authors:  David Chen and Sam Hu
 *     Date:  Nov 20th
 *
 *     summary:
 *      
 *     uinterprate.c contains implementations of opcode
 *     preprocess and interpretation operations functions. These
 *     functions serve as a prelinimary step to operation execution
 *
 **************************************************************/
#include "uinterprate.h"

#define OP_LEN 4
#define REG_LEN 3
#define VAL_LEN 25

uint32_t Bitpack_getu(uint32_t word, unsigned width, unsigned lsb)
{
        return (word <<(32 - (lsb + width))) >> (32 - width);
}

/* getOpcode
 * Extracts the opcode from an uint32_t instruction
 * Params: 
 *      - uint32_t inst: The instruction from which the opcode will 
 * be extracted
 * Return: 
 *      - uint32_t: The extracted opcode
 */
uint32_t getOpcode(uint32_t inst)
{
        return Bitpack_getu(inst,OP_LEN,28);
}
/* setRef
 * Sets the register references based on the given instruction
 * Params: 
 *      - uint32_t inst: The instruction containing encoded register references
 *      - uint32_t *ra: Pointer to the register to set with the first reference
 *      - uint32_t *rb: Pointer to the register to set with the second 
 * reference
 *      - uint32_t *rc: Pointer to the register to set with the third reference
 * Return: 
 *      - void
 */
void setRef(uint32_t inst, uint32_t *ra,uint32_t *rb,uint32_t *rc)
{
        
        *ra = Bitpack_getu(inst,REG_LEN,REG_LEN*2);
        *rb = Bitpack_getu(inst,REG_LEN,REG_LEN);
        *rc = Bitpack_getu(inst,REG_LEN,0);
        //printf("setRef: %u \n", *rb);
}


/* setLoad
 * Extracts a register reference and a value from a load instruction
 * Params: 
 *      - uint32_t inst: The load instruction containing the encoded register 
 * reference and value
 *      - uint32_t *ra: Pointer to the register to set with the extracted 
 * reference
 * Return: 
 *      - uint32_t: The extracted value from the instruction
 */
uint32_t setLoad(uint32_t inst, uint32_t *ra)
{
        *ra = Bitpack_getu(inst,REG_LEN,VAL_LEN);
        return Bitpack_getu(inst,VAL_LEN,0);
}
