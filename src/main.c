#include <stdlib.h>

#include "howfast.h"

int main(int argc, char* argv[]) {
    for (int i = 0; i < 100; i++) {
        char* warmup = malloc(4096);
        free(warmup);
    }

    HOWFAST_BENCHMARK_BEGIN("malloc-only");
    for (int i = 0; i < 10000; i++) {
        char* s = malloc(4096);
        free(s);
    }
    HOWFAST_BENCHMARK_END();

    HOWFAST_BENCHMARK_BEGIN("stack-only");
    for (int i = 0; i < 10000; i++) {
        volatile char buffer[4096];
    }
    HOWFAST_BENCHMARK_END();

    HOWFAST_PRINT_REPORT();
}
