//
// Created by andyroiiid on 3/24/2023.
//

#pragma once

#include <cmath>

struct alignas(16) PlainVec {
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

    float x;
    float y;
    float z;
    float w;
};
