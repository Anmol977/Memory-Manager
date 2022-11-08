#include <iostream>
#include <cstdlib>

#define MAX_HEAP 1 * 512

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1>>12)

#define ADD_HEADER(SIZE, ISALLOC) ((SIZE) | (ISALLOC))

#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

#define GET_HEADER_SIZE(p) (GET(p) & ~0x7)
#define GET_HEADER_ALLOC(p) (GET(p) & 0x1)

#define HEADER_PTR(bp) ((char *)(bp) - WSIZE)
#define FOOTER_PTR(cp) ((char *)(bp) + GET_SIZE(HDRP(bp)))

#define NEXT_BLK_PTR(bp) ((char *)(bp) + GET_HEADER_SIZE(HEADER_PTR(bp)))
#define PREV_BLK_PTR(bp) ((char *)(bp) - DSIZE - GET_HEADER_SIZE((char *)(bp) - DSIZE))

int main() {
    char *p = (char *) malloc(MAX_HEAP);
    char *temp = p;
    unsigned int blocksize = WSIZE + DSIZE + WSIZE;
    unsigned int numBlocks = MAX_HEAP / blocksize;
    std::cout << "numblocks : " << numBlocks << std::endl;
    for (unsigned int i = 0; i < numBlocks; i++) {
        PUT(temp, ADD_HEADER(DSIZE, 0));
        PUT(temp + WSIZE, i);
        PUT(temp + WSIZE + DSIZE, ADD_HEADER(DSIZE, 0));
        temp += blocksize;
    }
    temp = p;
    for (unsigned int i = 0; i < numBlocks; i++) {
        std::cout<< GET(temp + WSIZE)<<std::endl;
        std::cout << GET(temp) << std::endl;
        temp+=blocksize;
    }
    return 0;
}
