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

        int length = ftell(fp) /CMD_SIZE;
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
void halt_exit(Machine *mach)
{
        free(*mach);
        exit(0);
}
void callExe(Machine mach, uint32_t *line,  uint32_t *curP)
{
        uint32_t inst, opcode, value, a, b, c, *ra,*rb,*rc;
        inst = mem_inst(mach->mem, *line);
        opcode = getOpcode(inst);
        //printf("opcode = %u\n",opcode);
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
                // printf("CMOV\n");
                move(ra,rb,rc);
        } else if (opcode == SLOAD){
                // printf("SLOAD\n");
                segL(mach->mem,ra,rb,rc);
        } else if (opcode == SSTORE){
                // printf("SSTORE\n");
                segS(mach->mem,ra,rb,rc);
        } else if (opcode == ADD){
                // printf("ADD\n");
                add(ra,rb,rc);
        } else if (opcode == MUL){
                // printf("MUL\n");
                mult(ra,rb,rc);
        } else if (opcode == DIV){
                // printf("DIV\n");
                divide(ra,rb,rc);
        } else if (opcode == NAND){
                // printf("NAND\n");
                nand(ra,rb,rc);
        } else if (opcode == HALT){
                // printf("HALT\n");
                halt(&mach->mem);
                halt_exit(&mach);
        } else if (opcode == ACTIVATE){
                // printf("ACTIVATE\n");
                map(mach->mem,rb,rc);
        } else if (opcode == INACTIVATE){
                // printf("INACTIVATE\n");
                unmap(mach->mem,rc);
        } else if (opcode == OUT){
                // printf("OUT\n");
                out(rc);
        } else if (opcode == IN){
                // printf("IN\n");
                in(rc);
        } else if (opcode == LOADP){
                // printf("LOADP\n");
                if (*rb == 0) {
                        *line = *rc - 1;
                } else {
                        *line = *rc - 1; /*-1 to handle i++; sok since signed*/
                        *curP = loadP(mach->mem,rb);
                }
        } else if (opcode == LV){
                // printf("LV\n");
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

        // umemory_load_store_test(mach->mem);
        /*execution cycle*/
        uint32_t i;
        uint32_t curP = UArray_length(Seq_get(mach->mem->seg_mem, 0));
        for (i = 0; i < curP; i++) {
                // printf("%u\n", i);
                callExe(mach,&i,&curP);
        }
        mem_free(&mach->mem);
        free(mach);
        
}