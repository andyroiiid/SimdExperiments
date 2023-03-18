cmake_minimum_required(VERSION 3.24)
project(Simd)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(ThirdParty/Catch2 EXCLUDE_FROM_ALL)
add_subdirectory(ThirdParty/glm EXCLUDE_FROM_ALL)
add_subdirectory(ThirdParty/glfw EXCLUDE_FROM_ALL)
add_subdirectory(ThirdParty/glad EXCLUDE_FROM_ALL)

add_subdirectory(SimdMath EXCLUDE_FROM_ALL)

add_subdirectory(Tests EXCLUDE_FROM_ALL)

add_subdirectory(Visualization EXCLUDE_FROM_ALL)
