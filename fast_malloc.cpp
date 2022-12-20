//
// Created by anmol and shivam on 3/12/22.
//

#include "fast_malloc.h"

fast_malloc::fast_malloc() {
    mem_heap = (char *) malloc(MAX_HEAP);
    mem_brk = (char *) mem_heap;
    mem_max_addr = (char *) mem_heap + MAX_HEAP;
    logger = new Logger();
    init_mem_list();
}

void *fast_malloc::fast_sbrk(int incr_amt) {
    char *prev_brk = mem_brk;

    if (incr_amt < 0 or (mem_brk + incr_amt) >= mem_max_addr) {
        logger->print_error(error_strings::INSUFFICIENT_MEMORY);
        return (void *) -1;
    }
    mem_brk += incr_amt;
    return (void *) prev_brk;
}

int fast_malloc::init_mem_list() {
    if ((heap_listp = (char *) fast_sbrk(4 * WSIZE)) == (void *) -1) {
        return -1;
    }
    PUT(heap_listp, 0);
    PUT(heap_listp + (1 * WSIZE), PACK_INFO(DSIZE, 1));
    PUT(heap_listp + (2 * WSIZE), PACK_INFO(DSIZE, 1));
    PUT(heap_listp + (3 * WSIZE), PACK_INFO(0, 1));
    heap_listp = (char *) heap_listp + (2 * WSIZE);
    if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
        return -1;
    return 0;
}

void *fast_malloc::extend_heap(std::size_t words) {
    char *bp;
    std::size_t size;
    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;

    if ((long) (bp = (char *) fast_sbrk(size)) == -1) {
        return nullptr;
    }
    PUT(HEADER_PTR(bp), PACK_INFO(size, 0));
    PUT(FOOTER_PTR(bp), PACK_INFO(size, 0));
    PUT(HEADER_PTR(NEXT_BLK_PTR(bp)), PACK_INFO(size, 1));

    fast_coalesce(bp);
    return (void *) 1;
}

void fast_malloc::fast_free(void *block_ptr) {
    std::size_t curr_size = GET_BLOCK_SIZE(HEADER_PTR(block_ptr));
    PUT(HEADER_PTR(block_ptr), PACK_INFO(curr_size, 0));
    PUT(FOOTER_PTR(block_ptr), PACK_INFO(curr_size, 0));
    fast_coalesce(block_ptr);
}

void *fast_malloc::fast_coalesce(void *block_ptr) {
    std::size_t curr_size = GET_BLOCK_SIZE(HEADER_PTR(block_ptr));
    bool is_prev_free = GET_BLOCK_ALLOC(PREV_BLK_PTR(block_ptr));
    bool is_next_free = GET_BLOCK_ALLOC(NEXT_BLK_PTR(block_ptr));

    if (!is_next_free and !is_prev_free) {
        logger->print_info(info_strings::NO_COALESCE_EVENT);
        return block_ptr;
    }
    if (is_prev_free and !is_next_free) {
        curr_size += GET_BLOCK_SIZE(PREV_BLK_PTR(block_ptr));
        PUT(HEADER_PTR(PREV_BLK_PTR(block_ptr)), PACK_INFO(curr_size, 0));
        PUT(FOOTER_PTR(block_ptr), PACK_INFO(curr_size, 0));
    }
    if (!is_prev_free and is_next_free) {
        curr_size += GET_BLOCK_SIZE(NEXT_BLK_PTR(block_ptr));
        PUT(HEADER_PTR(block_ptr), PACK_INFO(curr_size, 0));
        PUT(FOOTER_PTR(NEXT_BLK_PTR(block_ptr)), PACK_INFO(curr_size, 0));
    }
    if (is_prev_free and is_next_free) {
        curr_size += GET_BLOCK_SIZE(NEXT_BLK_PTR(block_ptr)) + GET_BLOCK_SIZE(PREV_BLK_PTR(block_ptr));
        PUT(HEADER_PTR(PREV_BLK_PTR(block_ptr)), PACK_INFO(curr_size, 0));
        PUT(FOOTER_PTR(NEXT_BLK_PTR(block_ptr)), PACK_INFO(curr_size, 0));
    }
    return block_ptr;
}

void *fast_malloc::mem_malloc(std::size_t size) {
    if (size == 0) {
        logger->print_error(error_strings::INVALID_ALLOCATION_PARAMETER);
        return nullptr;
    }
    char *block_ptr;
    std::size_t adj_size;

    if (size <= DSIZE) {
        adj_size = 2 * DSIZE;
    } else {
        adj_size = DSIZE * ((size) + DSIZE + (DSIZE - 1) / DSIZE);
    }

    if ((block_ptr = (char *) fast_find_fit(adj_size)) != nullptr) {
        //allocate;
        return block_ptr;
    }

    std::size_t extend_size = std::max(adj_size, (unsigned long) CHUNKSIZE);

    if ((block_ptr = (char *) extend_heap(extend_size / WSIZE)) == nullptr) {
        return nullptr;
    }
    //allocate
    return block_ptr;
}

void *fast_malloc::fast_find_fit(std::size_t size) {
    return (void *) 1;
}

