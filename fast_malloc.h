//
// Created by anmol on 3/12/22.
//

#ifndef MEMALLOCATOR_FAST_MALLOC_H
#define MEMALLOCATOR_FAST_MALLOC_H

#ifdef DEBUG
#include "logger.cpp"
#include "error_strings.h"
#endif

#include "list.h"
#include <iostream>
#include <list>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <map>

#define MAX_HEAP (10 * 1024)
#define WSIZE 4 // in bytes
#define DSIZE 8 // in bytes
#define CHUNKSIZE (1<<12)

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

// get pointerqqs to header or footer from block pointer
#define HEADER_PTR(bp) ((char *)(bp) - WSIZE)
#define FOOTER_PTR(bp) ((char *)(bp) + GET_BLOCK_SIZE(HEADER_PTR(bp)))

#define NEXT_BLOCK_HEADER(bp) ((char *)FOOTER_PTR(bp) + WSIZE)

#define NEXT_BLK_PTR(bp) (((char *)bp) + DSIZE + GET_BLOCK_SIZE(HEADER_PTR(bp)))
#define PREV_BLK_PTR(bp) (((char *)bp) - DSIZE - GET_BLOCK_SIZE(((char *)bp) - DSIZE))

class fast_malloc {
private:
    char *m_memHeap; // ptr to first byte of the heap
    char *m_memBrk; // ptr to one beyond last byte of heap
    char *m_memUnallocAddr;
    char *m_memMaxAddr; // max legal heap address plus 1
#ifdef DEBUG
    Logger *logger;
#endif
    char *m_heapListp; // ptr to first usable block
    List m_coalesceBatch; //queue of address for deferred coalescing
#ifdef SEG_LIST
    std::map<int, List> buddy_map; // segregated list
#endif

#if defined FIRST_FIT || defined BEST_FIT || defined WORST_FIT || defined NEXT_FIT 
    char *rover;
    std::list<void *> m_freeList;
#endif

    inline void *fast_sbrk(int );

    int init_mem_list();

    void *fast_find_fit(std::size_t );

    void *extend_heap(std::size_t );

    inline void allocate_block(std::size_t , void *);

    inline void split_block(std::size_t , void *);

    void *coalesce_block(void *);

    void print_block_info(void *);

    void run_rover();

    void* alloc_on_heap(size_t);

public:
    void print_buddies();

    fast_malloc();

    void fast_free(void *);

    void *mem_malloc(std::size_t );

    void print_heap();
};


#endif //MEMALLOCATOR_FAST_MALLOC_H
