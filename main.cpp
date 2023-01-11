#include "fast_malloc.h"
#include <chrono>

#define LIMIT 100

void custom_malloc_benchmark(fast_malloc &mem) {
    int *iarr[LIMIT];
    for (int i = 0; i < LIMIT; i++) {
        iarr[i] = (int *) mem.mem_malloc(sizeof(int));
    }
}

void default_malloc_benchmark() {
    void *iarr[3000];
    for (int i = 0; i < 3000; i++) {
        iarr[i] = malloc(8);

    }
}

using std::chrono::milliseconds;

int main() {
    fast_malloc mem = fast_malloc();
    auto custom_start = std::chrono::high_resolution_clock::now();
//    int *iarr = (int *) mem.mem_malloc(sizeof(int) * 10);
//    int *iarr1 = (int *) mem.mem_malloc(sizeof(int) * 15);
//    int *iarr2 = (int *) mem.mem_malloc(sizeof(int) * 20);
//    mem.fast_free(iarr1);
//    int *iarr3 = (int *) mem.mem_malloc(sizeof(int) * 15);
    custom_malloc_benchmark(mem);
    auto custom_end = std::chrono::high_resolution_clock::now();
    auto custom_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(custom_end - custom_start);
    std::cout << "Custom Benchmark\t:\t" << custom_benchmark.count() << std::endl;

//    mem.print_buddies();

    auto dcustom_start = std::chrono::high_resolution_clock::now();
    default_malloc_benchmark();
    auto dcustom_end = std::chrono::high_resolution_clock::now();
    auto dcustom_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(dcustom_end - dcustom_start);
    std::cout << "default Benchmark\t:\t" << dcustom_benchmark.count() << std::endl;
}
