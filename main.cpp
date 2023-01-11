#include "fast_malloc.h"
#include <chrono>

using std::chrono::milliseconds;

int main() {
    fast_malloc mem = fast_malloc();
    void *mem1 = mem.mem_malloc(8);
    void *mem2 = mem.mem_malloc(2*8);
    void *mem3 = mem.mem_malloc(4*8);
    void *mem4 = mem.mem_malloc(6*8);
    mem.print_heap();
    std::cout<<"### \n";
    mem.fast_free(mem1);
    mem.fast_free(mem4);
    mem.fast_free(mem2);
    mem.print_heap();
    std::cout<<"### \n";

    void *mem5 = mem.mem_malloc(16);
    mem.print_heap();
}
