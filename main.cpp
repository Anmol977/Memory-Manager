#include "fast_malloc.h"
#include <chrono>
#define LIMIT 100

void custom_malloc_benchmark(fast_malloc &mem) {
	int *iarr[LIMIT];
	for (int i = 0; i < LIMIT; i++) {
		iarr[i] = (int *) mem.mem_malloc(sizeof(int));
	}
}

void default_malloc_benchmark(dummy* dum) {
	int *iarr[LIMIT];
	for (int i = 0; i < LIMIT; i++) {
		iarr[i] = (int *) dum->dummy_func(sizeof(int));
	}
}

using std::chrono::milliseconds;

int main() {
    fast_malloc mem = fast_malloc();
	dummy *dum = new dummy();
    auto custom_start = std::chrono::high_resolution_clock::now();
    custom_malloc_benchmark(mem);
    auto custom_end = std::chrono::high_resolution_clock::now();
    auto custom_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(custom_end - custom_start);
    std::cout << "Custom Benchmark\t:\t" << custom_benchmark.count() << std::endl;

//    mem.print_buddies();

    auto default_start = std::chrono::high_resolution_clock::now();
    default_malloc_benchmark(dum);
    auto default_end = std::chrono::high_resolution_clock::now();
    auto default_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(default_end - default_start);
    std::cout << "Default Benchmark\t:\t" << default_benchmark.count() << std::endl;
}
