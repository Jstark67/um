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


Mem_T mem_init(UArray_T program)
{       
        

        Mem_T mem = malloc(32); assert(mem);mem->capacity = 10; mem->size = 1; mem->ucap = 10;mem->usize = 0; mem->seg_mem = malloc(mem->capacity * 16);mem->unmapped = malloc(mem->ucap * 4);int length = UArray_length(program);mem->seg_mem[0].mem = malloc(length * 4);mem->seg_mem[0].length= length;for (int i = 0; i < length; i++) {mem->seg_mem[0].mem[i] = *(uint32_t *)UArray_at(program, i);}UArray_free(&program);return mem;
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
        for(uint32_t i = 0; i < (*mem)->size; i++) {
                if((*mem)->seg_mem[i].mem) {free((*mem)->seg_mem[i].mem); (*mem)->seg_mem[i].mem = NULL;}
        }
        free((*mem)->seg_mem);free((*mem)->unmapped);free(*mem);
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

        
        return mem->seg_mem[memi].mem[segi];
}

/* mem_store
 * Store a uint32_t value into a designated memorvalgrind --leak-check=full --track-origin=yes ./um ./umbin/midmark.um address
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
        mem->seg_mem[memi].mem[segi] = value;
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
        return mem->seg_mem[0].mem[line];
}

