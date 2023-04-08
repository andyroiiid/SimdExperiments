//
// Created by andyroiiid on 3/21/2023.
//

#pragma once

#include <cmath>
#include <immintrin.h>

#include "Mat4.h"

// x: data[31:0], m128_f32[0]
// y: data[63:32], m128_f32[1]
// z: data[95:64], m128_f32[2]
// w: data[127:96], m128_f32[3]
union alignas(16) Quat {
    // Constructors

    Quat() : Quat(0.0f, 0.0f, 0.0f, 1.0f) {}

    explicit Quat(__m128 v) : m(v) {}

    Quat(const Vec4 &axis, float theta) {
        const float halfTheta = theta * 0.5f;
        const float s = std::sin(halfTheta);
        const float c = std::cos(halfTheta);
        m = _mm_blend_ps(_mm_mul_ps(_mm_set_ps1(s), axis.Normalize().m), _mm_set_ps1(c), 0b1000);
    }

    Quat(float x, float y, float z, float w) : Quat(_mm_set_ps(w, z, y, x)) {}

    // Accessors

    float &operator[](size_t i) { return e[i]; }

    // Const Accessors

    [[nodiscard]] const float &operator[](size_t i) const { return e[i]; }

    // Operators

    const Quat &operator+() const {
        return *this;
    }

    Quat operator-() const {
        return Quat{_mm_sub_ps(_mm_setzero_ps(), m)};
    }

    [[nodiscard]] Quat operator*(const Quat &q) const {
        return {w * q.x + x * q.w + y * q.z - z * q.y,
                w * q.y - x * q.z + y * q.w + z * q.x,
                w * q.z + x * q.y - y * q.x + z * q.w,
                w * q.w - x * q.x - y * q.y - z * q.z};
    }

    [[nodiscard]] Mat4 ToMat4() const {
        const float x2 = x * x;
        const float y2 = y * y;
        const float z2 = z * z;
        const float xy = x * y;
        const float yz = y * z;
        const float xz = x * z;
        const float wx = w * x;
        const float wy = w * y;
        const float wz = w * z;
        return {1 - 2 * y2 - 2 * z2, 2 * xy - 2 * wz, 2 * xz + 2 * wy, 0,
                2 * xy + 2 * wz, 1 - 2 * x2 - 2 * z2, 2 * yz - 2 * wx, 0,
                2 * xz - 2 * wy, 2 * yz + 2 * wx, 1 - 2 * x2 - 2 * y2, 0,
                0, 0, 0, 1};
    }

    __m128 m{};
    struct {
        float x;
        float y;
        float z;
        float w;
    };
    float e[4];
};

static_assert(sizeof(Quat) == sizeof(__m128));
