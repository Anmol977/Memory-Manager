//
// Created by anmol and shivam and agrim on 3/12/22.
//

#include "fast_malloc.h"

fast_malloc::fast_malloc() {
    mem_heap = (char *) malloc(MAX_HEAP);
    mem_brk = (char *) mem_heap;
    heap_listp = (char *) mem_heap;
    mem_max_addr = (char *) mem_heap + MAX_HEAP;
    logger = new Logger();
    if (init_mem_list() == -1) {
        logger->print_error(error_strings::INITIALIZATION_ERROR);
        exit(-1);
    }
}

int fast_malloc::init_mem_list() {
    if ((heap_listp = (char *) fast_sbrk(4 * WSIZE)) == (void *) -1) {
        return -1;
    }
    PUT(heap_listp, 0);
    PUT(heap_listp + (1 * WSIZE), PACK_INFO(0, 1));
    PUT(heap_listp + (2 * WSIZE), PACK_INFO(0, 1));
    if ((heap_listp = (char *) extend_heap(CHUNKSIZE / WSIZE)) == nullptr)
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

    if (size > DSIZE) {
        PUT(HEADER_PTR(bp), PACK_INFO(size - DSIZE, 0));
        PUT(FOOTER_PTR(bp), PACK_INFO(size - DSIZE, 0));
    } else {
        PUT(HEADER_PTR(bp), PACK_INFO(size, 0));
        PUT(FOOTER_PTR(bp), PACK_INFO(size, 0));
    }
    PUT(HEADER_PTR(NEXT_BLK_PTR(bp)), PACK_INFO(0, 1));

//    ig no need to call coalesce on each extend
//    coalesce_block(bp);
    return (void *) bp;
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

void fast_malloc::fast_free(void *block_ptr) {
    std::size_t curr_size = GET_BLOCK_SIZE(HEADER_PTR(block_ptr));
    PUT(HEADER_PTR(block_ptr), PACK_INFO(curr_size, 0));
    PUT(FOOTER_PTR(block_ptr), PACK_INFO(curr_size, 0));
    buddy_map[curr_size].push_back(block_ptr);
//    coalesce_block(block_ptr);
}

void *fast_malloc::fast_coalesce() {
    for (char *i = heap_listp; i < mem_brk; i += DSIZE + GET_BLOCK_SIZE(HEADER_PTR(i))) {
        coalesce_block(i);
    }
    return nullptr;
}

void *fast_malloc::coalesce_block(void *block_ptr) {
    if (GET_BLOCK_ALLOC(HEADER_PTR(block_ptr))) {
        return nullptr;
    }

    std::size_t curr_size = GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) + DSIZE;
    bool is_prev_free = !GET_BLOCK_ALLOC(HEADER_PTR(PREV_BLK_PTR(block_ptr)));
    bool is_next_free = !GET_BLOCK_ALLOC(HEADER_PTR(NEXT_BLK_PTR(block_ptr)));

    if (!is_prev_free and !is_next_free) {
        logger->print_info(info_strings::NO_COALESCE_EVENT);
        return block_ptr;
    }
    if (is_prev_free and !is_next_free) {
        curr_size += GET_BLOCK_SIZE(HEADER_PTR(PREV_BLK_PTR(block_ptr)));
        PUT(HEADER_PTR(PREV_BLK_PTR(block_ptr)), PACK_INFO(curr_size, 0));
        PUT(FOOTER_PTR(block_ptr), PACK_INFO(curr_size, 0));
        return PREV_BLK_PTR(block_ptr);
    }
    if (!is_prev_free and is_next_free) {
        curr_size += GET_BLOCK_SIZE(HEADER_PTR(NEXT_BLK_PTR(block_ptr)));
        PUT(HEADER_PTR(block_ptr), PACK_INFO(curr_size, 0));
        PUT(FOOTER_PTR(NEXT_BLK_PTR(block_ptr)), PACK_INFO(curr_size, 0));
        return block_ptr;
    }
    if (is_prev_free and is_next_free) {
        curr_size += GET_BLOCK_SIZE(HEADER_PTR(NEXT_BLK_PTR(block_ptr))) +
                     GET_BLOCK_SIZE(HEADER_PTR(PREV_BLK_PTR(block_ptr)));
        PUT(HEADER_PTR(PREV_BLK_PTR(block_ptr)), PACK_INFO(curr_size, 0));
        PUT(FOOTER_PTR(NEXT_BLK_PTR(block_ptr)), PACK_INFO(curr_size, 0));
        return PREV_BLK_PTR(block_ptr);
    }
}

void *fast_malloc::mem_malloc(std::size_t size) {
    if (size == 0) {
        logger->print_error(error_strings::INVALID_ALLOCATION_PARAMETER);
        return nullptr;
    }
    void *block_ptr = nullptr;
    std::size_t adj_size = size;

    if (size <= DSIZE) {
        adj_size = 2 * DSIZE;
    } else {
        adj_size = std::ceil(((float) size / (float) DSIZE) + 1) * DSIZE;
    }

    if ((block_ptr = (char *) fast_allocate(adj_size)) != nullptr) {
        allocate_block(adj_size, block_ptr);
        return block_ptr;
    }

    std::size_t extend_size = std::max(adj_size, (unsigned long) CHUNKSIZE);

    if ((block_ptr = (char *) extend_heap(extend_size / WSIZE)) == nullptr) {
        logger->print_error(error_strings::NO_HEAP_EXTEND);
        return nullptr;
    }
    allocate_block(adj_size, block_ptr);
    return block_ptr;
}

void *fast_malloc::fast_find_fit(std::size_t size) {
    int min_diff = INT16_MAX;
    char *curr_min_ptr = nullptr;
    for (char *i = heap_listp; i < mem_brk; i = NEXT_BLK_PTR(i)) {
        if (!GET_BLOCK_ALLOC(HEADER_PTR(i))) {
            unsigned int curr_size = GET_BLOCK_SIZE(HEADER_PTR(i));
            if (curr_size - size < min_diff) {
                min_diff = curr_size - size;
                curr_min_ptr = i;
            }
        }
    }
    return (void *) curr_min_ptr;
}

void fast_malloc::allocate_block(std::size_t size, void *block_ptr) {
    PUT(HEADER_PTR(block_ptr), PACK_INFO(size - DSIZE, 1));
    PUT(FOOTER_PTR(block_ptr), PACK_INFO(size - DSIZE, 1));
}

void *fast_malloc::fast_allocate(std::size_t size) {
    void *mem_ptr = fast_find_fit(size);
    return mem_ptr;
}

void fast_malloc::print_block_info(void *block_ptr) {
    if (!block_ptr) {
        return;
    }
    std::cout << std::endl << "SIZE:\t\t" << GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) << std::endl;
    std::cout << "PREV:\t\t" << (void *) (PREV_BLK_PTR(block_ptr)) << std::endl;
    std::cout << "HEADER:\t\t" << (void *) HEADER_PTR(block_ptr) << std::endl;
    std::cout << "BLOCK:\t\t" << (void *) block_ptr << std::endl;
    std::cout << "FOOTER:\t\t" << (void *) FOOTER_PTR(block_ptr) << std::endl;
    std::cout << "NEXT:\t\t" << (void *) (NEXT_BLK_PTR(block_ptr)) << std::endl;
}

void fast_malloc::print_buddies() {
    std::cout << std::endl;
    for (auto buddy: buddy_map) {
        std::cout << buddy.first << " : ";
        for (auto mem:buddy.second) {
            std::cout << mem << " -> ";
        }
        std::cout << std::endl;
    }
}
