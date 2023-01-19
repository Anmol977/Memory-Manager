//
// Created by anmol and shivam and agrim on 3/12/22.
//

#include "MemMalloc.h"
#include <cstdlib>

MemMalloc::MemMalloc() {
    m_memHeap = m_memBrk = m_heapListp = m_memUnallocAddr = (char *) malloc(MAX_HEAP);
    m_memMaxAddr = (char *) m_memHeap + MAX_HEAP;
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

int MemMalloc::init_mem_list() {
    // 16 = 4 * WSIZE in the below line
    if ((m_heapListp = (char *) extend_heap(16)) == (void *) -1) {
        return -1;
    }
    PUT(m_heapListp, 0);
    PUT(m_heapListp + (1 * WSIZE), PACK_INFO(0, 1));
    PUT(m_heapListp + (2 * WSIZE), PACK_INFO(0, 1));
    PUT(m_heapListp + (3 * WSIZE), PACK_INFO(0, 1));
    m_heapListp += (16); // added another WSIZE because else heaplistp would be pointing to header of first block
    m_memUnallocAddr = m_heapListp;
#ifdef FIRST_FIT
    rover = m_heapListp;
#endif
    if ((!extend_heap(CHUNKSIZE / WSIZE)))
        return -1;
    return 0;
}

void *MemMalloc::extend_heap(std::size_t t_size) {
    std::size_t size = (t_size & 0x1) ? (t_size + 1) << 2 : t_size << 2;
    char *prevBrk = m_memBrk;
    if((m_memBrk + size >= m_memMaxAddr)){
        return nullptr;
    }
    m_memBrk +=size;
    return (void *) prevBrk;
}

void *MemMalloc::mem_malloc(std::size_t size) {
    if (size == 0) {
#ifdef DEBUG
        logger->print_error(error_strings::INVALID_ALLOCATION_PARAMETER);
#endif
        return nullptr;
    }

    void *block_ptr = nullptr;
    std::size_t adj_size;
    if (size <= DSIZE) {
        adj_size = 2 * DSIZE;
    } else {
        adj_size = CEILING(((float) size / (float) DSIZE) + 1) * DSIZE;
    }

    if ((m_memUnallocAddr + adj_size) < m_memBrk) {
        block_ptr = m_memUnallocAddr;
        m_memUnallocAddr += adj_size;
        allocate_block(adj_size, block_ptr);
        return block_ptr;
    }

    if ((block_ptr = (char *) mem_find_fit(size)) != nullptr) {
#if defined WORST_FIT || defined BEST_FIT
        if (GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) - size >= 16)
            split_block(size, block_ptr);
#endif
        allocate_block(adj_size, block_ptr);
        return block_ptr;
    }

    std::size_t extend_size = MAX(adj_size, (unsigned long) CHUNKSIZE);
    if ((block_ptr = (char *) extend_heap(extend_size >> 2)) == nullptr) {
#ifdef DEBUG
        logger->print_error(error_strings::NO_HEAP_EXTEND);
#endif
        return nullptr;
    }
    allocate_block(adj_size, block_ptr);
    return block_ptr;
}

void *MemMalloc::mem_find_fit(std::size_t size) {
#ifdef FIRST_FIT
    for (void *block_ptr: m_freeList) {
        if (!GET_BLOCK_ALLOC(HEADER_PTR(block_ptr)) && (GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) >= size)) {
            m_freeList.remove(block_ptr);
            return block_ptr;
        }
    }
#endif
#ifdef BEST_FIT
    uint16_t min_size = INT16_MAX;
    void *best_fit_ptr = nullptr;
    for (void *block_ptr: m_freeList) {
        if (!GET_BLOCK_ALLOC(HEADER_PTR(block_ptr)) && (GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) >= size)) {
            if (GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) - size < min_size) {
                min_size = GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) - size;
                best_fit_ptr = block_ptr;
                if (min_size == 0) return best_fit_ptr;
            }
        }
    }
    m_freeList.remove(best_fit_ptr);
    return best_fit_ptr;
