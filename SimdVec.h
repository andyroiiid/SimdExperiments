//
// Created by andyroiiid on 3/14/2023.
//

#pragma once

#include <smmintrin.h>
#include <xmmintrin.h>

// x: data[31:0], m128_f32[0]
// y: data[63:32], m128_f32[1]
// z: data[95:64], m128_f32[2]
// w: data[127:96], m128_f32[3]
//
// Requires SSE4.1
class SimdVec {
public:
    // Constructors

    SimdVec() : SimdVec(_mm_setzero_ps()) {}

    explicit SimdVec(__m128 v) : m(v) {}

    explicit SimdVec(float a) : SimdVec(_mm_set_ps1(a)) {}

    SimdVec(float x, float y, float z, float w) : SimdVec(_mm_set_ps(w, z, y, x)) {}

    // Accessors

    float *Components() { return reinterpret_cast<float *>(&m); }
    float &operator[](size_t i) { return Components()[i]; }
    float &X() { return Components()[0]; }
    float &Y() { return Components()[1]; }
    float &Z() { return Components()[2]; }
    float &W() { return Components()[3]; }

    // Const Accessors

    [[nodiscard]] const float *Components() const { return reinterpret_cast<const float *>(&m); }
    [[nodiscard]] const float &operator[](size_t i) const { return Components()[i]; }
    [[nodiscard]] const float &X() const { return Components()[0]; }
    [[nodiscard]] const float &Y() const { return Components()[1]; }
    [[nodiscard]] const float &Z() const { return Components()[2]; }
    [[nodiscard]] const float &W() const { return Components()[3]; }

    // Arithmetic Operators

    const SimdVec &operator+() const {
        return *this;
    }

    SimdVec operator-() const {
        return SimdVec{_mm_sub_ps(_mm_setzero_ps(), m)};
    }

    SimdVec operator+(const SimdVec &v) const {
        return SimdVec{_mm_add_ps(m, v.m)};
    }

    SimdVec operator-(const SimdVec &v) const {
        return SimdVec{_mm_sub_ps(m, v.m)};
    }

    SimdVec operator*(const SimdVec &v) const {
        return SimdVec{_mm_mul_ps(m, v.m)};
    }

    SimdVec operator/(const SimdVec &v) const {
        return SimdVec{_mm_div_ps(m, v.m)};
    }

    // Geometric Functions
    [[nodiscard]] float Dot(const SimdVec &v) const {
        return SimdVec{_mm_dp_ps(m, v.m, 0xFF)}.X();
    }

    [[nodiscard]] float Length() const {
        return SimdVec{_mm_sqrt_ps(_mm_dp_ps(m, m, 0xFF))}.X();
    }

    [[nodiscard]] float Distance(const SimdVec &v) const {
        __m128 delta = _mm_sub_ps(m, v.m);
        __m128 lenSqr = _mm_dp_ps(delta, delta, 0xFF);
        return SimdVec{_mm_sqrt_ps(lenSqr)}.X();
    }

    [[nodiscard]] SimdVec Normalize() const {
        __m128 lenSqr = _mm_dp_ps(m, m, 0xFF);
        __m128 len = _mm_sqrt_ps(lenSqr);
        return SimdVec{_mm_div_ps(m, len)};
    }

    // Maximum relative error: 1.5*2^-12 (about 0.0003662109375)
    [[nodiscard]] SimdVec FastNormalize() const {
        __m128 lenSqr = _mm_dp_ps(m, m, 0xFF);
        __m128 revLen = _mm_rsqrt_ps(lenSqr);
        return SimdVec{_mm_mul_ps(m, revLen)};
    }

    [[nodiscard]] SimdVec Cross(const SimdVec &v) const {
        __m128 a2a3a1a4 = _mm_shuffle_ps(m, m, 0xC9);
        __m128 b3b1b2b4 = _mm_shuffle_ps(v.m, v.m, 0xD2);
        __m128 a3a1a2a4 = _mm_shuffle_ps(m, m, 0xD2);
        __m128 b2b3b1b4 = _mm_shuffle_ps(v.m, v.m, 0xC9);
        return SimdVec{_mm_sub_ps(_mm_mul_ps(a2a3a1a4, b3b1b2b4), _mm_mul_ps(a3a1a2a4, b2b3b1b4))};
    }

    __m128 m;
};

static_assert(sizeof(SimdVec) == sizeof(__m128));
