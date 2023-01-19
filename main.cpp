#include "MemMalloc.h"
#include <chrono>

#define LIMIT 20000

void agrims_omnicase_scenario(MemMalloc &mem) {
    int *iarr1 = (int *) mem.mem_malloc(16);
    int *iarr2 = (int *) mem.mem_malloc(16);
    int *iarr3 = (int *) mem.mem_malloc(16);
    int *iarr4 = (int *) mem.mem_malloc(8);
    int *iarr5 = (int *) mem.mem_malloc(16);
    int *iarr6 = (int *) mem.mem_malloc(32);
    int *iarr7 = (int *) mem.mem_malloc(16);

    mem.print_heap();
    mem.mem_free(iarr2); // first fit
    mem.mem_free(iarr4); // best fit
    mem.mem_free(iarr6); // worst fit

    std::cout << "After Freeing\n";
    mem.print_heap();

    int *iarr8 = (int *) mem.mem_malloc(8);

    std::cout << "After Allocating\n";
    mem.print_heap();
    mem.mem_free(iarr8);
}

void custom_malloc_benchmark(MemMalloc &mem) {
    int *iarr[LIMIT];
    for (int i = 0; i < LIMIT; i++) {
        iarr[i] = (int *) mem.mem_malloc(8);
    }
}

void default_malloc_benchmark() {
    int *iarr[LIMIT];
    for (int i = 0; i < LIMIT; i++) {
        iarr[i] = (int *) malloc(8);
    }
}

using std::chrono::milliseconds;

int main() {
    MemMalloc mem = MemMalloc();
//    agrims_omnicase_scenario(mem);
    auto custom_start = std::chrono::high_resolution_clock::now();
    custom_malloc_benchmark(mem);
    auto custom_end = std::chrono::high_resolution_clock::now();
    auto custom_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(custom_end - custom_start);
    std::cout << "Custom Benchmark\t:\t" << custom_benchmark.count() << std::endl;

    auto default_start = std::chrono::high_resolution_clock::now();
    default_malloc_benchmark();
    auto dcustom_end = std::chrono::high_resolution_clock::now();
    auto dcustom_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(dcustom_end - default_start);
    std::cout << "Default Benchmark\t:\t" << dcustom_benchmark.count() << std::endl;
}
