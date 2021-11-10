cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O0")

set(BENCHMARK_ENABLE_TESTING OFF)
add_subdirectory(benchmark)

add_executable(Prefetch prefetch.cpp)
target_link_libraries(Prefetch benchmark::benchmark)

add_executable(Threads threads.cpp)
target_link_libraries(Threads benchmark::benchmark)

add_executable(Virtual virtual.cpp)
target_link_libraries(Virtual benchmark::benchmark)

# add_executable(Reordering ordering.cpp)
# target_link_libraries(Reordering benchmark::benchmark)

# add_executable(Branches branches.cpp)
# target_link_libraries(Branches benchmark::benchmark)
