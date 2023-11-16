#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <uarray.h>

#include "uinterprate.h"
#include "umemory.h"

// #include "uexecute.h"

//#include <>

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

const int CMD_SIZE = 4;
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

// void callExe(Mem_T mem,uint32_t curInst)
// {
//         uint32_t opcode, value, *ra,*rb,*rc;

// } not thought through;

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
        UArray_T program; 
        if (argc > 2) {
                fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
                exit(1);
        }

        if (argc == 2) {
                FILE *fp = fopen(argv[1], "r");
                assert(fp != NULL);
                program = read_code(fp);
                fclose(fp);
        } else {
                program = read_code(stdin);
        }
        Mem_T mem = mem_init(program);
        assert(Seq_get(mem->seg_mem, 0) == program);
        umemory_map_unmap_test(mem);
        mem_free(&mem);

        // int i;
        // for (i = 0; i < count; i++)
        // {
        //         break;
        // }
        

}