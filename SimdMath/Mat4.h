//
// Created by Andrew Huang on 3/17/2023.
//

#pragma once

#include <cmath>

#include "Vec4.h"

// c0: m00, m10, m20, m30
// c1: m01, m11, m21, m31
// c2: m02, m12, m22, m32
// c3: m03, m13, m23, m33
//
// Column-major
struct Mat4 {
    // Constructors

    Mat4()
        : c0{1.0f, 0.0f, 0.0f, 0.0f},
          c1{0.0f, 1.0f, 0.0f, 0.0f},
          c2{0.0f, 0.0f, 1.0f, 0.0f},
          c3{0.0f, 0.0f, 0.0f, 1.0f} {}

    Mat4(const Vec4 &c0, const Vec4 &c1, const Vec4 &c2, const Vec4 &c3)
        : c0{c0}, c1{c1}, c2{c2}, c3{c3} {}

    Mat4(__m128 c0, __m128 c1, __m128 c2, __m128 c3)
        : c0{c0}, c1{c1}, c2{c2}, c3{c3} {}

    Mat4(float m00, float m10, float m20, float m30,
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

    [[nodiscard]] Mat4 Transpose() const {
        __m128 m0 = c0.m;
        __m128 m1 = c1.m;
        __m128 m2 = c2.m;
        __m128 m3 = c3.m;
        _MM_TRANSPOSE4_PS(m0, m1, m2, m3)
        return {m0, m1, m2, m3};
    }

    Vec4 operator*(const Vec4 &v) const {
        const Vec4 v0{_mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(0, 0, 0, 0))};
        const Vec4 v1{_mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(1, 1, 1, 1))};
        const Vec4 v2{_mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(2, 2, 2, 2))};
        const Vec4 v3{_mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(3, 3, 3, 3))};
        return c0 * v0 + c1 * v1 + c2 * v2 + c3 * v3;
    }

    Mat4 operator*(const Mat4 &m) const {
        return {operator*(m.c0), operator*(m.c1), operator*(m.c2), operator*(m.c3)};
    }

    Vec4 c0;
    Vec4 c1;
    Vec4 c2;
    Vec4 c3;

    static Mat4 Translate(const Vec4 &translation) {
        return {{1.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                translation};
    }

    static Mat4 Scale(const float scale) {
        return {{scale, 0.0f, 0.0f, 0.0f},
                {0.0f, scale, 0.0f, 0.0f},
                {0.0f, 0.0f, scale, 0.0f},
                Vec4{0.0f, 0.0f, 0.0f, 1.0f}};
    }

    static Mat4 Scale(const Vec4 &scale) {
        return {{scale.X(), 0.0f, 0.0f, 0.0f},
                {0.0f, scale.Y(), 0.0f, 0.0f},
                {0.0f, 0.0f, scale.Z(), 0.0f},
                Vec4{0.0f, 0.0f, 0.0f, 1.0f}};
    }

    static Mat4 RotateX(const float theta) {
        const float c = std::cos(theta);
        const float s = std::sin(theta);
        return {{1.0f, 0.0f, 0.0f, 0.0f},
                {0.0f, c, s, 0.0f},
                {0.0f, -s, c, 0.0f},
                Vec4{0.0f, 0.0f, 0.0f, 1.0f}};
    }

    static Mat4 RotateY(const float theta) {
        const float c = std::cos(theta);
        const float s = std::sin(theta);
        return {{c, 0.0f, -s, 0.0f},
                {0.0f, 1.0f, 0.0f, 0.0f},
                {s, 0.0f, c, 0.0f},
                Vec4{0.0f, 0.0f, 0.0f, 1.0f}};
    }

    static Mat4 RotateZ(const float theta) {
        const float c = std::cos(theta);
        const float s = std::sin(theta);
        return {{c, s, 0.0f, 0.0f},
                {-s, c, 0.0f, 0.0f},
                {0.0f, 0.0f, 1.0f, 0.0f},
                Vec4{0.0f, 0.0f, 0.0f, 1.0f}};
    }

    // eye: (x, y, z, 1)
    // target: (x, y, z, 1)
    // up: (x, y, z, 0)
    static Mat4 LookAt(const Vec4 &eye, const Vec4 &target, const Vec4 &up) {
        const Vec4 z = (eye - target).Normalize();
        const Vec4 x = up.Cross(z).Normalize();
        const Vec4 y = z.Cross(x);
        Mat4 result = Mat4{x, y, z, {}}.Transpose();
        result.c3 = -Vec4{x.Dot(eye), y.Dot(eye), z.Dot(eye), -1.0f};
        return result;
    }

    static Mat4 Perspective(const float fov, const float aspectRatio, const float near, const float far) {
        const float halfTan = std::tan(fov * 0.5f);
        Mat4 result{1.0f / (aspectRatio * halfTan), 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f / halfTan, 0.0f, 0.0f,
                    0.0f, 0.0f, (far + near) / (near - far), -1.0f,
                    0.0f, 0.0f, 2.0f * far * near / (near - far), 0.0f};
        return result;
    }
};

static_assert(sizeof(Mat4) == 4 * sizeof(Vec4));

inline Vec4 Vec4::operator*(const Mat4 &mat) const {
    __m128 dp0 = _mm_dp_ps(mat.c0.m, m, 0xFF);
    __m128 dp1 = _mm_dp_ps(mat.c1.m, m, 0xFF);
    __m128 dp2 = _mm_dp_ps(mat.c2.m, m, 0xFF);
    __m128 dp3 = _mm_dp_ps(mat.c3.m, m, 0xFF);
    __m128 r01 = _mm_blend_ps(dp0, dp1, 0b0010);
    __m128 r23 = _mm_blend_ps(dp2, dp3, 0b1000);
    return Vec4(_mm_blend_ps(r01, r23, 0b1100));
}
