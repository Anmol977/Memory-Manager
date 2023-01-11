//
// Created by anmol and shivam and agrim on 3/12/22.
//

#include "fast_malloc.h"
#include <cstdlib>

fast_malloc::fast_malloc() {
    mem_heap = (char *) malloc(MAX_HEAP);
    mem_brk = (char *) mem_heap;
    heap_listp = (char *) mem_heap;
    mem_max_addr = (char *) mem_heap + MAX_HEAP;
#ifdef DEBUG
    logger = new Logger();
#endif
    if (init_mem_list() == -1) {
#ifdef DEBUG
        logger->print_error(error_strings::INITIALIZATION_ERROR);
#endif
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
    PUT(heap_listp + (3 * WSIZE), PACK_INFO(0, 1));
    heap_listp += (4 * WSIZE); // added another WSIZE because else heaplistp would be pointing to header of first block
#ifdef FIRST_FIT
    rover = heap_listp;
#endif
    if ((!extend_heap(CHUNKSIZE / WSIZE)))
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
    return (void *) bp;
}

inline void *fast_malloc::fast_sbrk(int incr_amt) {
    char *prev_brk = mem_brk;

    if (incr_amt < 0 or (mem_brk + incr_amt) >= mem_max_addr) {
#ifdef DEBUG
        logger->print_error(error_strings::INSUFFICIENT_MEMORY);
#endif
        return (void *) -1;
    }
    mem_brk += incr_amt;
    return (void *) prev_brk;
}

void fast_malloc::fast_free(void *block_ptr) {
    std::size_t curr_size = GET_BLOCK_SIZE(HEADER_PTR(block_ptr));
    PUT(HEADER_PTR(block_ptr), PACK_INFO(curr_size, 0));
    PUT(FOOTER_PTR(block_ptr), PACK_INFO(curr_size, 0));
#if defined FIRST_FIT || defined BEST_FIT || defined WORST_FIT || defined NEXT_FIT
    block_ptr = coalesce_block(block_ptr);
    free_list.push_back(block_ptr);
#endif
#ifdef SEG_LIST
    block_ptr = coalesce_block(block_ptr);
    buddy_map[GET_BLOCK_SIZE(HEADER_PTR(block_ptr))].push_back(block_ptr);
#endif
    return;
}

void *fast_malloc::coalesce_block(void *block_ptr) {
    std::size_t curr_size = GET_BLOCK_SIZE(HEADER_PTR(block_ptr));
    bool is_prev_free = !GET_BLOCK_ALLOC(HEADER_PTR(PREV_BLK_PTR(block_ptr)));
    bool is_next_free = !GET_BLOCK_ALLOC(HEADER_PTR(NEXT_BLK_PTR(block_ptr)));

    if (is_prev_free and !is_next_free) {
        curr_size += GET_BLOCK_SIZE(HEADER_PTR(PREV_BLK_PTR(block_ptr))) + DSIZE;
        void *new_ptr = PREV_BLK_PTR(block_ptr);
        PUT(HEADER_PTR(PREV_BLK_PTR(block_ptr)), PACK_INFO(curr_size, 0));
        PUT(FOOTER_PTR(block_ptr), PACK_INFO(curr_size, 0));
        return new_ptr;
    }
    if (!is_prev_free and is_next_free) {
        curr_size += GET_BLOCK_SIZE(HEADER_PTR(NEXT_BLK_PTR(block_ptr))) + DSIZE;
        PUT(HEADER_PTR(block_ptr), PACK_INFO(curr_size, 0));
        PUT(FOOTER_PTR(NEXT_BLK_PTR(block_ptr)), PACK_INFO(curr_size, 0));
        return block_ptr;
    }
    if (is_prev_free and is_next_free) {
        curr_size += GET_BLOCK_SIZE(HEADER_PTR(NEXT_BLK_PTR(block_ptr))) + GET_BLOCK_SIZE(HEADER_PTR(PREV_BLK_PTR(block_ptr))) + DSIZE + DSIZE;
        void *new_ptr = PREV_BLK_PTR(block_ptr);
        PUT(HEADER_PTR(PREV_BLK_PTR(block_ptr)), PACK_INFO(curr_size, 0));
        PUT(FOOTER_PTR(NEXT_BLK_PTR(block_ptr)), PACK_INFO(curr_size, 0));
        return new_ptr;
    }
#ifdef DEBUG
	logger->print_info(info_strings::NO_COALESCE_EVENT);
#endif
	return block_ptr;
}

void *fast_malloc::mem_malloc(std::size_t size) {
    if (size == 0) {
#ifdef DEBUG
        logger->print_error(error_strings::INVALID_ALLOCATION_PARAMETER);
#endif
        return nullptr;
    }
    void *block_ptr = nullptr;
    std::size_t adj_size = size;

    if (size <= DSIZE) {
        adj_size = 2 * DSIZE;
    } else {

        adj_size = CEILING(((float) size / (float) DSIZE) + 1) * DSIZE;
    }

    if ((block_ptr = (char *) fast_find_fit(size)) != nullptr) {
#if defined WORST_FIT || defined BEST_FIT
        if (GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) - size >= 16)
            split_block(size, block_ptr);
#endif
        allocate_block(adj_size, block_ptr);
        return block_ptr;
    }

    std::size_t extend_size = MAX(adj_size, (unsigned long) CHUNKSIZE);

    if ((block_ptr = (char *) extend_heap(extend_size / WSIZE)) == nullptr) {
#ifdef DEBUG
        logger->print_error(error_strings::NO_HEAP_EXTEND);
#endif
        return nullptr;
    }
    allocate_block(adj_size, block_ptr);
    return block_ptr;
}

