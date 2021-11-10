
#include <benchmark/benchmark.h>
#include <vector>
#include <thread>

const size_t N = 4;

size_t sumPart(size_t& sum) {
    for (size_t j = 0; j < 200000000u; ++j) {
        sum += j * j * j % 3;
    }
}

size_t sumSingleThread() {
    size_t sum = 0;
    for (size_t i = 0; i < N; ++i) {
        sumPart(sum);
    }
    return sum;
}


size_t sumThreads(size_t threads) {
    std::vector<std::thread> threads_list;
    size_t *counters = new size_t[threads];
    for (size_t i = 0; i < threads; ++i) {
        threads_list.emplace_back(std::thread{sumPart, std::ref(counters[i])});
    }
    for (auto& thread : threads_list) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

size_t sumThreadsOptimized(size_t threads) {
    std::vector<std::thread> threads_list;
    size_t *counters = new size_t[threads * 8];
    for (size_t i = 0; i < threads; ++i) {
        threads_list.emplace_back(std::thread{sumPart, std::ref(counters[i * 8])});
    }
    for (auto& thread : threads_list) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void MB_SingleThread(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(sumSingleThread());
    }
}

void MB_Threads4(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(sumThreads(4));
    }
}

void MB_OptimizedThreads4(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(sumThreadsOptimized(4));
    }
}

BENCHMARK(MB_SingleThread)->Iterations(1);
BENCHMARK(MB_Threads4)->Iterations(1);
BENCHMARK(MB_OptimizedThreads4)->Iterations(1);

BENCHMARK_MAIN();