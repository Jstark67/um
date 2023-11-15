#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <uarray.h>
//#include <>

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

UArray_T read_code(FILE *fp)
{
        return 1;
}

int main(int argc, char* argv[])
{
        /*initialize with input*/
        int i;
        for (i = 0; i < argc; i++){
                if (argc - i > 2) {
                        fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
                        exit(1);
                } else {
                        break;
                }
        }
        assert(argc - i <= 1);    /* at most one file on command line */
        if (i < argc) {
                FILE *fp = fopen(argv[i], "r");
                assert(fp != NULL);
                
                fclose(fp);
        } else {
                compress_or_decompress(stdin);
        }

        
}