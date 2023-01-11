#include "fast_malloc.h"
#include <chrono>

#define LIMIT 2 

void custom_malloc_benchmark(fast_malloc &mem) {
	int *iarr1 = (int *) mem.mem_malloc(16);
	int *iarr2 = (int *) mem.mem_malloc(16);
	int *iarr3 = (int *) mem.mem_malloc(16);
	int *iarr4 = (int *) mem.mem_malloc(8);
	int *iarr5 = (int *) mem.mem_malloc(16);
	int *iarr6 = (int *) mem.mem_malloc(32);
	int *iarr7 = (int *) mem.mem_malloc(16);

	mem.print_heap();

	mem.fast_free(iarr2); // first fit
	mem.fast_free(iarr4); // best fit
	mem.fast_free(iarr6); // worst fit
						  //
	mem.print_heap();

	int *iarr8 = (int *) mem.mem_malloc(8);

	mem.print_heap();
    /* int *iarr[LIMIT]; */
    /* for (int i = 0; i < LIMIT; i++) { */
    /*     iarr[i] = (int *) mem.mem_malloc(sizeof(int)); */
    /* } */
}

void default_malloc_benchmark() {
    void *iarr[LIMIT];
    for (int i = 0; i < LIMIT; i++) {
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
    /* std::cout << "Custom Benchmark\t:\t" << custom_benchmark.count() << std::endl; */

//    mem.print_buddies();

    /* auto default_start = std::chrono::high_resolution_clock::now(); */
    /* default_malloc_benchmark(); */
    /* auto dcustom_end = std::chrono::high_resolution_clock::now(); */
    /* auto dcustom_benchmark = std::chrono::duration_cast<std::chrono::nanoseconds>(dcustom_end - default_start); */
    /* std::cout << "Default Benchmark\t:\t" << dcustom_benchmark.count() << std::endl; */
}
