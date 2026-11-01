// Build (NO sanitizers for timing):
//   g++ -std=c++17 -O3 -DNDEBUG alloc_bench.cpp -lbenchmark -lpthread -o alloc_bench
//
// Run (pin to one core, repeat, look at the median):
//   taskset -c 2 ./alloc_bench --benchmark_repetitions=10 --benchmark_report_aggregates_only=true

#include <benchmark/benchmark.h>
#include <cstdlib>
#include <algorithm>
#include <random>
#include <vector>

#include "pool_allocator.h"
#include "freelist_allocator.h"

struct Order
{
    int id;
    double price;
};  // 16 bytes


// --------------------------------------------------
// Helper: Generate shuffled indices
// --------------------------------------------------
std::vector<size_t> getShuffledIndices(size_t n)
{
    std::vector<size_t> indices(n);

    for (size_t i = 0; i < n; i++)
    {
        indices[i] = i;
    }

    std::mt19937 rng(42); // fixed seed => reproducible
    std::shuffle(indices.begin(), indices.end(), rng);

    return indices;
}

// --------------------------------------------------
// Helper: Generate random allocation sizes (16-256 B)
// --------------------------------------------------

std::vector<size_t> getAllocationSizes(size_t n)
{
    std::vector<size_t> allocationSizes(n);

    std::mt19937 rng(7);
    std::uniform_int_distribution<size_t> distribution(1, 16);

    for (size_t i = 0; i < n; ++i)
    {
        allocationSizes[i] = distribution(rng) * 16;
    }
    return allocationSizes;
}

// ---------------------------------------------------------------
// Workload 1: fixed-size objects, allocate a batch then free it all
// (typical "order object" lifecycle). Batch size = state.range(0).
// ---------------------------------------------------------------
static void BM_New_Fixed(benchmark::State& state)
{
    const size_t n = state.range(0);
    
    std::vector<Order*> ptrs(n);

    for (auto _ : state)
    {
        // Allocate objects
        for (size_t i = 0; i < n; i++)
        {
            ptrs[i] = new Order{(int)i, 1.0};
            benchmark::DoNotOptimize(ptrs[i]);
        }

        // Free objects
        for (size_t i = 0; i < n; ++i) 
        {
            delete ptrs[i];
        }
    }

    state.SetItemsProcessed(state.iterations() * n);
}

static void BM_Pool_Fixed(benchmark::State& state)
{
    const size_t n = state.range(0);

    PoolAllocator pool(sizeof(Order), n);          // built once, outside the timed loop

    std::vector<void*> ptrs(n);

    for (auto _ : state)
    {
        // Allocate objects
        for (size_t i = 0; i < n; ++i)
        {
            ptrs[i] = new (pool.allocate()) Order{(int)i, 1.0};
            benchmark::DoNotOptimize(ptrs[i]);
        }

        // Destroy and free objects
        for (size_t i = 0; i < n; ++i)
        {
            static_cast<Order*>(ptrs[i])->~Order();
            pool.deallocate(ptrs[i]);
        }
    }
    state.SetItemsProcessed(state.iterations() * n);
}

// ---------------------------------------------------------------
// Workload 2: alloc/free churn in shuffled order (defeats the
// trivial LIFO case, closer to real behavior).
// ---------------------------------------------------------------

static void BM_New_Shuffled(benchmark::State& state)
{
    const size_t n = state.range(0);

    auto order = getShuffledIndices(n);

    std::vector<Order*> ptrs(n);

    for (auto _ : state)
    {
        // Allocate objects
        for (size_t i = 0; i < n; ++i)
        {
            ptrs[i] = new Order{(int)i, 1.0};
            benchmark::DoNotOptimize(ptrs[i]);
        }

        // Free objects in shuffled order
        for (size_t i : order)
        {
            delete ptrs[i];
        }
    }

    state.SetItemsProcessed(state.iterations() * n);
}

static void BM_Pool_Shuffled(benchmark::State& state)
{
    const size_t n = state.range(0);

    auto order = getShuffledIndices(n);

    PoolAllocator pool(sizeof(Order), n);

    std::vector<void*> ptrs(n);

    for (auto _ : state)
    {
        // Allocate objects
        for (size_t i = 0; i < n; ++i)
        {
            ptrs[i] = new (pool.allocate()) Order{(int)i, 1.0};
            benchmark::DoNotOptimize(ptrs[i]);
        }

        // Destroy and free objects in shuffled order
        for (size_t i : order)
        {
            static_cast<Order*>(ptrs[i])->~Order();
            pool.deallocate(ptrs[i]);
        }
    }

    state.SetItemsProcessed(state.iterations() * n);
}

// ---------------------------------------------------------------
// Workload 3: variable-size (16..256 B), shuffled free.
// Small batch on purpose: your coalesce() is O(n) per free.
// ---------------------------------------------------------------

static void BM_Malloc_Var(benchmark::State& state)
{
    const size_t n = state.range(0);

    auto allocationSizes = getAllocationSizes(n);
    auto order = getShuffledIndices(n);

    std::vector<void*> ptrs(n);

    for (auto _ : state)
    {
        // Allocate memory
        for (size_t i = 0; i < n; ++i)
        {
            ptrs[i] = std::malloc(allocationSizes[i]);
            benchmark::DoNotOptimize(ptrs[i]);
        }

        // Free memory in shuffled order
        for (size_t i : order)
        {
            std::free(ptrs[i]);
        }
    }

    state.SetItemsProcessed(state.iterations() * n);
}

static void BM_FreeList_Var(benchmark::State& state)
{
    const size_t n = state.range(0);

    auto allocationSizes = getAllocationSizes(n);
    auto order = getShuffledIndices(n);

    // Calculate total memory required
    size_t totalSize = 0;
    for (auto size : allocationSizes)
    {
        totalSize += size + sizeof(Header) + 16;
    }

    FreeListAllocator alloc(totalSize);                  // built once, outside the timed loop

    std::vector<void*> ptrs(n);

    for (auto _ : state)
    {
        // Allocate memory
        for (size_t i = 0; i < n; ++i)
        {
            ptrs[i] = alloc.allocate(allocationSizes[i]);
            benchmark::DoNotOptimize(ptrs[i]);
        }

        // Free memory in shuffled order
        for (size_t i : order)
        {
            alloc.deallocate(ptrs[i]);
        }
    }

    state.SetItemsProcessed(state.iterations() * n);
}

// --------------------------------------------------
// Benchmark registration
// --------------------------------------------------

BENCHMARK(BM_New_Fixed)->Arg(1024)->Arg(16384);
BENCHMARK(BM_Pool_Fixed)->Arg(1024)->Arg(16384);
BENCHMARK(BM_New_Shuffled)->Arg(1024)->Arg(16384);
BENCHMARK(BM_Pool_Shuffled)->Arg(1024)->Arg(16384);
BENCHMARK(BM_Malloc_Var)->Arg(256)->Arg(1024);
BENCHMARK(BM_FreeList_Var)->Arg(256)->Arg(1024);

BENCHMARK_MAIN();