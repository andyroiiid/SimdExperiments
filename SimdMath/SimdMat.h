//
// Created by Andrew Huang on 3/17/2023.
//

#pragma once

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
        : c0(c0), c1(c1), c2(c2), c3(c3) {}

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

    SimdVec c0;
    SimdVec c1;
    SimdVec c2;
    SimdVec c3;
};

static_assert(sizeof(SimdMat) == 4 * sizeof(SimdVec));
