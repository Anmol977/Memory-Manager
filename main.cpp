#include "fast_malloc.h"

int main() {
    fast_malloc mem = fast_malloc();
    int *iarr1 = (int *) mem.mem_malloc(sizeof(int) * 5);
    for (int i = 0; i != 10; i++) {
        *(iarr1 + i) = i;
    }

    int *iarr2 = (int *) mem.mem_malloc(sizeof(int) * 7);
    for (int i = 0; i != 10; i++) {
        *(iarr2 + i) = i;
    }
//    int *iarr3 = (int *) mem.mem_malloc(sizeof(int) * 9);
//    for (int i = 0; i != 10; i++) {
//        *(iarr3 + i) = i;
//    }
//    int *iarr4 = (int *) mem.mem_malloc(sizeof(int) * 11);
//    for (int i = 0; i != 10; i++) {
//        *(iarr4 + i) = i;
//    }
//    int *iarr5 = (int*)mem.mem_malloc((sizeof(int) * 1));

    mem.print_block_info(iarr1);
    mem.print_block_info(iarr2);
//    mem.print_block_info(iarr2);

//    mem.fast_free(iarr2);
//    mem.fast_free(iarr3);
//    mem.coalesce_block(iarr3);
}
