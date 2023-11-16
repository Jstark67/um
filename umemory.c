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
        printf("fuck1\n");

        /* remove every virtual memory segment */  
        for(int i = 0; i < Seq_length((*mem)->seg_mem); i++){
                UArray_T cur = Seq_get((*mem)->seg_mem, i);
                if (cur != NULL) {
                        UArray_free(&cur);
                }
        }
        printf("fuck2\n");
        /* remove the two sequences and the entire memory struct */  
        Seq_free(&(*mem)->seg_mem);
        Seq_free(&(*mem)->unmapped);
        printf("fuck3\n");
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

/* Tests the basic functionalities of map and unmap, will require more tests since this is just a prototype */
void umemory_map_unmap_test(Mem_T mem)
{
        /* check whether mapping returns consecutive ids and allocated correct space*/
        uint32_t id;
        uint32_t size = 64; /* random number to test with */
        printf("work1\n");
        for (unsigned i = 0; i < 1000; i++) {
                id = mem_map(size, mem);
                assert(id == i + 1);
                assert(UArray_length(Seq_get(mem->seg_mem, id)) == (int)size);
        }
        printf("work2\n");
        /* call unmap with an invalid id */
        // uint32_t unmap_id = 114514;
        
        /* unmap a segment in the middle and call map again, check whether the two segment indices are the same */

        for (uint32_t i = 250; i < 500; i++) {
                printf("work3\n");
                mem_unmap(mem, i);
                id = mem_map(size, mem);
                printf("index 1: %u, index 2: %u\n", i, id);
                
                assert(i == id);
        }
        
        
        // mem_unmap(mem, unmap_id); /* Should fail the program */
}
