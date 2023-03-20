//
// Created by andyroiiid on 3/14/2023.
//

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "SimdMat.h"

struct Vec {
    float x;
    float y;
    float z;
    float w;

    [[nodiscard]] Vec Normalize() const {
        float sum = x * x + y * y + z * z + w * w;
        float len = sqrt(sum);
        return {x / len, y / len, z / len, w / len};
    }

    [[nodiscard]] Vec Cross(const Vec &v) const {
        return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x, 0.0f};
    }
};

TEST_CASE("Normalization Benchmarks") {
    const Vec plain{1.0f, 2.0f, 3.0f, 4.0f};
    const SimdVec simd{1.0f, 2.0f, 3.0f, 4.0f};

    BENCHMARK("Plain Normalize") {
        volatile Vec result = plain.Normalize();
        (void) result;
    };

    BENCHMARK("SIMD Normalize") {
        volatile SimdVec result = simd.Normalize();
        (void) result;
    };

    BENCHMARK("SIMD Fast Normalize") {
        volatile SimdVec result = simd.FastNormalize();
        (void) result;
    };
}

TEST_CASE("Cross Product Benchmarks") {
    const Vec plain{1.0f, 2.0f, 3.0f, 4.0f};
    const Vec plainB{2.0f, 3.0f, 4.0f, 5.0f};

    BENCHMARK("Plain Cross Product") {
        volatile Vec result = plain.Cross(plainB);
        (void) result;
    };

    const SimdVec simd{1.0f, 2.0f, 3.0f, 4.0f};
    const SimdVec simdB{2.0f, 3.0f, 4.0f, 5.0f};

    BENCHMARK("SIMD Cross Product") {
        volatile SimdVec result = simd.Cross(simdB);
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

    const SimdMat simd{1.0f, 2.0f, 3.0f, 4.0f,
                       5.0f, 6.0f, 7.0f, 8.0f,
                       9.0f, 10.0f, 11.0f, 12.0f,
                       13.0f, 14.0f, 15.0f, 16.0f};

    BENCHMARK("SIMD Matrix Multiplication") {
        volatile SimdMat result = simd * simd;
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
        volatile SimdMat result = SimdMat::LookAt({1.0f, 2.0f, 3.0f, 1.0f},
                                                  {0.0f, 0.0f, 0.0f, 1.0f},
                                                  {0.0f, 1.0f, 0.0f, 0.0f});
        (void) result;
    };
}
