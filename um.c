
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>


#define CMD_SIZE 4
#define NUM_REG 8
#define VALUE_FLAG 33554431
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


#define OP_LEN 4
#define REG_LEN 3
#define VAL_LEN 25
#define Bitpack_getu(word, width, lsb) (((word) << (32 - ((lsb) + (width)))) >> (32 - (width)))


uint32_t **mem;
// uint32_t *length;
uint32_t *unmapped;
uint32_t capacity;
uint32_t size;
uint32_t ucap;
uint32_t usize;
uint32_t reg[NUM_REG] = {0,0,0,0,0,0,0,0};
uint32_t inst, opcode, value, a, b, c;









static inline void mem_free()
{
        size++;
        for(uint32_t i = 0; i < size; i++) {
                // printf("index: %u\n", i);
                if(mem[i] != NULL) {free(mem[i]); }
        }free(unmapped); 
}



static inline uint32_t mem_map(uint32_t len)
{

        // uint32_t* Seg = calloc(len + 1, 4);
        // assert(Seg);
        uint32_t idx;
        if (usize) {
                idx = unmapped[--usize];
                
        } else {
                
                if (size >= capacity) {
                        capacity = capacity * 4 + 4;
                        mem = realloc(mem, capacity * 8); 
                        assert(mem);
                }
                idx = size++;
        }
        mem[idx] = calloc(len + 1, 4);
        mem[idx][0] = len;
        return idx;
}

static inline void mem_unmap(uint32_t idx){
        if (usize == ucap) {
                ucap = ucap * 4 + 4;
                unmapped = realloc(unmapped,ucap * sizeof(uint32_t));
                // assert(unmapped);
        }
        unmapped[usize++] = idx;
        if(mem[idx]) {
                free(mem[idx]);
                mem[idx] = NULL;
        }
}






