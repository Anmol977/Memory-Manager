//
// Created by anmol on 3/12/22.
//

#ifndef MEMALLOCATOR_FAST_MALLOC_H
#define MEMALLOCATOR_FAST_MALLOC_H

#include <cstdlib>
#include <cmath>
#include <algorithm>
#include "error_strings.h"
#include "logger.cpp"

#define MAX_HEAP 1 * 512
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1>>12)

// pack header of a memory block
#define PACK_INFO(SIZE, ISALLOC) ((SIZE) | (ISALLOC))

// to get data of memory block using a pointer
#define GET(p) (*(unsigned int *)(p))
// update data of memory block using pointer
#define PUT(p, val) (*(unsigned int *)(p) = (val))

// self-explanatory, p is a pointer to memory block
#define GET_BLOCK_SIZE(p) (GET(p) & ~0x7)
#define GET_BLOCK_ALLOC(p) (GET(p) & 0x1)

// get pointers to header or footer from block pointer
#define HEADER_PTR(bp) ((char *)(bp) - WSIZE)
#define FOOTER_PTR(bp) ((char *)(bp) + GET_BLOCK_SIZE(HEADER_PTR(bp)))

#define NEXT_BLK_PTR(bp) ((char *)(bp) + DSIZE + GET_BLOCK_SIZE(HEADER_PTR(bp)))
#define PREV_BLK_PTR(bp) ((char *)(bp) - DSIZE - GET_BLOCK_SIZE((char *)(bp) - DSIZE))


class fast_malloc {
private:
    char *mem_heap; // ptr to first byte of the heap
    char *mem_brk; // ptr to one beyond last byte of heap
    char *mem_max_addr; // max legal heap address plus 1
    Logger *logger;
    char *heap_listp;



    void *fast_sbrk(int incr_amt);

    int init_mem_list();

    void *fast_find_fit(std::size_t size);

    void *extend_heap(std::size_t words);

    void *fast_allocate(std::size_t size);

    void allocate_block(std::size_t size, void *block_ptr);
public:
    void *coalesce_block(void *block_ptr);

    void *fast_coalesce();

    fast_malloc();

    void fast_free(void *block_ptr);

    void *mem_malloc(std::size_t size);
};


#endif //MEMALLOCATOR_FAST_MALLOC_H
