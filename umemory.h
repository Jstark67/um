/**************************************************************
 *
 *                     umemory.h
 *
 *     Assignment: HW6 um
 *     Authors:  David Chen and Sam Hu
 *     Date:  Nov 20th
 *
 *     summary:
 *      
 *     umemory.h is the interface of the memory system and all
 *     the memory handling functions
 *
 **************************************************************/
#ifndef UMEM_H
#define UMEM_H

#include <seq.h>
#include <uarray.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* The following defines the struct of a memory system */
typedef struct Mem_T {
        /* */
        // Seq_T seg_mem;
        // Seq_T unmapped;
        uint32_t **seg_mem;
        uint32_t *unmapped;
        uint32_t capacity;
        uint32_t size;
        uint32_t ucap;
        uint32_t usize;
        
} *Mem_T;



//uint32_t **mem;

// OR 

// typedef struct Linkmem {
//         uint32_t* segment;
//         linkmem* next;
// }Linkmem;

// typedef struct NewMem_T {
//         Linkmem seg_mem;
//         Linkmem unmapped;
// };

/* Memory Operations */
Mem_T mem_init(UArray_T program);
uint32_t mem_map(uint32_t size, Mem_T mem);
void mem_unmap(Mem_T mem, uint32_t idx);
uint32_t mem_load(Mem_T mem, uint32_t memi, uint32_t segi);
void mem_store(Mem_T mem, uint32_t memi, uint32_t segi,uint32_t value);
uint32_t mem_loadP(Mem_T mem, uint32_t idx);
void mem_free(Mem_T *mem);
uint32_t mem_inst(Mem_T mem, int line);
// void umemory_map_unmap_test(Mem_T mem);
// void umemory_load_store_test(Mem_T mem);
#endif
