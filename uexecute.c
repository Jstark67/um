#include "uexecute.h"


void move(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        if(*rc != 0){
                *ra = *rb;
        }
}
void segL(Mem_T mem, uint32_t *ra, uint32_t *rb, uint32_t *rc){
        *ra = mem_load(mem,*rb,*rc);
}
void segS(Mem_T mem,uint32_t *ra, uint32_t *rb, uint32_t *rc){
        mem_store(mem,*ra,*rb,*rc);
}
void add(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        *ra = *rb + *rc;
}
void mult(uint32_t *ra, uint32_t *rb, uint32_t *rc)
{
        *ra = *rb * *rc;
}
void divide(uint32_t *ra, uint32_t *rb, uint32_t *rc)
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
        exit(0);
}
void map(Mem_T mem, uint32_t *rb, uint32_t *rc)
{
        *rb = mem_map(*rc, mem);
}
void unmap(Mem_T mem, uint32_t *rc)
{
        mem_unmap(mem, *rc);
}
void out(uint32_t *rc)
{
        fputc((int)*rc,stdout);
}
void in(uint32_t *rc)
{
        *rc = fgetc(stdin);
}
int loadP(Mem_T mem, uint32_t *rb)
{
        return mem_loadP(mem,*rb);
}
void lv(uint32_t value, uint32_t *rc)
{
        *rc = value;
}