int main(int argc, char* argv[])
{
        if (argc != 2) {
                fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
                exit(1);
        }


        struct stat stats;
        assert(stat(argv[1], &stats) == 0);
        int num_words = stats.st_size / 4;

        FILE *fp = fopen(argv[1], "r");
        assert(fp);

        uint32_t *prog = calloc(num_words + 1, sizeof(uint32_t));
        uint32_t len = num_words;

        

        
       capacity = 1;
       size = 1;
       ucap = 1;
       usize = 0;
       mem = calloc(capacity , 8);
//        length = malloc(capacity * 4);
       unmapped = calloc(ucap , 4);
       mem[0] = calloc(len + 1, 4);
       mem[0][0] = len;
       for (long i = 0; i < len; i++) {
                prog[i] = 0;

                int ch1 = fgetc(fp);
                int ch2 = fgetc(fp);
                int ch3 = fgetc(fp);
                int ch4 = fgetc(fp);
                prog[i] = ((ch1 << 24) | (ch2 << 16) | (ch3 << 8) | ch4);
                // for (int j = 0; j < CMD_SIZE; j++) { 
                //         prog[i] = prog[i] << 8;
                //         prog[i] += (int)fgetc(fp);
                // }
                 mem[0][i + 1] = prog[i];

        }
//        length[0] = len;

        fclose(fp);
        

        

        
        setbuf(stdin,NULL);
        setbuf(stdout, NULL);

        
        uint32_t counter = 1;
        uint32_t *program = mem[0];
        // printf("length %u\n", len);
        while (counter <= len) {
                inst = program[counter];
                opcode = inst >> 28;
                // printf("%u\n", opcode);
                if (opcode < LV){
                        // printf("inside: %u\n", opcode);
                        

                        switch(opcode) {
                                case SLOAD:
                                a = Bitpack_getu(inst,REG_LEN,REG_LEN*2);
                                b = Bitpack_getu(inst,REG_LEN,REG_LEN);
                                c = Bitpack_getu(inst,REG_LEN,0);
                                        reg[a] = mem[reg[b]][reg[c]+1];
                                        break;
                                case SSTORE:
                                a = Bitpack_getu(inst,REG_LEN,REG_LEN*2);
                                b = Bitpack_getu(inst,REG_LEN,REG_LEN);
                                c = Bitpack_getu(inst,REG_LEN,0);
                                        mem[reg[a]][reg[b]+1] = reg[c];
                                        break;
                                case ACTIVATE:
                                b = Bitpack_getu(inst,REG_LEN,REG_LEN);
                                c = Bitpack_getu(inst,REG_LEN,0);
                                        reg[b] = mem_map(reg[c]);
                                        break;

                                case INACTIVATE:

                                c = Bitpack_getu(inst,REG_LEN,0);
                                        mem_unmap(reg[c]);
                                        break;

                                case ADD:
                                a = Bitpack_getu(inst,REG_LEN,REG_LEN*2);
                                b = Bitpack_getu(inst,REG_LEN,REG_LEN);
                                c = Bitpack_getu(inst,REG_LEN,0);
                                        reg[a] = reg[b] + reg[c];
                                        break;
                                case MUL:
                                a = Bitpack_getu(inst,REG_LEN,REG_LEN*2);
                                b = Bitpack_getu(inst,REG_LEN,REG_LEN);
                                c = Bitpack_getu(inst,REG_LEN,0);
                                        reg[a] = reg[b] * reg[c];
                                        break;
                                case DIV:
                                a = Bitpack_getu(inst,REG_LEN,REG_LEN*2);
                                b = Bitpack_getu(inst,REG_LEN,REG_LEN);
                                c = Bitpack_getu(inst,REG_LEN,0);
                                        reg[a] = reg[b] / reg[c];
                                        break;
                                case NAND:
                                a = Bitpack_getu(inst,REG_LEN,REG_LEN*2);
                                b = Bitpack_getu(inst,REG_LEN,REG_LEN);
                                c = Bitpack_getu(inst,REG_LEN,0);
                                        reg[a] = ~( reg[b] & reg[c]);
                                        break;
                                case HALT:
                                        mem_free();
                                        exit(0);
                                        break;
                                case CMOV:
                                a = Bitpack_getu(inst,REG_LEN,REG_LEN*2);
                                b = Bitpack_getu(inst,REG_LEN,REG_LEN);
                                c = Bitpack_getu(inst,REG_LEN,0);
                                        if (reg[c]) reg[a] = reg[b];
                                        // printf("regC: %u, regA: %u, regB: %u\n", reg[c], reg[a], reg[b]);
                                        // printf("counter: %u, length: %u\n", counter, len);
                                        break;
                        
                                case OUT:

                                c = Bitpack_getu(inst,REG_LEN,0);
                                        fputc(reg[c], stdout);
                                        break;
                                case IN:

                                c = Bitpack_getu(inst,REG_LEN,0);
                                        reg[c] = fgetc(stdin);
                                        break;
                                case LOADP:

                                b = Bitpack_getu(inst,REG_LEN,REG_LEN);
                                c = Bitpack_getu(inst,REG_LEN,0);
                                        counter = reg[c]; 
                                        if (reg[b]) {
                                                // printf("work1\n");
                                                int size = (mem[reg[b]][0] +1 )* 4;
                                                // printf("work2\n");
                                                free(mem[0]);
                                                // printf("work3\n");
                                                mem[0] = malloc(size);
                                                // printf("work4\n");
                                                memcpy(mem[0], mem[reg[b]], size);
                                                program = mem[0];
                                                len = mem[0][0];
                                        }
                                        break;
                // Add additional cases for other opcodes
                              
                                }

                } else if (opcode == LV) {
                        value = inst & VALUE_FLAG;
                        reg[Bitpack_getu(inst,REG_LEN,VAL_LEN)] = value;

                } else {
                        exit(1);
                }
                counter += 1;
        }
       
        mem_free();
        
        exit(0);
}