#endif
#ifdef WORST_FIT
    uint16_t max_diff = 0;
    void *best_fit_ptr = nullptr;
    for (void *block_ptr: m_freeList) {
        if (!GET_BLOCK_ALLOC(HEADER_PTR(block_ptr)) && (GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) >= size)) {
            if (GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) - size > max_diff) {
                max_diff = GET_BLOCK_SIZE(HEADER_PTR(block_ptr)) - size;
                best_fit_ptr = block_ptr;
            }
        }
    }
    m_freeList.remove(best_fit_ptr);
    return best_fit_ptr;
#endif
#ifdef SEG_LIST
        auto it = buddy_map.lower_bound(size);
        if (it == buddy_map.end()) {
            return nullptr;
        } else {
            return (*it).second.pop();
        }
#endif
    return nullptr;
}

inline void MemMalloc::allocate_block(std::size_t size, void *block_ptr) {
    PUT(HEADER_PTR(block_ptr), PACK_INFO(MAX(size - DSIZE, GET_BLOCK_SIZE(HEADER_PTR(block_ptr))), 1));
    PUT(FOOTER_PTR(block_ptr), PACK_INFO(MAX(size - DSIZE, GET_BLOCK_SIZE(HEADER_PTR(block_ptr))), 1));
}

inline void MemMalloc::split_block(std::size_t size, void *block_ptr) {
    size_t org_block_size = GET_BLOCK_SIZE(HEADER_PTR(block_ptr));
    PUT(HEADER_PTR(block_ptr), PACK_INFO(size, 0));
    PUT(FOOTER_PTR(block_ptr), PACK_INFO(size, 0));
    PUT(HEADER_PTR(NEXT_BLK_PTR(block_ptr)), PACK_INFO(org_block_size - size - DSIZE, 0));
    PUT(FOOTER_PTR(NEXT_BLK_PTR(block_ptr)), PACK_INFO(org_block_size - size - DSIZE, 0));
}

void MemMalloc::mem_free(void *block_ptr) {
    std::size_t curr_size = GET_BLOCK_SIZE(HEADER_PTR(block_ptr));
    PUT(HEADER_PTR(block_ptr), PACK_INFO(curr_size, 0));
    PUT(FOOTER_PTR(block_ptr), PACK_INFO(curr_size, 0));
#if defined FIRST_FIT || defined BEST_FIT || defined WORST_FIT || defined NEXT_FIT
    block_ptr = coalesce_block(block_ptr);
    m_freeList.push_back(block_ptr);
#endif
#ifdef SEG_LIST
    block_ptr = coalesce_block(block_ptr);
    buddy_map[GET_BLOCK_SIZE(HEADER_PTR(block_ptr))].push(block_ptr);
#endif
    return;
}

void *MemMalloc::coalesce_block(void *block_ptr) {
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
        curr_size += GET_BLOCK_SIZE(HEADER_PTR(NEXT_BLK_PTR(block_ptr))) +
                     GET_BLOCK_SIZE(HEADER_PTR(PREV_BLK_PTR(block_ptr))) + DSIZE + DSIZE;
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

void MemMalloc::print_block_info(void *block_ptr) {
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

void MemMalloc::print_buddies() {
#ifdef SEG_LIST
    //    std::cout << std::endl;
    //    for (auto buddy: buddy_map) {
    //        std::cout << buddy.first << " : ";
    //        for (auto mem: buddy.second) {
    //            std::cout << mem << " -> ";
    //        }
    //        std::cout << std::endl;
    //    }
#endif
}

void MemMalloc::print_heap() {
    for (char *trover = m_heapListp; GET_BLOCK_SIZE(HEADER_PTR(trover)) > 0; trover = NEXT_BLK_PTR(trover)) {
        print_block_info(trover);
    }
}

void MemMalloc::run_rover() {
#ifdef FIRST_FIT
    char *temp_rover = rover;
    for (; GET_BLOCK_SIZE(HEADER_PTR(temp_rover)) > 0; temp_rover = NEXT_BLK_PTR(temp_rover)) {
        print_block_info(temp_rover);
    }
#endif
}

