//
// Created by anmol on 3/12/22.
//

#ifndef MEMALLOCATOR_FAST_MALLOC_H
#define MEMALLOCATOR_FAST_MALLOC_H

#ifdef DEBUG
#include "logger.cpp"
#include "error_strings.h"
#endif
#include <iostream>
#include <list>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <map>

#define MAX_HEAP (16 * 5 * 1024)
#define WSIZE 4 // in bytes
#define DSIZE 8 // in bytes
#define CHUNKSIZE (1>>12)

//some helper macros
#define MAX(a, b) a > b? a : b
#define CEILING(X) ((X-(int)(X)) > 0 ? (int)(X+1) : (int)(X))

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

#define NEXT_BLK_PTR(bp) (((char *)bp) + DSIZE + GET_BLOCK_SIZE(HEADER_PTR(bp)))
#define PREV_BLK_PTR(bp) (((char *)bp) - DSIZE - GET_BLOCK_SIZE(((char *)bp) - DSIZE))

class fast_malloc {
private:
    char *mem_heap; // ptr to first byte of the heap
    char *mem_brk; // ptr to one beyond last byte of heap
    char *mem_max_addr; // max legal heap address plus 1
#ifdef DEBUG
    Logger *logger;
#endif
    char *heap_listp; // ptr to first usable block
    std::list<void *> coalesce_batch; //queue of address for deferred coalescing
#ifdef SEG_LIST
    std::map<int, std::list<void*>> buddy_map; // segregated list
#endif

#if defined FIRST_FIT || defined BEST_FIT || defined WORST_FIT || defined NEXT_FIT 
    char *rover;
    std::list<void *> free_list;
#endif

    inline void *fast_sbrk(int incr_amt);

    int init_mem_list();

    void *fast_find_fit(std::size_t size);

    void *extend_heap(std::size_t words);

    inline void allocate_block(std::size_t size, void *block_ptr);

    inline void split_block(std::size_t size, void *block_ptr);

    void *coalesce_block(void *block_ptr);

    void print_block_info(void *block_ptr);

    void run_rover();

public:
    void print_buddies();

    fast_malloc();

    void fast_free(void *block_ptr);

    void *mem_malloc(std::size_t size);

    void print_heap();
};


#endif //MEMALLOCATOR_FAST_MALLOC_H
