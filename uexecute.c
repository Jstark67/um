#include "uexecute.h"
#include "umemory.h"
void move(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        if(*rc != 0){
                *ra = *rb;
        }
}
void segL(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void segS(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void add(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        *ra = *rb + *rc;
}
void mult(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        *ra = *rb * *rc;
}
void div(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        *ra = *rb / *rc;
}
void nand(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        *ra = ~(*rb & *rc);
}
void halt(Mem_T *mem)
{
        mem_free(mem);
}
void map(Mem_T mem, uint32_t *rb, uint32_t *rc)
{
        *rb = mem_map(*rc, mem);
}
void unmap(Mem_T mem, uint32_t *rc)
{
        mem_unmap(mem, *rc);
}
void out(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        fputc((int)*rc,stdout);
}
void in(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        *rc = fgetc(stdin);
}
void loadP(uint32_t *ra, uint32_t *rb, uint32_t *rc);
void LV(uint32_t value, uint32_t *rc)
{
        *rc = value;
}