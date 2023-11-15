#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <uarray.h>

#include "uinterprate.h"
#include "umemory.h"
#include "uexecute.h"

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

void printArr(UArray_T a)
{
        for (int i = 0; i < UArray_length(a); i++) {
                uint32_t *cur_code = UArray_at(a, i);
                // printf("code is %d at line %d\n",*cur_code,i );
                (void)cur_code;
        }
        
}
int main(int argc, char* argv[])
{
        printA();
        printB();
        printC();

        
        /*initialize with input*/
        int i;
        UArray_T arr; 
        for (i = 0; i < argc; i++){
                if (argc - i > 2) {
                        fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
                        exit(1);
                }
                printf("%d,%d\n",i,argc);
        }
        assert(argc - i <= 1);    /* at most one file on command line */
        printf("i: %d, argc: %d\n", i, argc);
     
        if (i == argc) {
                FILE *fp = fopen(argv[i - 1], "r");
                assert(fp != NULL);
                printf("fuckme\n");
                
                arr = read_code(fp);
                fclose(fp);
        } else {
                arr = read_code(stdin);
        }
        printf("size: %u\n", UArray_length(arr));
        printArr(arr);
        
        UArray_free(&arr);
}