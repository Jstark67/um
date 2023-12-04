
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>


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

typedef struct Memory
{
        uint32_t *mem;
        uint32_t length;
} *Memory;

typedef struct Mem_T {

        Memory seg_mem;
        uint32_t *unmapped;
        uint32_t capacity;
        uint32_t size;
        uint32_t ucap;
        uint32_t usize;
        
} *Mem_T;










static inline void mem_free(Mem_T *mem)
{
        for(uint32_t i = 0; i < (*mem)->size; i++) {
                if((*mem)->seg_mem[i].mem) {free((*mem)->seg_mem[i].mem); (*mem)->seg_mem[i].mem = NULL;}
        }free((*mem)->seg_mem);free((*mem)->unmapped);free(*mem);
}



static inline uint32_t mem_map(uint32_t length, Mem_T mem)
{
        if (length == 0){
                if(mem->usize){
                        return mem->unmapped[--mem->usize];
                }else {
                        return ++mem->size;
                }
        }

        uint32_t* Seg = calloc(length, 4);
        assert(Seg);
        uint32_t idx;
        if (mem->usize) {
                idx = mem->unmapped[--mem->usize];
        } else {
                
                if (mem->size == mem->capacity) {
                        mem->capacity *= 2;
                        mem->seg_mem = realloc(mem->seg_mem, mem->capacity * 16); 
                        // assert(mem->seg_mem);
                }
                idx = mem->size++;
        }
        mem->seg_mem[idx].mem = Seg;
        mem->seg_mem[idx].length = length;
        return idx;
}

static inline void mem_unmap(Mem_T mem, uint32_t idx){
        if (mem->usize == mem->ucap) {
                mem->ucap *= 2;
                mem->unmapped = realloc(mem->unmapped,mem->ucap * sizeof(uint32_t));
                // assert(mem->unmapped);
        }
        mem->unmapped[mem->usize++] = idx;
        if(mem->seg_mem[idx].mem != NULL) {
                free(mem->seg_mem[idx].mem);
                mem->seg_mem[idx].mem = NULL;
        }
}

// static inline uint32_t mem_loadP(Mem_T mem, uint32_t idx)
// {
//         int size = mem->seg_mem[idx].length * 4;
//         free(mem->seg_mem[0].mem);
//         mem->seg_mem[0].mem = malloc(size);
//         memcpy(mem->seg_mem[0].mem, mem->seg_mem[idx].mem, size);
//         return mem->seg_mem[idx].length;
// }




int main(int argc, char* argv[])
{
        if (argc != 2) {
                fprintf(stderr, "Usage: %s [filename]\n", argv[0]);
                exit(1);
        }




        FILE *fp = fopen(argv[1], "r");
        assert(fp);
        fseek(fp,0,SEEK_END);
        uint32_t *prog = malloc(ftell(fp));
        uint32_t length = ftell(fp) / CMD_SIZE;
        rewind(fp);

        for (long i = 0; i < length; i++) {
                prog[i] = 0;
                for (int j = 0; j < CMD_SIZE; j++) { 
                        prog[i] = prog[i] << 8;
                        prog[i] += (int)fgetc(fp);
                }

        }

        Mem_T mem = malloc(32); 
        assert(mem);
        mem->capacity = 10;
        mem->size = 1;
        mem->ucap = 10;
        mem->usize = 0;
        mem->seg_mem = malloc(mem->capacity * 16);
        mem->unmapped = malloc(mem->ucap * 4);
        mem->seg_mem[0].mem = prog;
        mem->seg_mem[0].length= length;

        fclose(fp);
        

        uint32_t len = mem->seg_mem[0].length;

        uint32_t inst, opcode, value, a, b, c;
        uint32_t reg[NUM_REG];
        for (uint32_t i = 0; i < NUM_REG; i++){
                reg[i] = 0;
        }
        
        uint32_t counter;

        for (counter = 0;  counter < len; counter++) {
                inst = mem->seg_mem[0].mem[counter];
                opcode = Bitpack_getu(inst,OP_LEN,28);
                if (opcode < LV){
                        a = Bitpack_getu(inst,REG_LEN,REG_LEN*2);
                        b = Bitpack_getu(inst,REG_LEN,REG_LEN);
                        c = Bitpack_getu(inst,REG_LEN,0);


                        switch(opcode) {
    case ACTIVATE:
        reg[b] = mem_map(reg[c], mem);
        break;
    case SSTORE:
        mem->seg_mem[reg[a]].mem[reg[b]] = reg[c];
        break;
    case SLOAD:
        reg[a] = mem->seg_mem[reg[b]].mem[reg[c]];
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
        reg[a] = ~(reg[b] & reg[c]);
        break;
    case HALT:
        mem_free(&mem);
        exit(0);
        break;
    case CMOV:
        if (reg[c]) reg[a] = reg[b];
        break;
    case INACTIVATE:
        mem_unmap(mem, reg[c]);
        break;
    case OUT:
        fputc(reg[c], stdout);
        break;
    case IN:
        reg[c] = fgetc(stdin);
        break;
    case LOADP:
        counter = reg[c] - 1; 
        if (reg[b]) {
            int size = mem->seg_mem[reg[b]].length * sizeof(uint32_t);
            free(mem->seg_mem[0].mem);
            mem->seg_mem[0].mem = malloc(size);
            memcpy(mem->seg_mem[0].mem, mem->seg_mem[reg[b]].mem, size);
            len = mem->seg_mem[reg[b]].length;
        }
        break;
    // Add additional cases for other opcodes
    default:
        // Handle unknown opcode
        break;
}


                        // if (opcode == ACTIVATE){
                        //         reg[b] = mem_map(reg[c],mem);
                        // } else if (opcode == SSTORE){
                        //         mem->seg_mem[reg[a]].mem[reg[b]]= reg[c];
                        // } else if (opcode == SLOAD){
                        //         reg[a] = mem->seg_mem[reg[b]].mem[reg[c]];
                        // } else if (opcode == ADD){
                        //         reg[a] = reg[b] + reg[c];
                        // } else if (opcode == MUL){
                        //         reg[a] = reg[b] * reg[c];
                        // } else if (opcode == DIV){
                        //         reg[a] = reg[b] / reg[c];
                        // } else if (opcode == NAND){
                        //         reg[a] = ~(reg[b] & reg[c]);
                        // } else if (opcode == HALT){
                        //         mem_free(&mem);
                        //         exit(0);
                        // } else if (opcode == CMOV){
                        //         if (reg[c]) reg[a] = reg[b];
                        // } else if (opcode == INACTIVATE){
                        //         mem_unmap(mem,reg[c]);
                        // } else if (opcode == OUT){
                        //         fputc(reg[c],stdout);
                        // } else if (opcode == IN){
                        //         reg[c] = fgetc(stdin);
                        // } else if (opcode == LOADP){
                        //         counter = reg[c]-1; 
                        //         if (reg[b]){
                        //                 // len = mem_loadP(mem, reg[b]);
                        //                 int size = mem->seg_mem[reg[b]].length * 4;
                        //                 free(mem->seg_mem[0].mem);
                        //                 mem->seg_mem[0].mem = malloc(size);
                        //                 memcpy(mem->seg_mem[0].mem, mem->seg_mem[reg[b]].mem, size);
                        //                 len = mem->seg_mem[reg[b]].length;
                        //         }
                        // }
                } else if (opcode == LV) {
                        value = Bitpack_getu(inst,VAL_LEN,0);
                        reg[Bitpack_getu(inst,REG_LEN,VAL_LEN)] = value;

                } else {
                        exit(1);
                }
        }
       
        mem_free(&mem);
        
        exit(0);
}