#include <seq.h>
#include <uarray.h>
#include <assert.h>
#include <stdio.h>
#include "umemory.h"
#include <stdlib.h>

#define unit_size 4
#define HINT 0


// struct Mem_T {
//         Seq_T seg_mem;
//         Seq_T unmapped;
//         uint32_t maxID;
// };

/* mem_init
 * Initialize a Mem_T object and populate m[0] through the inputted program instructions
 * Params: Uarray_T of uint32_ts that stores program instructions
 * Return: 
 *      - a Mem_T object, with m[0] substantiated
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

        return mem;
}

/* mem_free
 * Free a Mem_T struct and all its segments
 * Params: 
 *      - address of the mem_T object to be freed 
 * Return: a Mem_T object, with m[0] substantiated
 * Expectation: CRE if address or segments itself is null
*/
void mem_free(Mem_T *mem)
{       
        /* assure thatt virtual memory was allocated before freeing */
        assert(mem != NULL && *mem != NULL && (*mem)->seg_mem != NULL
               && (*mem)->unmapped != NULL);
        /* remove every virtual memory segment */  
        for(int i = 0; i < Seq_length((*mem)->seg_mem); i++){
                UArray_T cur = Seq_get((*mem)->seg_mem, i);
                if (cur != NULL) {
                        UArray_free(&cur);
                }
        }
        /* remove the two sequences and the entire memory struct */  
        Seq_free(&(*mem)->seg_mem);
        Seq_free(&(*mem)->unmapped);
        free(*mem);
    
}


uint32_t mem_map(uint32_t length, Mem_T mem)
{
        UArray_T Segment = UArray_new(length, unit_size);
        int unmapped_num = Seq_length(mem->unmapped);
        uint32_t idx;
        if (unmapped_num == 0) {
                Seq_addhi(mem->seg_mem, Segment);
                idx = Seq_length(mem->seg_mem) - 1;
        } else {
                idx = (uint32_t)(uintptr_t)Seq_get(mem->unmapped, 0);
                
                assert(Seq_get(mem->seg_mem, idx) == NULL);
                Seq_remlo(mem->unmapped);
                Seq_put(mem->seg_mem, idx, Segment);
        }

        return idx;
}
void mem_unmap(Mem_T mem, uint32_t idx)
{
        Seq_addhi(mem->unmapped, (void *)(uintptr_t)idx);
        assert(mem != NULL);
        UArray_T unmapped_seg = Seq_get(mem->seg_mem, idx);
        assert(unmapped_seg != NULL);
        Seq_put(mem->seg_mem, idx, NULL);
        assert(Seq_get(mem->seg_mem, idx) == NULL);
        UArray_free(&unmapped_seg);
}



uint32_t mem_load(Mem_T mem, uint32_t memi, uint32_t segi)
{
        UArray_T segment = Seq_get(mem->seg_mem, memi);
        return (uint32_t)(uintptr_t)UArray_at(segment, segi);
}


void mem_store(Mem_T mem, uint32_t memi, uint32_t segi,uint32_t value)
{
        UArray_T segment = Seq_get(mem->seg_mem, memi);
        *(uint32_t *)UArray_at(segment, segi) = value;
}



int mem_loadP(Mem_T mem, uint32_t idx)
{
        UArray_T old = Seq_get(mem->seg_mem, 0);
        UArray_T new = Seq_get(mem->seg_mem, idx);
        uint32_t length = UArray_length(new);
        UArray_free(&old);
        Seq_put(mem->seg_mem, 0, UArray_copy(new, length));
        return length;
}

uint32_t mem_inst(Mem_T mem, int line)
{
        return (uint32_t)(uintptr_t)UArray_at(Seq_get(mem->seg_mem, 0), line);
}