#include "fast_malloc.h"
#include <chrono>

void custom_malloc_benchmark() {
    fast_malloc mem = fast_malloc();

    int *iarr1 = (int *) mem.mem_malloc(sizeof(int) * 3);
//    mem.print_block_info(iarr1);

//    int *iarr2 = (int *) mem.mem_malloc(sizeof(int) * 5);
////    mem.print_block_info(iarr2);
//
//    int *iarr3 = (int *) mem.mem_malloc(sizeof(int) * 7);
////    mem.print_block_info(iarr3);
//
//    int *iarr4 = (int *) mem.mem_malloc(sizeof(int) * 5);
////    mem.print_block_info(iarr4);
//
//
//    mem.fast_free(iarr2);
//    mem.fast_free(iarr3);
//    mem.fast_free(iarr4);
}

void default_malloc_benchmark() {
    int *iarr1 = (int *) malloc(sizeof(int) * 3);
//    mem.print_block_info(iarr1);
//
//    int *iarr2 = (int *)malloc(sizeof(int) * 5);
////    mem.print_block_info(iarr2);
//
//    int *iarr3 = (int *) malloc(sizeof(int) * 7);
////    mem.print_block_info(iarr3);
//
//    int *iarr4 = (int *)malloc(sizeof(int) * 5);
////    mem.print_block_info(iarr4);
//
//    free(iarr2);
//    free(iarr3);
//    free(iarr4);
}

using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

int main() {
    fast_malloc mem = fast_malloc();
    auto custom_start = std::chrono::high_resolution_clock::now();
    int *iarr1 = (int *) mem.mem_malloc(sizeof(int) * 3);
//    custom_malloc_benchmark();
    auto custom_end = std::chrono::high_resolution_clock::now();
    auto custom_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(custom_end - custom_start);
    std::cout << "Custom Benchmark\t:\t" << custom_benchmark.count() << std::endl;

    auto default_start = std::chrono::high_resolution_clock::now();
    default_malloc_benchmark();
    auto default_end = std::chrono::high_resolution_clock::now();
    auto default_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(default_end - default_start);
    std::cout << "Default Benchmark\t:\t" << default_benchmark.count() << std::endl;
}
