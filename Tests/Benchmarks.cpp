//
// Created by andyroiiid on 3/14/2023.
//

#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mat4.h"
#include "Quat.h"

struct PlainVec {
    float x;
    float y;
    float z;
    float w;

    [[nodiscard]] PlainVec Normalize() const {
        float sum = x * x + y * y + z * z + w * w;
        float len = sqrt(sum);
        return {x / len, y / len, z / len, w / len};
    }

    [[nodiscard]] PlainVec Cross(const PlainVec &v) const {
        return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x, 0.0f};
    }
};

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

struct alignas(16) PlainQuat {
    PlainQuat operator*(const PlainQuat &q) const {
        const float x1 = x;
        const float y1 = y;
        const float z1 = z;
        const float w1 = w;
        const float x2 = q.x;
        const float y2 = q.y;
        const float z2 = q.z;
        const float w2 = q.w;
        return {w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2,
                w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2,
                w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2,
                w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2};
    }

    float x;
    float y;
    float z;
    float w;
};

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
