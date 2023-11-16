#ifndef UMEM_H
#define UMEM_H

#include <seq.h>
#include <uarray.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>





// typedef struct Mem_T *Mem_T;
typedef struct Mem_T {
        Seq_T seg_mem;
        Seq_T unmapped;
} *Mem_T;

void umemory_map_unmap_test(Mem_T mem);
Mem_T mem_init(UArray_T program);
uint32_t mem_map(uint32_t size, Mem_T mem);
void mem_unmap(Mem_T mem, uint32_t idx);
uint32_t mem_load(Mem_T mem, uint32_t memi, uint32_t segi);
void mem_store(Mem_T mem, uint32_t memi, uint32_t segi,uint32_t value);
void mem_loadP(Mem_T mem, uint32_t idx);
void mem_free(Mem_T *mem);
#endif
