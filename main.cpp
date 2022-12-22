#include "fast_malloc.h"

int main() {
    fast_malloc mem = fast_malloc();
    int *iarr = (int *) mem.mem_malloc(sizeof(int) * 5);
    for (int i = 0; i != 10; i++) {
        *(iarr + i) = i;
    }

    int *iarr1 = (int *) mem.mem_malloc(sizeof(int) * 7);
    for (int i = 0; i != 10; i++) {
        *(iarr1 + i) = i;
    }
    int *iarr2 = (int *) mem.mem_malloc(sizeof(int) * 9);
    for (int i = 0; i != 10; i++) {
        *(iarr2 + i) = i;
    }
    int *iarr3 = (int *) mem.mem_malloc(sizeof(int) * 11);
    for (int i = 0; i != 10; i++) {
        *(iarr3 + i) = i;
    }
    int *iarr4 = (int*)mem.mem_malloc((sizeof(int) * 1));
    std::cout<<GET_BLOCK_SIZE(HEADER_PTR(iarr))<<" " << GET_BLOCK_SIZE(HEADER_PTR(NEXT_BLK_PTR(iarr)))<<" " << GET_BLOCK_SIZE(HEADER_PTR(PREV_BLK_PTR(iarr)))<<std::endl;
    std::cout<<GET_BLOCK_SIZE(HEADER_PTR(iarr1))<<" " << GET_BLOCK_SIZE(HEADER_PTR(NEXT_BLK_PTR(iarr1)))<<" " << (GET_BLOCK_SIZE((char *)(iarr1) - DSIZE))<<std::endl;
    std::cout<<GET_BLOCK_SIZE(HEADER_PTR(iarr2))<<" " << GET_BLOCK_SIZE(HEADER_PTR(NEXT_BLK_PTR(iarr2)))<<" " << GET_BLOCK_SIZE(HEADER_PTR(PREV_BLK_PTR(iarr2)))<<std::endl;
    std::cout<<GET_BLOCK_SIZE(HEADER_PTR(iarr3))<<" " << GET_BLOCK_SIZE(HEADER_PTR(NEXT_BLK_PTR(iarr3)))<<" " << GET_BLOCK_SIZE(HEADER_PTR(PREV_BLK_PTR(iarr3)))<<std::endl;

//    mem.fast_free(iarr2);
//    mem.fast_free(iarr3);
//    mem.coalesce_block(iarr3);
}
