/**************************************************************
 *
 *                     uexecute.c
 *
 *     Assignment: HW6 um
 *     Authors:  David Chen and Sam Hu
 *     Date:  Nov 20th
 *
 *     summary:
 *     uexecute.c provides the functionality of the execution operations
 *     in a universal machine. The operations utilize umemory as the 
 *     building blocks to return the desired outcome.
 *
 **************************************************************/
#include "uexecute.h"


/* move
 * move the content in register rb to register ra if register rc is not zero
 * Params: 
 *      - uint32_t *ra: Pointer to the register to store the value
 *      - uint32_t *rb: Pointer to the register to store the source
 *      - uint32_t *rc: Pointer to the register that holds the condition
 * Return: 
 *      - void
*/
void move(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        if(*rc != 0){
                *ra = *rb;
        }
}

/* segL (Segment Load)
 * Load a value from a specified memory segment and offset into a register
 * Params: 
 *      - Mem_T mem: Memory structure from which to load
 *      - uint32_t *ra: Pointer to the register to store the loaded value
 *      - uint32_t *rb: Pointer to the register holding the segment identifier
 *      - uint32_t *rc: Pointer to the register holding the offset within the 
 * segment
 * Return: 
 *      - void
 */
void segL(Mem_T mem, uint32_t *ra, uint32_t *rb, uint32_t *rc){
        *ra = mem_load(mem,*rb,*rc);
}

/* segS (Segment Store)
 * Store a value into a specified memory segment and offset
 * Params: 
 *      - Mem_T mem: Memory structure to which the value will be stored
 *      - uint32_t *ra: Pointer to the register holding the value to store
 *      - uint32_t *rb: Pointer to the register holding the segment identifier
 *      - uint32_t *rc: Pointer to the register holding the offset within the 
 * segment
 * Return: 
 *      - void
 */
void segS(Mem_T mem,uint32_t *ra, uint32_t *rb, uint32_t *rc){
        mem_store(mem,*ra,*rb,*rc);
}

/* add
 * Adds two registers and stores the result in a third register
 * Params: 
 *      - uint32_t *ra: Pointer to the register to store the result
 *      - uint32_t *rb: Pointer to the first operand register
 *      - uint32_t *rc: Pointer to the second operand register
 * Return: 
 *      - void
 */
void add(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        *ra = *rb + *rc;
}

/* mult
 * Multiplies two registers and stores the result in a third register
 * Params: 
 *      - uint32_t *ra: Pointer to the register to store the result
 *      - uint32_t *rb: Pointer to the first operand register
 *      - uint32_t *rc: Pointer to the second operand register
 * Return: 
 *      - void
 */
void mult(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        *ra = *rb * *rc;
}

/* divide
 * Divides the first register by the second and stores the result in a 
 * third register
 * Params: 
 *      - uint32_t *ra: Pointer to the register to store the result
 *      - uint32_t *rb: Pointer to the dividend register
 *      - uint32_t *rc: Pointer to the divisor register
 * Return: 
 *      - void
 */
void divide(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        *ra = *rb / *rc;
}

/* nand
 * Performs bitwise NAND on two registers and stores the result in a third 
 * register
 * Params: 
 *      - uint32_t *ra: Pointer to the register to store the result
 *      - uint32_t *rb: Pointer to the first operand register
 *      - uint32_t *rc: Pointer to the second operand register
 * Return: 
 *      - void
 */
void nand(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        *ra = ~(*rb & *rc);
}

/* halt
 * Frees the memory segment
 * Params: 
 *      - Mem_T *mem: Pointer to the memory structure to be freed
 * Return: 
 *      - void
 */
void halt(Mem_T *mem)
{
        mem_free(mem);
}

/* map
 * Maps a new memory segment
 * Params: 
 *      - Mem_T mem: Memory structure
 *      - uint32_t *rb: Pointer to the register to store the identifier of 
 * the new segment
 *      - uint32_t *rc: Pointer to the register holding the size of 
 * the new segment
 * Return: 
 *      - void
 */
void map(Mem_T mem, uint32_t *rb, uint32_t *rc)
{
        *rb = mem_map(*rc, mem);
}

