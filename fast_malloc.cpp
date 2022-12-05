//
// Created by anmol on 3/12/22.
//

#include "fast_malloc.h"

fast_malloc::fast_malloc() {
    mem_heap = (char *) malloc(MAX_HEAP);
    mem_brk = (char *) mem_heap;
    mem_max_addr = (char *) mem_heap + MAX_HEAP;
    logger = new Logger();
}

void *fast_malloc::fast_sbrk(int incr_amt) {
    char *prev_brk = mem_brk;

    if (incr_amt < 0 or (mem_brk + incr_amt) >= mem_max_addr) {
        logger->print_error(error_strings::INSUFFICIENT_MEMORY);
        exit(-1); // todo : better error handling
    }
    mem_brk += incr_amt;
    return (void *) prev_brk;
}

int fast_malloc::init_mem_list() {

}