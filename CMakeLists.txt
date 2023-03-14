cmake_minimum_required(VERSION 3.24)
project(Simd)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Catch2 EXCLUDE_FROM_ALL)

add_executable(Simd main.cpp SimdVec.h)

target_link_libraries(Simd PUBLIC Catch2::Catch2WithMain)
