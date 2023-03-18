cmake_minimum_required(VERSION 3.24)
project(Simd)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(SimdMath EXCLUDE_FROM_ALL)

add_subdirectory(Catch2 EXCLUDE_FROM_ALL)

add_subdirectory(glm EXCLUDE_FROM_ALL)

add_subdirectory(Tests EXCLUDE_FROM_ALL)
