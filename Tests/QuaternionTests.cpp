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
}
