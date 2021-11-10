
#include <benchmark/benchmark.h>
#include <vector>
#include <atomic>
#include <string.h>
#include <cmath>
#include <functional>

// double foo() {
//     double result = 0;
//     for (int i = 0; i < 10000000; ++i) {
//         double j = i;
//         double b = std::cos(j);
//         double a = std::sin(j * 2);
//         result += a * b + a / b;
//     }
//     return result;
// }

// double deps() {
//     double result = 0;
//     for (int i = 0; i < 10000000; ++i) {
//         double j = i;
//         double b = std::cos(j);
//         double a = std::sin(b * 2);
//         result += j + a * b / b;
//     }
//     return result;
// }

std::vector<double> g(1 << 23, 1.0);

double light(double a) {
    return a * 4;
}

double light2(double a) {
    return a * 3;
}

using Fn = std::function<double(double)>;

double foo(std::vector<Fn>& values) {
    double result = 0;
    for (int i = 0; i < values.size(); ++i) {
        auto a = values[i](i);
        auto b = g[rand() % g.size()]; // cachemiss
        result += a * b;
    }
    return result;
}

double deps(std::vector<Fn>& values) {
    double result = 0;
    for (int i = 0; i < values.size(); ++i) {
        auto b = g[rand() % g.size()]; // cachemiss
        auto a = values[i](i);
        result += a * b;
    }
    return result;
}


constexpr size_t N = 1 << 20;
void MB_Foo(benchmark::State& state) {
    std::vector<Fn> values;
    for (int i = 0; i < N; ++i) {
        values.push_back(rand() % 2 ? light2 : light);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(foo(values));
    }
}

void MB_Deps(benchmark::State& state) {
    std::vector<Fn> values;
    for (int i = 0; i < N; ++i) {
        values.push_back(rand() % 2 ? light2 : light);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(deps(values));
    }
}

BENCHMARK(MB_Deps);
BENCHMARK(MB_Foo);

BENCHMARK_MAIN();