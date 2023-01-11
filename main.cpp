#include "fast_malloc.h"
#include <chrono>
#include <cstdint>

void custom_malloc_benchmark(fast_malloc &mem, int LIMIT) {
	/* EXAMPLE CASE */
	/* int *iarr1 = (int *) mem.mem_malloc(16); */
	/* int *iarr2 = (int *) mem.mem_malloc(16); */
	/* int *iarr3 = (int *) mem.mem_malloc(16); */
	/* int *iarr4 = (int *) mem.mem_malloc(8); */
	/* int *iarr5 = (int *) mem.mem_malloc(16); */
	/* int *iarr6 = (int *) mem.mem_malloc(32); */
	/* int *iarr7 = (int *) mem.mem_malloc(16); */

	/* mem.print_heap(); */

	/* mem.fast_free(iarr2); // first fit */
	/* mem.fast_free(iarr4); // best fit */
	/* mem.fast_free(iarr6); // worst fit */
	/* 					  // */
	/* mem.print_heap(); */

	/* int *iarr8 = (int *) mem.mem_malloc(8); */

	/* mem.print_heap(); */
	int *iarr = (int *) mem.mem_malloc(sizeof(int) * LIMIT);
}


void default_malloc_benchmark(int LIMIT) {
	int *iarr = (int *) malloc(sizeof(int) * LIMIT);
}

using std::chrono::milliseconds;

int main() {
	int limit[12] = { 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096 };
    fast_malloc mem = fast_malloc();
	uint32_t custom_sum = 0, default_sum = 0;
	for (int i = 0; i < 12; i++) {
		for (int i = 0; i < 5; i++) {
			auto custom_start = std::chrono::high_resolution_clock::now();
			custom_malloc_benchmark(mem, limit[i]);
			auto custom_end = std::chrono::high_resolution_clock::now();
			auto custom_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(custom_end - custom_start);
			custom_sum += custom_benchmark.count();
		}
		std::cout << "Average Custom Benchmark for " << limit[i] << " Ints\t:\t" << custom_sum / 5 << std::endl;

//    mem.print_buddies();

		for (int i = 0; i < 5; i++) {
			auto default_start = std::chrono::high_resolution_clock::now();
			 default_malloc_benchmark(limit[i]);
			auto default_end = std::chrono::high_resolution_clock::now();
			auto default_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(default_end - default_start);
			default_sum += default_benchmark.count();
		}
		std::cout << "Default Custom Benchmark for " << limit[i] << " Ints\t:\t" << default_sum / 5 << std::endl << std::endl;
	}

    /* auto default_start = std::chrono::high_resolution_clock::now(); */
    /* default_malloc_benchmark(); */
    /* auto dcustom_end = std::chrono::high_resolution_clock::now(); */
    /* auto dcustom_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(dcustom_end - default_start); */
    /* std::cout << "Default Benchmark\t:\t" << dcustom_benchmark.count() << std::endl; */
}
