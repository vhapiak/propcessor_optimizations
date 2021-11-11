
#include <benchmark/benchmark.h>
#include <vector>

constexpr size_t N = 1000000;

int sumDigits(char const* str) {
    int sum = 0;
    while(*str) {
        char c = *str++;
        if (c >= '0' && c <= '9') {
            ++sum;
        } else {
            --sum;
        }
    }
    return sum;
}

using Frame = std::vector<char>;

template<bool B, size_t I>
int branches(Frame& f) {
    static char value = rand() % 256;
    if (f[I] > value) {
        return branches<true, I - 1>(f);
    } else {
        return branches<false, I - 1>(f);
    }
}

template<>
int branches<true, 0>(Frame& f) {
    return 1;
}

template<>
int branches<false, 0>(Frame& f) {
    return -1;
}

constexpr size_t Depth = 4;
int sumDeep(std::vector<Frame>& frames) {
    int sum = 0;
    for (Frame& frame : frames) {
        sum += branches<true, Depth>(frame);
    }
    return sum;
}

std::vector<signed char> generateTable() {
    std::vector<signed char> table(256, -1);
    table['0'] = 1;
    table['1'] = 1;
    table['2'] = 1;
    table['3'] = 1;
    table['4'] = 1;
    table['5'] = 1;
    table['6'] = 1;
    table['7'] = 1;
    table['8'] = 1;
    table['9'] = 1;
    return table;
}

size_t sumDigitsLookUp(char const* str) {
    static auto table = generateTable();

    size_t sum = 0;
    while(*str) {
        char c = *str;
        sum += table[c];
        ++str;
    }
    return sum;
}

void MB_OnlyDigits(benchmark::State& state) {
    std::vector<char> str;
    for (int i = 0; i < N; ++i) {
        str.push_back(i % 10 + '0');
    }
    str.push_back(0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(sumDigits(str.data()));
    }
}

void MB_Random(benchmark::State& state) {
    std::vector<char> str;
    for (int i = 0; i < N; ++i) {
        str.push_back(rand() % 255 + 1);
    }
    str.push_back(0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(sumDigits(str.data()));
    }
}

void MB_Pattern(benchmark::State& state) {
    std::vector<char> pattern;
    for (int i = 0; i < state.range(0); ++i) {
        pattern.push_back(rand() % 255 + 1);
    }
    std::vector<char> str;
    for (int i = 0; i < N; ++i) {
        str.push_back(pattern[i % pattern.size()]);
    }
    str.push_back(0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(sumDigits(str.data()));
    }
}

void MB_OnlyDigitsLookUp(benchmark::State& state) {
    std::vector<char> str;
    for (int i = 0; i < N; ++i) {
        str.push_back(i % 10 + '0');
    }
    str.push_back(0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(sumDigitsLookUp(str.data()));
    }
}

void MB_RandomLookUp(benchmark::State& state) {
    std::vector<char> str;
    for (int i = 0; i < N; ++i) {
        str.push_back(rand() % 255 + 1);
    }
    str.push_back(0);
    for (auto _ : state) {
        benchmark::DoNotOptimize(sumDigitsLookUp(str.data()));
    }
}

void MB_DeepConst(benchmark::State& state) {
    Frame frame;
    for (int i = 0; i < Depth + 1; ++i) {
        frame.push_back(rand() % 256);
    }
    std::vector<Frame> data(N, frame);
    for (auto _ : state) {
        benchmark::DoNotOptimize(sumDeep(data));
    }
}

void MB_DeepRand(benchmark::State& state) {
    std::vector<Frame> data;
    for (int j = 0; j < N; ++j) {
        Frame frame;
        for (int i = 0; i < Depth + 1; ++i) {
            frame.push_back(rand() % 256);
        }
        data.push_back(frame);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(sumDeep(data));
    }
}

void MB_DeepPattern(benchmark::State& state) {
    std::vector<Frame> pattern;
    for (int j = 0; j < state.range(0); ++j) {
        Frame frame;
        for (int i = 0; i < Depth + 1; ++i) {
            frame.push_back(rand() % 256);
        }
        pattern.push_back(frame);
    }
    std::vector<Frame> data;
    for (int j = 0; j < N; ++j) {
        data.push_back(pattern[j % pattern.size()]);
    }
    for (auto _ : state) {
        benchmark::DoNotOptimize(sumDeep(data));
    }
}

BENCHMARK(MB_OnlyDigits);
BENCHMARK(MB_Random);
BENCHMARK(MB_OnlyDigitsLookUp);
BENCHMARK(MB_RandomLookUp);
BENCHMARK(MB_Pattern)->DenseRange(1, 10001, 100);
BENCHMARK(MB_DeepConst);
BENCHMARK(MB_DeepRand);
BENCHMARK(MB_DeepPattern)->DenseRange(1, 100, 1);
BENCHMARK(MB_DeepPattern)->DenseRange(1, 2000, 100);

BENCHMARK_MAIN();