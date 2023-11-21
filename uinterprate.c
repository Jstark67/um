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

const int OP_MASK = 0xF0000000;
const int RA_MASK = 0x000001c0;
const int RB_MASK = 0x00000038;
const int RC_MASK = 0x00000007;
const int LV_MASK = 0x0d000000;
const int VAL_MASK = 0x01ffffff;

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
        return Bitpack_getu(inst,4,28);
}
/* setRef
 * Sets the register references based on the given instruction
 * Params: 
 *      - uint32_t inst: The instruction containing encoded register references
 *      - uint32_t *ra: Pointer to the register to set with the first reference
 *      - uint32_t *rb: Pointer to the register to set with the second reference
 *      - uint32_t *rc: Pointer to the register to set with the third reference
 * Return: 
 *      - void
 */
void setRef(uint32_t inst, uint32_t *ra,uint32_t *rb,uint32_t *rc)
{
        *ra = Bitpack_getu(inst,3,6);
        *rb = Bitpack_getu(inst,3,3);
        *rc = Bitpack_getu(inst,3,0);
}


/* setLoad
 * Extracts a register reference and a value from a load instruction
 * Params: 
 *      - uint32_t inst: The load instruction containing the encoded register reference and value
 *      - uint32_t *ra: Pointer to the register to set with the extracted reference
 * Return: 
 *      - uint32_t: The extracted value from the instruction
 */
uint32_t setLoad(uint32_t inst, uint32_t *ra)
{
        *ra = Bitpack_getu(inst,3,25);
        return Bitpack_getu(inst,25,0);
}
