//
// Created by andyroiiid on 3/21/2023.
//

#include <catch2/catch_test_macros.hpp>

#include "TestUtils.h"

using Catch::Matchers::WithinRel;

TEST_CASE("Construction") {
    const Quat identity;
    const Quat i({1.0f, 0.0f, 0.0f, 0.0f}, M_PI);
    const Quat j({0.0f, 1.0f, 0.0f, 0.0f}, M_PI);
    const Quat k({0.0f, 0.0f, 1.0f, 0.0f}, M_PI);
    CHECK_THAT(identity, EqualsQuat({0.0f, 0.0f, 0.0f, 1.0f}));
    CHECK_THAT(i, EqualsQuat({1.0f, 0.0f, 0.0f, 0.0f}));
    CHECK_THAT(j, EqualsQuat({0.0f, 1.0f, 0.0f, 0.0f}));
    CHECK_THAT(k, EqualsQuat({0.0f, 0.0f, 1.0f, 0.0f}));
}

TEST_CASE("Accessor") {
    Quat a{0.0f, 0.0f, 0.0f, 0.0f};

    CHECK_THAT(a.Components()[0], WithinRel(0.0f));
    CHECK_THAT(a.Components()[1], WithinRel(0.0f));
    CHECK_THAT(a.Components()[2], WithinRel(0.0f));
    CHECK_THAT(a.Components()[3], WithinRel(0.0f));

    CHECK_THAT(a[0], WithinRel(0.0f));
    CHECK_THAT(a[1], WithinRel(0.0f));
    CHECK_THAT(a[2], WithinRel(0.0f));
    CHECK_THAT(a[3], WithinRel(0.0f));

    CHECK_THAT(a.X(), WithinRel(0.0f));
    CHECK_THAT(a.Y(), WithinRel(0.0f));
    CHECK_THAT(a.Z(), WithinRel(0.0f));
    CHECK_THAT(a.W(), WithinRel(0.0f));

    a.X() = 1.0f;

    CHECK_THAT(a.Components()[0], WithinRel(1.0f));
    CHECK_THAT(a.X(), WithinRel(1.0f));
}

Mat4 ToMat4Reference(const Quat &q) {
    const float x = q.m.m128_f32[0];
    const float y = q.m.m128_f32[1];
    const float z = q.m.m128_f32[2];
    const float w = q.m.m128_f32[3];
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

TEST_CASE("Operators") {
    const Quat identity;
    const Quat i(1.0f, 0.0f, 0.0f, 0.0f);
    const Quat j(0.0f, 1.0f, 0.0f, 0.0f);
    const Quat k(0.0f, 0.0f, 1.0f, 0.0f);
    CHECK_THAT(i * identity, EqualsQuat(i));
    CHECK_THAT(identity * i, EqualsQuat(i));
    CHECK_THAT(j * identity, EqualsQuat(j));
    CHECK_THAT(identity * j, EqualsQuat(j));
    CHECK_THAT(k * identity, EqualsQuat(k));
    CHECK_THAT(identity * k, EqualsQuat(k));
    CHECK_THAT(i * i, EqualsQuat(-identity));
    CHECK_THAT(j * j, EqualsQuat(-identity));
    CHECK_THAT(k * k, EqualsQuat(-identity));
    CHECK_THAT(i * j, EqualsQuat(-j * i));
    CHECK_THAT(i * j, EqualsQuat(k));
    CHECK_THAT(j * k, EqualsQuat(-k * j));
    CHECK_THAT(j * k, EqualsQuat(i));
    CHECK_THAT(k * i, EqualsQuat(-i * k));
    CHECK_THAT(k * i, EqualsQuat(j));
    CHECK_THAT(i * j * k, EqualsQuat(-identity));
    CHECK_THAT(identity.ToMat4(), EqualsMat4(ToMat4Reference(identity)));
    CHECK_THAT(i.ToMat4(), EqualsMat4(ToMat4Reference(i)));
    CHECK_THAT(j.ToMat4(), EqualsMat4(ToMat4Reference(j)));
    CHECK_THAT(k.ToMat4(), EqualsMat4(ToMat4Reference(k)));
}
