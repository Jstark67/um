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
        if (argc > 2) {
                fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
                exit(1);
        }

        /* innitialize the universal machine */
        Machine mach = malloc(sizeof(*mach));
        assert(mach != NULL);

        for (uint32_t i = 0; i < NUM_REG; i++){
                mach->reg[i] = 0;
        }

        if (argc == 2) {
                FILE *fp = fopen(argv[1], "r");
                assert(fp != NULL);
                mach->mem = mem_init(read_code(fp));
                fclose(fp);
        } else {
                mach->mem = mem_init(read_code(stdin));
        }

        /* execute individual program instructions */
        uint32_t curP = UArray_length(Seq_get(mach->mem->seg_mem, 0));
        for (mach->program_counter = 0; mach->program_counter < curP; 
             mach->program_counter++) {
                callExe(mach,&mach->program_counter,&curP);
        }
        
        /* free the program after use */
        mem_free(&mach->mem);
        free(mach);
}