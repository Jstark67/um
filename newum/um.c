/**************************************************************
 *
 *                     um.c
 *
 *     Assignment: HW6 um
 *     Authors:  David Chen and Sam Hu
 *     Date:  Nov 20th
 *
 *     summary:
 *      
 *     um.c contains driver functions for the universal machine. The
 *     program reads in a list of program instructions, store them 
 *     in a virtual memory system and execute them individually. 
 *
 **************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <uarray.h>
#include "uinterprate.h"
#include "umemory.h"
#include "uexecute.h"


#define CMD_SIZE 4
#define NUM_REG 8

/* callExe (Call Execute)
 * Executes an instruction in the machine's memory based on its opcode
 * Params: 
 *      - Machine mach: The machine instance containing the state, memory, 
        and registers
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
                        *line = *rc - 1; /*-1 to handle i++;*/
                        *curP = loadP(mach->mem,rb);
                }
        } else if (opcode == LV){
                lv(value,rc);
        } 
} 

/********** read_code ********
 *
 * Reads file input and return a Uarray of program instructions
 *
 * Parameters:
 *      FILE *fp: input stream that contains program instructions
 *
 * Return: UArray_T of uint32_ts: program instructions
 * 
 * Expects
 *      input must not be NULL
 * Notes:
 *      Will CRE if input is NULL
 ************************/
UArray_T read_code(FILE *fp)
{
        /* Error Handling */
        assert(fp != NULL);
        
        /* Process input and find the number of instructions */
        fseek(fp,0,SEEK_END);
        int length = ftell(fp) / CMD_SIZE;
        assert(ftell(fp) != 0);
        rewind(fp);

        /* Create an empty UArray and innitialize it */
        UArray_T program = UArray_new(length,CMD_SIZE);
        for (long i = 0; i < length; i++) {
                uint32_t code = 0;
                for (int j = 0; j < CMD_SIZE; j++) { /* big endian insertion*/
                        code = code << 8;
                        code += (int)fgetc(fp);
                }
                uint32_t *cur_code = UArray_at(program, i);
                *cur_code = code;
        }
        
        return program;
}


/* main */
int main(int argc, char* argv[])
{
        /* handle false input */
        if (argc != 2) {
                fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
                exit(1);
        }

        /* innitialize the universal machine */
        Machine mach = malloc(sizeof(*mach));
        assert(mach != NULL);
        for (uint32_t i = 0; i < NUM_REG; i++){
                mach->reg[i] = 0;
        }


        /*load program*/
        FILE *fp = fopen(argv[1], "r");
        assert(fp != NULL);
        mach->mem = mem_init(read_code(fp));
        fclose(fp);
        

        /* execute individual program instructions */
        uint32_t curP = UArray_length(Seq_get(mach->mem->seg_mem, 0));
        for (mach->program_counter = 0; mach->program_counter < curP; 
             mach->program_counter++) {
                callExe(mach,&mach->program_counter,&curP);
        }
        /* free the program after use if not freed already */
        mem_free(&mach->mem);
        free(mach);
}