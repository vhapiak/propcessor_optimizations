
#include <benchmark/benchmark.h>
#include <vector>
#include <type_traits>
#include <algorithm>

constexpr std::size_t N = 1000000u;

struct Iface {
    virtual size_t run() const noexcept = 0;
};

struct A : Iface {
    size_t run() const noexcept override {
        return -1;
    }
};

struct B : Iface {
    size_t run() const noexcept override {
        return 1;
    }
};

std::size_t sum(std::vector<Iface*> const& vec) {
    std::size_t result = 0u;
    for (auto item : vec) {
        result += item->run();
    }
    return result;
}

void MB_SumSameObject(benchmark::State& state) {
    std::vector<Iface*> vec(N, new A{});

    for (auto _ : state) {
        benchmark::DoNotOptimize(sum(vec));
    }
}

void MB_SumSameClassDifferentObject(benchmark::State& state) {
    std::vector<Iface*> vec;
    for (std::size_t i = 0; i < N; ++i) {
        vec.push_back(new A{});
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(sum(vec));
    }
    for (Iface* p : vec) {
        delete p;
    }
}

void MB_SumTwoClassesOddEven(benchmark::State& state) {
    std::vector<Iface*> vec;
    for (std::size_t i = 0; i < N; ++i) {
        if (i % 2) {
            vec.push_back(new A{});
        } else {
            vec.push_back(new B{});
        }
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(sum(vec));
    }
    for (Iface* p : vec) {
        delete p;
    }
}

void MB_SumTwoClassesRand(benchmark::State& state) {
    std::vector<Iface*> vec;
    for (std::size_t i = 0; i < N; ++i) {
        if (rand() % 2) {
            vec.push_back(new A{});
        } else {
            vec.push_back(new B{});
        }
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(sum(vec));
    }
    for (Iface* p : vec) {
        delete p;
    }
}

std::vector<Iface*> generateRandom() {
    std::vector<Iface*> pattern;
    for (std::size_t i = 0; i < 200; ++i) {
        if (rand() % 2) {
            pattern.push_back(new A{});
        } else {
            pattern.push_back(new B{});
        }
    }
    return pattern;
}

void MB_SumTwoClassesPattern(benchmark::State& state) {
    static std::vector<Iface*> pattern = generateRandom();
    const size_t pattern_length = state.range(0);

    std::vector<Iface*> vec;
    for (std::size_t i = 0; i < N; ++i) {
        vec.push_back(pattern[i % pattern_length]);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(sum(vec));
    }
}

template <size_t I>
struct Template : Iface {
    size_t run() const noexcept override {
        return I;
    }
};


template<size_t ... I>
std::vector<Iface*> generateImpl(std::index_sequence<I...>) {
    return {new Template<I>{}...};
}

template<size_t I>
std::vector<Iface*> generate() {
    return generateImpl(std::make_index_sequence<I>{});
}

void MB_SumTemplate10Rand(benchmark::State& state) {
    std::vector<Iface*> set = generate<10>();
    std::vector<Iface*> vec;
    for (std::size_t i = 0; i < N; ++i) {
        vec.push_back(set[rand() % set.size()]);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(sum(vec));
    }
}

void MB_SumTemplate20Rand(benchmark::State& state) {
    std::vector<Iface*> set = generate<20>();
    std::vector<Iface*> vec;
    for (std::size_t i = 0; i < N; ++i) {
        vec.push_back(set[rand() % set.size()]);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(sum(vec));
    }
}

void MB_SumTemplate20Sorted(benchmark::State& state) {
    std::vector<Iface*> set = generate<20>();
    std::vector<Iface*> vec;
    for (std::size_t i = 0; i < N; ++i) {
        vec.push_back(set[rand() % set.size()]);
    }

    std::sort(vec.begin(), vec.end());

    for (auto _ : state) {
        benchmark::DoNotOptimize(sum(vec));
    }
}

BENCHMARK(MB_SumSameObject);
BENCHMARK(MB_SumSameClassDifferentObject);
BENCHMARK(MB_SumTwoClassesOddEven);
BENCHMARK(MB_SumTwoClassesRand);
BENCHMARK(MB_SumTemplate10Rand);
BENCHMARK(MB_SumTemplate20Rand);
BENCHMARK(MB_SumTemplate20Sorted);
BENCHMARK(MB_SumTwoClassesPattern)->DenseRange(1, 101, 1);

BENCHMARK_MAIN();