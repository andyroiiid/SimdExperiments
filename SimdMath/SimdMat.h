//
// Created by Andrew Huang on 3/17/2023.
//

#pragma once

#include <cmath>

#include "SimdVec.h"

// c0: m00, m10, m20, m30
// c1: m01, m11, m21, m31
// c2: m02, m12, m22, m32
// c3: m03, m13, m23, m33
//
// Column-major
struct SimdMat {
    // Constructors

    SimdMat()
        : c0{1.0f, 0.0f, 0.0f, 0.0f},
          c1{0.0f, 1.0f, 0.0f, 0.0f},
          c2{0.0f, 0.0f, 1.0f, 0.0f},
          c3{0.0f, 0.0f, 0.0f, 1.0f} {}

    SimdMat(const SimdVec &c0, const SimdVec &c1, const SimdVec &c2, const SimdVec &c3)
        : c0{c0}, c1{c1}, c2{c2}, c3{c3} {}

    SimdMat(__m128 c0, __m128 c1, __m128 c2, __m128 c3)
        : c0{c0}, c1{c1}, c2{c2}, c3{c3} {}

    SimdMat(float m00, float m10, float m20, float m30,
            float m01, float m11, float m21, float m31,
            float m02, float m12, float m22, float m32,
            float m03, float m13, float m23, float m33)
        : c0{m00, m10, m20, m30},
          c1{m01, m11, m21, m31},
          c2{m02, m12, m22, m32},
          c3{m03, m13, m23, m33} {}

    // Accessors

    float *Components() { return reinterpret_cast<float *>(this); }
    float &operator[](size_t i) { return Components()[i]; }

    // Const Accessors

    [[nodiscard]] const float *Components() const { return reinterpret_cast<const float *>(this); }
    [[nodiscard]] const float &operator[](size_t i) const { return Components()[i]; }

    // Operators

    [[nodiscard]] SimdMat Transpose() const {
        __m128 m0 = c0.m;
        __m128 m1 = c1.m;
        __m128 m2 = c2.m;
        __m128 m3 = c3.m;
        _MM_TRANSPOSE4_PS(m0, m1, m2, m3)
        return {m0, m1, m2, m3};
    }

    SimdVec operator*(const SimdVec &v) const {
        const SimdVec v0{_mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(0, 0, 0, 0))};
        const SimdVec v1{_mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(1, 1, 1, 1))};
        const SimdVec v2{_mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(2, 2, 2, 2))};
        const SimdVec v3{_mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(3, 3, 3, 3))};
        return c0 * v0 + c1 * v1 + c2 * v2 + c3 * v3;
    }

    SimdMat operator*(const SimdMat &m) const {
        return {operator*(m.c0), operator*(m.c1), operator*(m.c2), operator*(m.c3)};
    }

    SimdVec c0;
    SimdVec c1;
    SimdVec c2;
    SimdVec c3;

    static SimdMat Translate(const SimdVec &translation) {
        return {{1.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                translation};
    }

    static SimdMat Scale(const float scale) {
        return {{scale, 0.0f, 0.0f, 0.0f},
                {0.0f, scale, 0.0f, 0.0f},
                {0.0f, 0.0f, scale, 0.0f},
                SimdVec{0.0f, 0.0f, 0.0f, 1.0f}};
    }

    static SimdMat Scale(const SimdVec &scale) {
        return {{scale.X(), 0.0f, 0.0f, 0.0f},
                {0.0f, scale.Y(), 0.0f, 0.0f},
                {0.0f, 0.0f, scale.Z(), 0.0f},
                SimdVec{0.0f, 0.0f, 0.0f, 1.0f}};
    }

    static SimdMat RotateX(const float theta) {
        const float c = std::cos(theta);
        const float s = std::sin(theta);
        return {{1.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, c, s, 0.0f},
                {0.0f, -s, c, 0.0f},
                SimdVec{0.0f, 0.0f, 0.0f, 1.0f}};
    }

    static SimdMat RotateY(const float theta) {
        const float c = std::cos(theta);
        const float s = std::sin(theta);
        return {{c, 0.0f, -s, 0.0f},
                {0.0f, 1.0f, 0.0f, 0.0f},
                {s, 0.0f, c, 0.0f},
                SimdVec{0.0f, 0.0f, 0.0f, 1.0f}};
    }

    static SimdMat RotateZ(const float theta) {
        const float c = std::cos(theta);
        const float s = std::sin(theta);
        return {{c, s, 0.0f, 0.0f},
                {-s, c, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                SimdVec{0.0f, 0.0f, 0.0f, 1.0f}};
    }

    // eye: (x, y, z, 1)
    // target: (x, y, z, 1)
    // up: (x, y, z, 0)
    static SimdMat LookAt(const SimdVec &eye, const SimdVec &target, const SimdVec &up) {
        const SimdVec z = (eye - target).Normalize();
        const SimdVec x = up.Cross(z).Normalize();
        const SimdVec y = z.Cross(x);
        SimdMat result = SimdMat{x, y, z, {}}.Transpose();
        result.c3 = -SimdVec{x.Dot(eye), y.Dot(eye), z.Dot(eye), -1.0f};
        return result;
    }

    static SimdMat Perspective(const float fov, const float aspectRatio, const float near, const float far) {
        const float halfTan = std::tan(fov * 0.5f);
        SimdMat result{1.0f / (aspectRatio * halfTan), 0.0f, 0.0f, 0.0f,
                       0.0f, 1.0f / halfTan, 0.0f, 0.0f,
                       0.0f, 0.0f, (far + near) / (near - far), -1.0f,
                       0.0f, 0.0f, 2.0f * far * near / (near - far), 0.0f};
        return result;
    }
};

static_assert(sizeof(SimdMat) == 4 * sizeof(SimdVec));

inline SimdVec SimdVec::operator*(const SimdMat &mat) const {
    __m128 dp0 = _mm_dp_ps(mat.c0.m, m, 0xFF);
    __m128 dp1 = _mm_dp_ps(mat.c1.m, m, 0xFF);
    __m128 dp2 = _mm_dp_ps(mat.c2.m, m, 0xFF);
    __m128 dp3 = _mm_dp_ps(mat.c3.m, m, 0xFF);
    __m128 r01 = _mm_blend_ps(dp0, dp1, 0b0010);
    __m128 r23 = _mm_blend_ps(dp2, dp3, 0b1000);
    return SimdVec(_mm_blend_ps(r01, r23, 0b1100));
}
