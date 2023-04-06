//
// Created by andyroiiid on 3/14/2023.
//

#pragma once

#include <smmintrin.h>
#include <xmmintrin.h>

union Mat4;

// x: data[31:0], m128_f32[0]
// y: data[63:32], m128_f32[1]
// z: data[95:64], m128_f32[2]
// w: data[127:96], m128_f32[3]
//
// Requires SSE4.1
union alignas(16) Vec4 {
    // Constructors

    Vec4() : Vec4(_mm_setzero_ps()) {}

    explicit Vec4(__m128 v) : m(v) {}

    explicit Vec4(float a) : Vec4(_mm_set_ps1(a)) {}

    Vec4(float x, float y, float z, float w) : Vec4(_mm_set_ps(w, z, y, x)) {}

    // Accessors

    float &operator[](size_t i) { return e[i]; }

    // Const Accessors

    [[nodiscard]] const float &operator[](size_t i) const { return e[i]; }

    // Arithmetic Operators

    const Vec4 &operator+() const {
        return *this;
    }

    Vec4 operator-() const {
        return Vec4{_mm_sub_ps(_mm_setzero_ps(), m)};
    }

    Vec4 operator+(const Vec4 &v) const {
        return Vec4{_mm_add_ps(m, v.m)};
    }

    Vec4 operator-(const Vec4 &v) const {
        return Vec4{_mm_sub_ps(m, v.m)};
    }

    Vec4 operator*(const Vec4 &v) const {
        return Vec4{_mm_mul_ps(m, v.m)};
    }

    Vec4 operator*(const Mat4 &mat) const;

    Vec4 operator/(const Vec4 &v) const {
        return Vec4{_mm_div_ps(m, v.m)};
    }

    // Geometric Functions

    [[nodiscard]] float Dot(const Vec4 &v) const {
        return _mm_cvtss_f32(_mm_dp_ps(m, v.m, 0xFF));
    }

    [[nodiscard]] float Length() const {
        __m128 lenSqr = _mm_dp_ps(m, m, 0xFF);
        return _mm_cvtss_f32(_mm_sqrt_ps(lenSqr));
    }

    [[nodiscard]] float Distance(const Vec4 &v) const {
        __m128 delta = _mm_sub_ps(m, v.m);
        __m128 lenSqr = _mm_dp_ps(delta, delta, 0xFF);
        return _mm_cvtss_f32(_mm_sqrt_ps(lenSqr));
    }

    [[nodiscard]] Vec4 Normalize() const {
        __m128 lenSqr = _mm_dp_ps(m, m, 0xFF);
        __m128 len = _mm_sqrt_ps(lenSqr);
        return Vec4{_mm_div_ps(m, len)};
    }

    // Maximum relative error: 1.5*2^-12 (about 0.0003662109375)
    [[nodiscard]] Vec4 FastNormalize() const {
        __m128 lenSqr = _mm_dp_ps(m, m, 0xFF);
        __m128 revLen = _mm_rsqrt_ps(lenSqr);
        return Vec4{_mm_mul_ps(m, revLen)};
    }

    [[nodiscard]] Vec4 Cross(const Vec4 &v) const {
        __m128 a2a3a1a4 = _mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 a3a1a2a4 = _mm_shuffle_ps(m, m, _MM_SHUFFLE(3, 1, 0, 2));
        __m128 b2b3b1b4 = _mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(3, 0, 2, 1));
        __m128 b3b1b2b4 = _mm_shuffle_ps(v.m, v.m, _MM_SHUFFLE(3, 1, 0, 2));
        return Vec4{_mm_sub_ps(_mm_mul_ps(a2a3a1a4, b3b1b2b4), _mm_mul_ps(a3a1a2a4, b2b3b1b4))};
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

static_assert(sizeof(Vec4) == sizeof(__m128));
