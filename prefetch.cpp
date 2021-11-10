#include <benchmark/benchmark.h>
#include <vector>
#include <algorithm>
#include <random>

const size_t N = 10000;
int8_t matrix[N][N];

using Pair = std::pair<size_t, size_t>;
using List = std::vector<Pair>;

size_t sum(List& indexes) {
    size_t sum = 0;
    for (auto& index : indexes) {
        sum += matrix[index.first][index.second];
    }
    return sum;
}

void MB_SumByRows(benchmark::State& state) {
    List list;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            list.push_back({i, j});
        }
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(sum(list));
    }
}

void MB_SumByColumns(benchmark::State& state) {
    List list;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            list.push_back({j, i});
        }
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(sum(list));
    }
}

void MB_Random(benchmark::State& state) {
    List list;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            list.push_back({i, j});
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(list.begin(), list.end(), g);

    for (auto _ : state) {
        benchmark::DoNotOptimize(sum(list));
    }
}

BENCHMARK(MB_SumByRows)->Iterations(10);
BENCHMARK(MB_SumByColumns)->Iterations(10);
BENCHMARK(MB_Random)->Iterations(10);

BENCHMARK_MAIN();