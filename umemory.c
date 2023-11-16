#ifndef UMEM_H
#define UMEM_H

#include <seq.h>
#include <uarray.h>
#include <assert.h>
#include <stdio.h>
#include "umemory.h"

const int HINT = 0;

typedef struct Mem_T {
        Seq_T seg_mem;
        Seq_T unmapped;
        uint32_t maxID;
} *Mem_T

/* mem_init
 * Initialize a Mem_T object and populate m[0] through the inputted program instructions
 * Params: Uarray_T of uint32_ts that stores program instructions
 * Return: a Mem_T object, with m[0] substantiated
 * Expectation: CRE will occur if the initialization of Hanson sequences failed, which would be handled by Hanson’s exception mechanisms
*/

Mem_T mem_init(UArray_T program)
{       
        /* allocate host memory for the virtual memory */
        Mem_T mem = malloc(sizeof(*mem));
        assert(mem != NULL);

        /* allocate host memory for the two sequences */
        mem->seg_mem = Seq_new(HINT);
        mem->unmapped = Seq_new(HINT);
        
        /* add the program instructions to $m[0] */
        Seq_addhi(mem->seg_mem, program);
        mem->maxID = 0;

        return mem;
}

/* mem_free
 * Free a Mem_T struct and all its segments
 * Params: address of the em_T object oto be freed * Return: a Mem_T object, with m[0] substantiated
 * Expectation: CRE if address or segment itself is null
*/
void mem_free(Mem_T *mem)
{       
        /* assure thatt virtual memory was allocated before freeing */
        assert(mem != NULL && *mem != NULL && (*mem)->seg_mem != NULL
               && (*mem)->unmapped != NULL);

        /* remove every virtual memory segment */  
        for(int i = 0; i < Seq_length((*mem)->seg_mem); i++){
                if (Seq_at((*mem)->seg_mem, i) != NULL) {
                        UArray_free(&((*mem)->seg_mem));
                }
        }

        /* remove the two sequences and the entire memory struct */  
        Seq_free(&(*mem)->seg_mem);
        Seq_free(&(*mem)->unmapped);
        free(mem);
}