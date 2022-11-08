#include <iostream>

#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1>>12)

#define ADD_HEADER(SIZE, ISALLOC) ((SIZE) | (ISALLOC))

#define GET(p) (*unsigned int *)p)
#define PUT(p, val) (*(unsigned int *)(p) = (val))

#define GET_HEADER_SIZE(p) (GET(p) & ~0x7)
#define GET_HEADER_ALLOC(p) (GET(p) & 0x1)

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
