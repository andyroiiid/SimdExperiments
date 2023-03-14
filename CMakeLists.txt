cmake_minimum_required(VERSION 3.24)
project(Simd)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Catch2 EXCLUDE_FROM_ALL)

add_executable(Tests Tests.cpp SimdVec.h)

target_link_libraries(Tests PUBLIC Catch2::Catch2WithMain)

add_executable(Benchmarks Benchmarks.cpp SimdVec.h)

target_link_libraries(Benchmarks PUBLIC Catch2::Catch2WithMain)
