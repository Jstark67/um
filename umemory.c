/**************************************************************
 *
 *                     umemory.c
 *
 *     Assignment: HW6 um
 *     Authors:  David Chen and Sam Hu
 *     Date:  Nov 20th
 *
 *     summary:
 *      
 *     umemory.c contains functions that handle all operations involving
 *     the virtual memory system of the universal machine; clients are 
 *     allowed to allocate/dealocate memory through map/unmap and 
 *     fetch/store information through store/load. 
 *
 **************************************************************/
#include "umemory.h"

#define unit_size 4
#define HINT 0

/* mem_init
 * Initialize a Mem_T object and populate m[0] through the inputted program 
 * instructions
 * Params: Uarray_T of uint32_ts: program instructions
 * Return: 
 *      - a Mem_T object, with m[0] substantiated
 * Expectation: 
 *      - CRE will occur if the initialization of Hanson sequences
 *        failed, which would be handled by Hanson’s exception mechanisms
 *      - CRE will occur if the input is NULL
*/
Mem_T mem_init(UArray_T program)
{       
        /* exit of input instructions are null */
        assert(program != NULL);

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
 *      - Mem_T *mem: pointer of the memory structure
 * Return: void
 * Expectation: CRE if the memory, the address of the memory or the memory
 * segments are null
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

/* mem_map
 * Map a memory segment with designated length and return its index
 * Params: 
 *      - uint32_t length: length of the segment to be mapped
 *      - Mem_T mem: the working memory structure
 * Return: uint32_t length
 * Expectation: 
 *      - CRE if the input length is not greater than 0
 *      - CRE if UArray initialization failed
 *      - CRE if mem or seg_mem or unmapped is NULL
 * segments are null
*/
uint32_t mem_map(uint32_t length, Mem_T mem)
{
        /* invalid input */
        assert(length > 0 && mem != NULL && mem->seg_mem != NULL
               && mem->unmapped != NULL);

        /* initialize UArray */
        UArray_T Segment = UArray_new(length, unit_size);
        assert(Segment != NULL);

        /* initiate a Hanson sequence that stores uint32_t unmapped indices */
        int unmapped_num = Seq_length(mem->unmapped);
        
        /* map the memory, assign index based on previously unmmaped slots */
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

/* mem_unmap
 * Free a previously mapped memory segment
 * Params: 
 *      - uint32_t idx: index of the segment to be unmapped
 *      - Mem_T mem: the working memory structure
 * Return: void
 * Expectation: 
 *      - CRE if the segment was not mapped
 *      - CRE if mem or seg_mem or unmapped is NULL
 *      - CRE if index is invalid
*/
void mem_unmap(Mem_T mem, uint32_t idx)
{
        /* invalid inputs */
        assert(mem != NULL && mem->seg_mem != NULL &&
               mem->unmapped != NULL && idx > 0);
        UArray_T unmapped_seg = Seq_get(mem->seg_mem, idx);
        assert(unmapped_seg != NULL);

        /* append the current index to the ummaped sequence */
        Seq_addhi(mem->unmapped, (void *)(uintptr_t)idx);
        
        /* replace the segment with NULL and free the segment */
        Seq_put(mem->seg_mem, idx, NULL);
        assert(Seq_get(mem->seg_mem, idx) == NULL);
        UArray_free(&unmapped_seg);
}


/* mem_load
 * Return the uint32_t stored at a desiganted memory address
 * Params: 
 *      - uint32_t memi: index of memory segments
 *      - uint32_t segi: offset within a segment
 *      - Mem_T mem: the working memory structure
 * Return: void
 * Expectation: 
 *      - CRE if mem or seg_mem is NULL
 *      - CRE if address is invalid (handled by Hanson)
*/
uint32_t mem_load(Mem_T mem, uint32_t memi, uint32_t segi)
{
        assert(mem != NULL && mem->seg_mem != NULL);
        UArray_T segment = Seq_get(mem->seg_mem, memi);
        uint32_t *value = UArray_at(segment, segi);
        return *value;
}

/* mem_store
 * Store a uint32_t value into a designated memory address
 * Params: 
 *      - uint32_t memi: index of memory segments
 *      - uint32_t segi: offset within a segment
 *      - Mem_T mem: the working memory structure
 *      - uint32_t value: the value we want to store
 * Return: void
 * Expectation: 
 *      - CRE if mem or seg_mem is NULL
 *      - CRE if address is invalid (handled by Hanson)
*/
void mem_store(Mem_T mem, uint32_t memi, uint32_t segi, uint32_t value)
{
        assert(mem != NULL && mem->seg_mem != NULL);
        UArray_T segment = Seq_get(mem->seg_mem, memi);
        *(uint32_t *)UArray_at(segment, segi) = value;
}


/* mem_loadP
 * Load a segment into $m[0] as the program instructions
 * Params: 
 *      - uint32_t index: index of memory segments
 *      - Mem_T mem: the working memory structure
 * Return: uint32_t length: the length of the 
 * Expectation: 
 *      - CRE if mem or seg->mem is NULL
*/
uint32_t mem_loadP(Mem_T mem, uint32_t idx)
{
        /* invalid input */
        assert(mem != NULL && mem->seg_mem != NULL);

        /* fetch target segments */
        UArray_T old = Seq_get(mem->seg_mem, 0);
        UArray_T new = Seq_get(mem->seg_mem, idx);
        uint32_t length = UArray_length(new);
        
        /* free the original $m[0] */
        UArray_free(&old);
        
        /* load the new segment into $m[0] */
        Seq_put(mem->seg_mem, 0, UArray_copy(new, length));
        return length;
}

/* mem_inst
 * Fetch program instructions stored at $m[0]
 * Params: 
 *      - int line: index of memory segments
 *      - Mem_T mem: the working memory structure
 * Return: uint32_t instruction: the instruction stored at the position
 * Expectation: 
 *      - CRE if mem or seg_mem is NULL
*/
uint32_t mem_inst(Mem_T mem, int line)
{
        assert(mem != NULL && mem->seg_mem != NULL);
        return *(uint32_t *)UArray_at(Seq_get(mem->seg_mem, 0), line);
}

