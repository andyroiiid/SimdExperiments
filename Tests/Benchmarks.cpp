//
// Created by andyroiiid on 3/14/2023.
//

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>

#include "SimdVec.h"

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