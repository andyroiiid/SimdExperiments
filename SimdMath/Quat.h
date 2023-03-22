//
// Created by andyroiiid on 3/21/2023.
//

#pragma once

#include <cmath>

#include "Mat4.h"

// x: data[31:0], m128_f32[0]
// y: data[63:32], m128_f32[1]
// z: data[95:64], m128_f32[2]
// w: data[127:96], m128_f32[3]
//
// Requires SSE4.1
struct Quat {
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

    float *Components() { return reinterpret_cast<float *>(this); }
    float &operator[](size_t i) { return Components()[i]; }
    float &X() { return Components()[0]; }
    float &Y() { return Components()[1]; }
    float &Z() { return Components()[2]; }
    float &W() { return Components()[3]; }

    // Const Accessors

    [[nodiscard]] const float *Components() const { return reinterpret_cast<const float *>(this); }
    [[nodiscard]] const float &operator[](size_t i) const { return Components()[i]; }
    [[nodiscard]] const float &X() const { return Components()[0]; }
    [[nodiscard]] const float &Y() const { return Components()[1]; }
    [[nodiscard]] const float &Z() const { return Components()[2]; }
    [[nodiscard]] const float &W() const { return Components()[3]; }

    // Operators

    const Quat &operator+() const {
        return *this;
    }

    Quat operator-() const {
        return Quat{_mm_sub_ps(_mm_setzero_ps(), m)};
    }

    [[nodiscard]] Quat operator*(const Quat &q) const {
        const float x1 = m.m128_f32[0];
        const float y1 = m.m128_f32[1];
        const float z1 = m.m128_f32[2];
        const float w1 = m.m128_f32[3];
        const float x2 = q.m.m128_f32[0];
        const float y2 = q.m.m128_f32[1];
        const float z2 = q.m.m128_f32[2];
        const float w2 = q.m.m128_f32[3];
        return {w1 * x2 + x1 * w2 + y1 * z2 - z1 * y2,
                w1 * y2 - x1 * z2 + y1 * w2 + z1 * x2,
                w1 * z2 + x1 * y2 - y1 * x2 + z1 * w2,
                w1 * w2 - x1 * x2 - y1 * y2 - z1 * z2};
    }

    [[nodiscard]] Mat4 ToMat4() const {
        const float x = m.m128_f32[0];
        const float y = m.m128_f32[1];
        const float z = m.m128_f32[2];
        const float w = m.m128_f32[3];
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
};

static_assert(sizeof(Quat) == sizeof(__m128));
