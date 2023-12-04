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
        /* exit of input instructions are null */
        assert(program != NULL);

        /* allocate host memory for the virtual memory */
        Mem_T mem = malloc(32);
        assert(mem != NULL);

        /* allocate host memory for the two sequences */
        mem->capacity = 10;
        mem->size = 1;
        mem->ucap =10;
        mem->usize = 0;
        mem->seg_mem = malloc(mem->capacity * sizeof(struct Memory));
        mem->unmapped = malloc(mem->ucap * 4);
        
        /* add the program instructions to $m[0] */
        int length = UArray_length(program);
        mem->seg_mem[0].mem = malloc(length * sizeof(uint32_t));
        mem->seg_mem[0].length= length;
        for (int i = 0; i < length; i++) {
                mem->seg_mem[0].mem[i] = *(uint32_t *)UArray_at(program, i);
        }

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
        /* assure that virtual memory was allocated before freeing */
        //assert(mem != NULL && *mem != NULL && (*mem)->seg_mem != NULL && (*mem)->unmapped != NULL);
        //can't think of a case where called when freed
        /* remove every virtual memory segment */  
        for(uint32_t i = 0; i < (*mem)->size; i++) {
                if(!(*mem)->seg_mem[i].mem) free((*mem)->seg_mem[i].mem);
        }

        /* remove the two sequences and the entire memory struct */  
        //free((*mem)->seg_mem);
        free((*mem)->unmapped);
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
        //assert(mem != NULL && mem->seg_mem != NULL && mem->unmapped != NULL);
        if (length == 0){
                if(mem->usize){
                        return mem->unmapped[--mem->usize];
                }else {
                        return mem->size++;
                }
        }
        
        /* initialize UArray */

        uint32_t* Seg = malloc(length * sizeof(uint32_t));
        assert(Seg);


        /* map the memory, assign index based on previously unmmaped slots */
        uint32_t idx;
        if (mem->usize) {
                mem->usize--;
                idx = mem->unmapped[mem->usize];
        } else {
                
                if (mem->size == mem->capacity) {
                        mem->capacity *= 2;
                        Memory newMem = realloc(mem->seg_mem, mem->capacity *  sizeof(struct Memory)); 
                        assert(newMem);
                        
                        mem->seg_mem = newMem;
                }
                idx = mem->size;
                mem->size++;                
        }
        mem->seg_mem[idx].mem = Seg;
        mem->seg_mem[idx].length = length;
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
        //assert( mem->unmapped != NULL && idx > 0);


        /* append the current index to the ummaped sequence */
        if (mem->usize == mem->ucap) {
                mem->ucap *= 2;
                uint32_t *new_unmapped= realloc(mem->unmapped,mem->ucap * sizeof(uint32_t));
                assert(new_unmapped);

                
                
                mem->unmapped = new_unmapped;
        }
        
        /* replace the segment with NULL and free the segment */
        mem->unmapped[mem->usize] = idx;
        mem->usize++;

        free(mem->seg_mem[idx].mem);
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
        //assert(mem->seg_mem != NULL && memi < mem->size);
        //printf("value: %u\n",mem->seg_mem[memi].mem[segi]);
        return mem->seg_mem[memi].mem[segi];
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
        //assert(mem != NULL && mem->seg_mem != NULL && memi < mem->size);
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
uint32_t mem_loadP(Mem_T mem, uint32_t idx)
{
        /* invalid input */
        //assert(mem != NULL && mem->seg_mem != NULL);

        /* fetch target segments */
        // UArray_T old = Seq_get(mem->seg_mem, 0);
        // UArray_T new = Seq_get(mem->seg_mem, idx);
        // uint32_t length = UArray_length(new);
        
        
        /* free the original $m[0] */
        //UArray_free(&old);
        free(mem->seg_mem[0].mem);

        int length = mem->seg_mem[idx].length;
        uint32_t *M = mem->seg_mem[0].mem, *N = mem->seg_mem[idx].mem;
        M = malloc(sizeof(uint32_t)*length);
        for (int i = 0; i < length; i++) {M[i] = N[i];}

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
        //assert(mem != NULL && mem->seg_mem != NULL);
        return mem->seg_mem[0].mem[line];
}