/* unmap
 * Unmaps a memory segment
 * Params: 
 *      - Mem_T mem: Memory structure to unmap
 *      - uint32_t *rc: Pointer to the register holding the identifier 
 * of the segment to unmap
 * Return: 
 *      - void
 */
void unmap(Mem_T mem, uint32_t *rc)
{
        mem_unmap(mem, *rc);
}

/* out
 * Outputs a character to the standard output
 * Params: 
 *      - uint32_t *rc: Pointer to the register holding the character to 
 * output
 * Return: 
 *      - void
 */
void out(uint32_t *rc)
{
        fputc((int)*rc,stdout);
}

/* in
 * Reads a character from the standard input
 * Params: 
 *      - uint32_t *rc: Pointer to the register to store the read character
 * Return: 
 *      - void
 */
void in(uint32_t *rc)
{
        *rc = fgetc(stdin);
}

/* loadP
 * Load program instructions from a register index to $m[0]
 * Params: 
 *      - Mem_T mem: Memory structure to load
 *      - uint32_t *rb: Pointer to the register that contains information
 * to load to $m[0]
 * Return: 
 *      - void
 */
int loadP(Mem_T mem, uint32_t *rb)
{
        return mem_loadP(mem,*rb);
}

/* lv (Load Value)
 * Loads a constant value into a register
 * Params: 
 *      - uint32_t value: The constant value to be loaded
 *      - uint32_t *rc: Pointer to the register where the value will be stored
 * Return: 
 *      - void
 * Description: 
 *      Sets the register pointed by rc to the specified value.
 */
void lv(uint32_t value, uint32_t *rc)
{
        *rc = value;
}

/* Helper function for halt that frees the machine structure */
void halt_exit(Machine *mach)
{
        free(*mach);
        exit(0);
}

/* callExe (Call Execute)
 * Executes an instruction in the machine's memory based on its opcode
 * Params: 
 *      - Machine mach: The machine instance containing the state, memory, and registers
 *      - uint32_t *line: Pointer to the program counter
 *      - uint32_t *curP: Pointer to the length of the program instructions
 * Return: 
 *      - void
 * Description: 
 *      This function decodes and executes an instruction from the machine's
 *      memory. It retrieves the instruction from the current line, 
 *      decodes its opcode, and executes the corresponding operation.
 *      The function updates the machine state accordingly, including 
 *      registers and program counters.
 */
void callExe(Machine mach, uint32_t *line,  uint32_t *curP)
{
        /* initialize components of the machine */
        uint32_t inst, opcode, value, a, b, c, *ra,*rb,*rc;
        inst = mem_inst(mach->mem, *line);
        opcode = getOpcode(inst);
        
        /* preprocess based on the opcode */
        if (opcode < LV){
                setRef(inst,&a,&b,&c);
                ra = &mach->reg[a];
                rb = &mach->reg[b];
                rc = &mach->reg[c];
        } else if (opcode == LV) {
                value = setLoad(inst,&c);
                rc = &mach->reg[c];
        } else {
                exit(1);
        }
        
        /* call specific execution functions */
        if (opcode == CMOV){
                move(ra,rb,rc);
        } else if (opcode == SLOAD){
                segL(mach->mem,ra,rb,rc);
        } else if (opcode == SSTORE){
                segS(mach->mem,ra,rb,rc);
        } else if (opcode == ADD){
                add(ra,rb,rc);
        } else if (opcode == MUL){
                mult(ra,rb,rc);
        } else if (opcode == DIV){
                divide(ra,rb,rc);
        } else if (opcode == NAND){
                nand(ra,rb,rc);
        } else if (opcode == HALT){
                halt(&mach->mem);
                halt_exit(&mach);
        } else if (opcode == ACTIVATE){
                map(mach->mem,rb,rc);
        } else if (opcode == INACTIVATE){
                unmap(mach->mem,rc);
        } else if (opcode == OUT){
                out(rc);
        } else if (opcode == IN){
                in(rc);
        } else if (opcode == LOADP){
                if (*rb == 0) {
                        *line = *rc - 1;
                } else {
                        *line = *rc - 1; /*-1 to handle i++; sok since signed*/
                        *curP = loadP(mach->mem,rb);
                }
        } else if (opcode == LV){
                lv(value,rc);
        } 
} 