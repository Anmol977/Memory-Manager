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
    custom_malloc_benchmark(mem);
    auto custom_end = std::chrono::high_resolution_clock::now();
    auto custom_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(custom_end - custom_start);
    std::cout << "Custom Benchmark\t:\t" << custom_benchmark.count() << std::endl;

    auto dcustom_start = std::chrono::high_resolution_clock::now();
    default_malloc_benchmark();
    auto dcustom_end = std::chrono::high_resolution_clock::now();
    auto dcustom_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(dcustom_end - dcustom_start);
    std::cout << "default Benchmark\t:\t" << dcustom_benchmark.count() << std::endl;
}
