add_library(TestUtils TestUtils.cpp TestUtils.h)

target_link_libraries(TestUtils PUBLIC Catch2::Catch2WithMain SimdMath)

macro(add_my_test test)
    add_executable(${test} ${test}.cpp)
    target_link_libraries(${test} PUBLIC TestUtils)
endmacro()

add_my_test(VectorTests)
add_my_test(Benchmarks)
