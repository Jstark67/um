
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>


#define CMD_SIZE 4
#define NUM_REG 8

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
        for(uint32_t i = 0; i < size + 1; i++) {
                // printf("index: %u\n", i);
                if(mem[i] != NULL) {free(mem[i]); mem[i] = NULL;}
        }free(unmapped); 
}



static inline uint32_t mem_map(uint32_t len)
{
        if (len == 0){
                if(usize){
                        return unmapped[--usize];
                }else {
                        return ++size;
                }
        }

        // uint32_t* Seg = calloc(len + 1, 4);
        // assert(Seg);
        uint32_t idx;
        if (usize) {
                idx = unmapped[--usize];
                
        } else {
                
                if (size >= capacity) {
                        capacity = capacity * 2 + 2;
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
                ucap = ucap * 2 + 2;
                unmapped = realloc(unmapped,ucap * sizeof(uint32_t));
                // assert(unmapped);
        }
        unmapped[usize++] = idx;
        if(mem[idx] != NULL) {
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

        uint32_t *prog = calloc(num_words + 1, 4);
        uint32_t len = num_words;

        

        
       capacity = 10;
       size = 1;
       ucap = 1;
       usize = 0;
       mem = calloc(capacity , 8);
//        length = malloc(capacity * 4);
       unmapped = calloc(ucap , 4);
       mem[0] = calloc(len + 1, 4);
       mem[0][0] = len;
       for (long i = 0; i < len; i++) {
                for (int j = 0; j < CMD_SIZE; j++) { 
                        prog[i] = prog[i] << 8;
                        prog[i] += (int)fgetc(fp);
                }
                mem[0][i + 1] = prog[i];

        }
       printf("%u\n", mem[0][1]);
//        length[0] = len;

        fclose(fp);

        uint32_t counter = 1;
        // printf("length %u\n", len);
        while (counter <= len) {
                inst = mem[0][counter];
                opcode = inst >> 28;
                // printf("%u\n", opcode);
                if (opcode < LV){
                        // printf("inside: %u\n", opcode);
                        a = inst << 23 >>29;
                        b = inst << 26 >>29;
                        c = inst << 29 >>29;

                        switch(opcode) {
                                case ACTIVATE:
                                        reg[b] = mem_map(reg[c]);
                                        break;
                                case SSTORE:
                                        mem[reg[a]][reg[b]+1] = reg[c];
                                        break;
                                case SLOAD:
                                        reg[a] = mem[reg[b]][reg[c]+1];
                                        break;
                                case ADD:
                                        reg[a] = reg[b] + reg[c];
                                        break;
                                case MUL:
                                        reg[a] = reg[b] * reg[c];
                                        break;
                                case DIV:
                                        reg[a] = reg[b] / reg[c];
                                        break;
                                case NAND:
                                        reg[a] = ~( reg[b] & reg[c]);
                                        break;
                                case HALT:
                                        mem_free();
                                        exit(0);
                                        break;
                                case CMOV:
                                        if (reg[c]) reg[a] = reg[b];
                                        // printf("regC: %u, regA: %u, regB: %u\n", reg[c], reg[a], reg[b]);
                                        // printf("counter: %u, length: %u\n", counter, len);
                                        break;
                                case INACTIVATE:
                                        mem_unmap(reg[c]);
                                        break;
                                case OUT:
                                        fputc(reg[c], stdout);
                                        break;
                                case IN:
                                        reg[c] = fgetc(stdin);
                                        break;
                                case LOADP:
                                        counter = reg[c]; 
                                        if (reg[b] != 0) {
                                                // printf("work1\n");
                                                int size = mem[reg[b]][0] ;
                                                // printf("work2\n");
                                                free(mem[0]);
                                                // printf("work3\n");
                                                mem[0] = calloc(size+1,4);
                                                // printf("work4\n");
                                                memcpy(mem[0], mem[reg[b]], (size+1)*4);
                                                len = mem[0][0];
                                        }
                                        break;
                // Add additional cases for other opcodes
                                default:
                                        // Handle unknown opcode
                                        break;
                                }

                } else if (opcode == LV) {
                        value = Bitpack_getu(inst,VAL_LEN,0);
                        reg[Bitpack_getu(inst,REG_LEN,VAL_LEN)] = value;

                } else {
                        exit(1);
                }
                counter += 1;
        }
       
        mem_free();
        
        exit(0);
}