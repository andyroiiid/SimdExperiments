//
// Created by andyroiiid on 3/14/2023.
//

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mat4.h"
#include "PlainMath.h"
#include "Quat.h"

TEST_CASE("Normalization Benchmarks") {
    const PlainVec plain{1.0f, 2.0f, 3.0f, 4.0f};
    const Vec4 simd{1.0f, 2.0f, 3.0f, 4.0f};

    BENCHMARK("Plain Normalize") {
        volatile PlainVec result = plain.Normalize();
        (void) result;
    };

    BENCHMARK("SIMD Normalize") {
        volatile Vec4 result = simd.Normalize();
        (void) result;
    };

    BENCHMARK("SIMD Fast Normalize") {
        volatile Vec4 result = simd.FastNormalize();
        (void) result;
    };
}

TEST_CASE("Cross Product Benchmarks") {
    const PlainVec plain{1.0f, 2.0f, 3.0f, 4.0f};
    const PlainVec plainB{2.0f, 3.0f, 4.0f, 5.0f};

    BENCHMARK("Plain Cross Product") {
        volatile PlainVec result = plain.Cross(plainB);
        (void) result;
    };

    const Vec4 simd{1.0f, 2.0f, 3.0f, 4.0f};
    const Vec4 simdB{2.0f, 3.0f, 4.0f, 5.0f};

    BENCHMARK("SIMD Cross Product") {
        volatile Vec4 result = simd.Cross(simdB);
        (void) result;
    };
}

TEST_CASE("Matrix Multiplication Benchmarks") {
    const glm::mat4 plain{1.0f, 2.0f, 3.0f, 4.0f,
                          5.0f, 6.0f, 7.0f, 8.0f,
                          9.0f, 10.0f, 11.0f, 12.0f,
                          13.0f, 14.0f, 15.0f, 16.0f};

    BENCHMARK("Plain Matrix Multiplication") {
        volatile glm::mat4 result = plain * plain;
        (void) result;
    };

    const Mat4 simd{1.0f, 2.0f, 3.0f, 4.0f,
                    5.0f, 6.0f, 7.0f, 8.0f,
                    9.0f, 10.0f, 11.0f, 12.0f,
                    13.0f, 14.0f, 15.0f, 16.0f};

    BENCHMARK("SIMD Matrix Multiplication") {
        volatile Mat4 result = simd * simd;
        (void) result;
    };
}

TEST_CASE("LookAt Matrix Benchmarks") {
    BENCHMARK("Plain LookAt") {
        volatile glm::mat4 result = glm::lookAt(glm::vec3{1.0f, 2.0f, 3.0f},
                                                glm::vec3{0.0f, 0.0f, 0.0f},
                                                glm::vec3{0.0f, 1.0f, 0.0f});
        (void) result;
    };

    BENCHMARK("SIMD LookAt") {
        volatile Mat4 result = Mat4::LookAt({1.0f, 2.0f, 3.0f, 1.0f},
                                            {0.0f, 0.0f, 0.0f, 1.0f},
                                            {0.0f, 1.0f, 0.0f, 0.0f});
        (void) result;
    };
}

TEST_CASE("Perspective Matrix Benchmarks") {
    BENCHMARK("Plain Perspective") {
        volatile glm::mat4 result = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);
        (void) result;
    };

    BENCHMARK("SIMD Perspective") {
        volatile Mat4 result = Mat4::Perspective(M_PI / 3.0f, 1.0f, 0.1f, 100.0f);
        (void) result;
    };
}

TEST_CASE("Quaternion Hamilton Product Benchmarks") {
    BENCHMARK("Plain Quat Hamilton Product") {
        volatile PlainQuat result = PlainQuat{1.0f, 0.0f, 0.0f, 0.0f} * PlainQuat{0.0f, 0.0f, 0.0f, 1.0};
        (void) result;
    };

    BENCHMARK("SIMD Quat Hamilton Product") {
        volatile Quat result = Quat{1.0f, 0.0f, 0.0f, 0.0f} * Quat{0.0f, 0.0f, 0.0f, 1.0f};
        (void) result;
    };
}

TEST_CASE("Quaternion To Matrix Benchmarks") {
    BENCHMARK("Plain Quat To Matrix") {
        volatile Mat4 result = PlainQuat{1.0f, 0.0f, 0.0f, 0.0f}.ToMat4();
        (void) result;
    };

    BENCHMARK("SIMD Quat To Matrix") {
        volatile Mat4 result = Quat{1.0f, 0.0f, 0.0f, 0.0f}.ToMat4();
        (void) result;
    };
}
