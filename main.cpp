#include "fast_malloc.h"

int main() {
    fast_malloc mem = fast_malloc();

    int *iarr1 = (int *) mem.mem_malloc(sizeof(int) * 3);
    for (int i = 0; i < 3; i++) {
        *(iarr1 + i) = i;
    }
//    mem.print_block_info(iarr1);

    int *iarr2 = (int *) mem.mem_malloc(sizeof(int) * 5);
    for (int i = 0; i < 5; i++) {
        *(iarr2 + i) = i;
    }
//    mem.print_block_info(iarr2);

    int *iarr3 = (int *) mem.mem_malloc(sizeof(int) * 7);
    for (int i = 0; i < 7; i++) {
        *(iarr3 + i) = i;
    }
//    mem.print_block_info(iarr3);

    int *iarr4 = (int *) mem.mem_malloc(sizeof(int) * 9);
    for (int i = 0; i < 9; i++) {
        *(iarr4 + i) = i;
    }
//    mem.print_block_info(iarr4);


//    mem.fast_free(iarr2);
//    mem.fast_free(iarr3);
    mem.fast_coalesce();
//    mem.coalesce_block(iarr3);
}
