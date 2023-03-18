add_library(SimdMath INTERFACE SimdVec.h SimdMat.h)

target_compile_definitions(SimdMath INTERFACE _USE_MATH_DEFINES)

target_include_directories(SimdMath INTERFACE .)
