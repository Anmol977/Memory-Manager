//
// Created by anmol on 3/12/22.
//

#ifndef MEMALLOCATOR_FAST_MALLOC_H
#define MEMALLOCATOR_FAST_MALLOC_H

#include <cstdlib>
#include "error_strings.h"
#include "logger.cpp"

#define MAX_HEAP 1 * 512
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1>>12)

// pack header of a memory block
#define ADD_HEADER(SIZE, ISALLOC) ((SIZE) | (ISALLOC))

// to get data of memory block using a pointer
#define GET(p) (*(unsigned int *)(p))
// update data of memory block using pointer
#define PUT(p, val) (*(unsigned int *)(p) = (val))

// self-explanatory, p is a pointer to memory block
#define GET_HEADER_SIZE(p) (GET(p) & ~0x7)
#define GET_HEADER_ALLOC(p) (GET(p) & 0x1)

// get pointers to header or footer from block pointer
#define HEADER_PTR(bp) ((char *)(bp) - WSIZE)
#define FOOTER_PTR(cp) ((char *)(bp) + GET_SIZE(HDRP(bp)))

#define NEXT_BLK_PTR(bp) ((char *)(bp) + GET_HEADER_SIZE(HEADER_PTR(bp)))
#define PREV_BLK_PTR(bp) ((char *)(bp) - DSIZE - GET_HEADER_SIZE((char *)(bp) - DSIZE))


class fast_malloc {
    char *mem_heap; // ptr to first byte of the heap
    char *mem_brk; // ptr to one beyond last byte of heap
    char *mem_max_addr; // max legal heap address plus 1
    Logger *logger;

    fast_malloc();

    void *fast_sbrk(int incr_amt);

    int init_mem_list();
};


#endif //MEMALLOCATOR_FAST_MALLOC_H
