#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <uarray.h>

#include "uinterprate.h"
#include "umemory.h"

#include "uexecute.h"

//#include <>
#define CMD_SIZE 4
#define NUM_REG 8
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

typedef struct Machine {
        Mem_T mem;
        uint32_t reg[NUM_REG];

} *Machine;

UArray_T read_code(FILE *fp)
{
        fseek(fp,0,SEEK_END);

        int length = ftell(fp) / CMD_SIZE;
        assert(ftell(fp) != 0);

        rewind(fp);
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

void callExe(Machine mach, int *line,  int *curP)
{
        uint32_t inst, opcode, value, a, b, c, *ra,*rb,*rc;
        inst = mem_inst(mach->mem, *line);
        printf("inst = %u\n",inst);
        opcode = getOpcode(inst);
        printf("opcode = %u\n",opcode);
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
        } else if (opcode == ACTIVATE){
                map(mach->mem,rb,rc);
        } else if (opcode == INACTIVATE){
                unmap(mach->mem,rc);
        } else if (opcode == OUT){
                out(rc);
        } else if (opcode == IN){
                in(rc);
        } else if (opcode == LOADP){
                if(*rb == 0) *line = *rc;
                else {
                        *line = *rc - 1; /*-1 to handle i++; sok since signed*/
                        *curP = loadP(mach->mem,rb);
                }
        } else if (opcode == LV){
                lv(value,rc);
        } 
        
        
} 

void printArr(UArray_T a)
{
        for (int i = 0; i < UArray_length(a); i++) {
                uint32_t *cur_code = UArray_at(a, i);
                (void)cur_code;
        }
        
}
int main(int argc, char* argv[])
{


        
        /*initialize with input*/
         
        if (argc > 2) {
                fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
                exit(1);
        }
        Machine mach = malloc(sizeof(*mach));
        assert(mach != NULL);

        for (int i = 0; i < NUM_REG; i++){
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

        umemory_map_unmap_test(mach->mem);
        umemory_load_store_test(mach->mem);

        /*execution cycle*/
        int i;
        int curP = UArray_length(Seq_get(mach->mem->seg_mem, 0));
        for (i = 0; i < curP; i++) {
                callExe(mach,&i,&curP);
        }
        mem_free(&mach->mem);
        free(mach);
        
}