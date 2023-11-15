#ifndef UMEM_H
#define UMEM_H

#include <seq.h>
#include <uarray.h>
#include <assert.h>
#include <stdio.h>
#include <stdint.h>

void printB(){
        printf("hello umemory\n");
}
typedef struct Mem_T *Mem_T;

Mem_T mem_init(UArray_T);


#endif