void *fast_malloc::fast_find_fit(std::size_t size) {
#ifdef FIRST_FIT
    for (void *block_ptr : free_list){
        if (!GET_BLOCK_ALLOC(HEADER_PTR(block_ptr)) && (GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) >= size)) {
            return block_ptr;
        }
    }
#endif
#ifdef BEST_FIT
    uint16_t min_size = INT16_MAX;
    void *best_fit_ptr = nullptr;
    for (void *block_ptr: free_list) {
        if (!GET_BLOCK_ALLOC(HEADER_PTR(block_ptr)) && (GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) >= size)) {
            if (GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) - size < min_size) {
                min_size = GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) - size;
                best_fit_ptr = block_ptr;
                if (min_size == 0) return best_fit_ptr;
            }
        }
    }
    return best_fit_ptr;
#endif
#ifdef WORST_FIT
    uint16_t max_diff = 0;
    void *best_fit_ptr = nullptr;
    for (void *block_ptr: free_list) {
        if (!GET_BLOCK_ALLOC(HEADER_PTR(block_ptr)) && (GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) >= size)) {
            if (GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) - size > max_diff) {
                max_diff = GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) - size;
                best_fit_ptr = block_ptr;
            }
        }
    }
    return best_fit_ptr;
#endif
#ifdef SEG_LIST
    if (buddy_map.find(size) != buddy_map.end() and buddy_map[size].size() != 0) {
        void *bp = buddy_map[size].front();
        buddy_map[size].pop_front();
        return bp;
    } else {
        auto it = buddy_map.upper_bound(size);
        if (it == buddy_map.end()) {
            return nullptr;
        } else {
            void *bp = (*it).second.front();
            (*it).second.pop_front();
            return bp;
        }
    }
#endif
    return nullptr;
}

inline void fast_malloc::allocate_block(std::size_t size, void *block_ptr) {

    PUT(HEADER_PTR(block_ptr), PACK_INFO(MAX(size - DSIZE, GET_BLOCK_SIZE(HEADER_PTR(block_ptr))), 1));
    PUT(FOOTER_PTR(block_ptr), PACK_INFO(MAX(size - DSIZE, GET_BLOCK_SIZE(HEADER_PTR(block_ptr))), 1));
}

inline void fast_malloc::split_block(std::size_t size, void *block_ptr) {
    size_t org_block_size = GET_BLOCK_SIZE(HEADER_PTR(block_ptr));
    PUT(HEADER_PTR(block_ptr), PACK_INFO(size, 0));
    PUT(FOOTER_PTR(block_ptr), PACK_INFO(size, 0));
    PUT(HEADER_PTR(NEXT_BLK_PTR(block_ptr)), PACK_INFO(org_block_size - size - DSIZE, 0));
    PUT(FOOTER_PTR(NEXT_BLK_PTR(block_ptr)), PACK_INFO(org_block_size - size - DSIZE, 0));
}

void fast_malloc::print_block_info(void *block_ptr) {
//#ifdef DEBUG

    if (!block_ptr) {
        return;
    }
    std::cout << std::endl << "SIZE:\t\t" << GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) << std::endl;
    std::cout << "STATUS:\t\t" << GET_BLOCK_ALLOC(HEADER_PTR(block_ptr)) << std::endl;
    std::cout << "PREV:\t\t" << (void *) (PREV_BLK_PTR(block_ptr)) << std::endl;
    std::cout << "HEADER:\t\t" << (void *) HEADER_PTR(block_ptr) << std::endl;
    std::cout << "BLOCK:\t\t" << (void *) block_ptr << std::endl;
    std::cout << "FOOTER:\t\t" << (void *) FOOTER_PTR(block_ptr) << std::endl;
    std::cout << "NEXT:\t\t" << (void *) (NEXT_BLK_PTR(block_ptr)) << std::endl;
//#endif
}

void fast_malloc::print_buddies() {
#ifdef SEG_LIST
    std::cout << std::endl;
    for (auto buddy: buddy_map) {
        std::cout << buddy.first << " : ";
        for (auto mem: buddy.second) {
            std::cout << mem << " -> ";
        }
        std::cout << std::endl;
    }
#endif
}

void fast_malloc::print_heap() {
    for (char *trover = heap_listp; GET_BLOCK_SIZE(HEADER_PTR(trover)) > 0; trover = NEXT_BLK_PTR(trover)) {
        print_block_info(trover);
    }
}

void fast_malloc::run_rover() {
#ifdef FIRST_FIT
    char *temp_rover = rover;
    for (; GET_BLOCK_SIZE(HEADER_PTR(temp_rover)) > 0; temp_rover = NEXT_BLK_PTR(temp_rover)) {
        print_block_info(temp_rover);
    }
#endif
}

