add_library(SimdMath INTERFACE Vec4.h Mat4.h Quat.h)

target_compile_definitions(SimdMath INTERFACE _USE_MATH_DEFINES)

target_include_directories(SimdMath INTERFACE .)
