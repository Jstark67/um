#include "umemory.h"

typedef struct Mem_T {
        Seq_T seg_mem;
        Seq_T unmapped;
        uint32_t maxID;
} *Mem_T