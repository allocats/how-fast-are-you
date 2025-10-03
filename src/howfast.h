#ifndef HOWFAST_H
#define HOWFAST_H

#ifndef HOWFAST_ASSERT
#include <assert.h>
#define HOWFAST_ASSERT assert
#endif // !HOWFAST_ASSERT

#include <stdint.h>
#include <stdio.h>
#include <time.h>

#define HOWFAST_BENCHMARK_BEGIN(label) howfast_benchmark_begin(label)
#define HOWFAST_BENCHMARK_END() howfast_benchmark_end()
#define HOWFAST_PRINT_REPORT() howfast_print_report()

typedef struct {
    const char* label;
    uint64_t start_ns;
    uint64_t end_ns;
    uint64_t start_cycles;
    uint64_t end_cycles;
} Howfast_Benchmark;

#define SIZE 256

static size_t count = 0;
static uint32_t current = 0;
static Howfast_Benchmark benchmarks[SIZE] = {0};

static inline uint64_t howfast_rdtsc(void) {
    uint32_t low, high;
    __asm__ volatile("rdtsc" : "=a"(low), "=d"(high));
    return ((uint64_t) high << 32) | low;
}

static inline uint64_t howfast_get_time_ns(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t) ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

static void howfast_benchmark_begin(const char* label) {
    HOWFAST_ASSERT(count < SIZE && "Benchmark records are full");
    HOWFAST_ASSERT(label && "passing null is mean :c");

    current = count++;
    Howfast_Benchmark* b = &benchmarks[current];
    b -> label = label;
    b -> start_cycles = howfast_rdtsc();
    b -> start_ns = howfast_get_time_ns();
}

static void howfast_benchmark_end(void) {
    uint64_t end_cycles = howfast_rdtsc(); 
    uint64_t end_ns = howfast_get_time_ns();

    Howfast_Benchmark* b = &benchmarks[current];
    b -> end_cycles = end_cycles;
    b -> end_ns = end_ns;
}

static void howfast_print_report(void) {
    HOWFAST_ASSERT(count > 0 && "No benchmarks recorded");

    for (int i = 0; i < count; i++) {
        Howfast_Benchmark* b = &benchmarks[i];
        uint64_t cycles = b -> end_cycles - b -> start_cycles;
        uint64_t ns = b -> end_ns - b -> start_ns;

        printf("%s:\n", b -> label);
        printf("  Cycles: %lu\n", cycles);
        printf("  Took: %.10fms\n", ns / 1e6);
    }
}

#endif // !HOWFAST_H
