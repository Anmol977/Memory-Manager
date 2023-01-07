#include "fast_malloc.h"
#include <chrono>

void custom_malloc_benchmark(fast_malloc &mem) {
    int *iarr = (int *)mem.mem_malloc(sizeof(int ) * 10);
    mem.fast_free(iarr);
    return;
}

void default_malloc_benchmark() {
    int *iarr = (int *)malloc(sizeof(int ) * 10);
    free(iarr);
    return;
}

using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

int main() {
    fast_malloc mem = fast_malloc();
    auto custom_start = std::chrono::high_resolution_clock::now();
    custom_malloc_benchmark(mem);
    auto custom_end = std::chrono::high_resolution_clock::now();
    auto custom_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(custom_end - custom_start);
    std::cout << "Custom Benchmark\t:\t" << custom_benchmark.count() << std::endl;

    mem.print_buddies();

    auto default_start = std::chrono::high_resolution_clock::now();
    default_malloc_benchmark();
    auto default_end = std::chrono::high_resolution_clock::now();
    auto default_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(default_end - default_start);
    std::cout << "Default Benchmark\t:\t" << default_benchmark.count() << std::endl;
}
