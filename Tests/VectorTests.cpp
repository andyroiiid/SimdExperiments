//
// Created by andyroiiid on 3/14/2023.
//

#include <catch2/catch_test_macros.hpp>

#include "TestUtils.h"

using Catch::Matchers::WithinRel;

TEST_CASE("Construction") {
    const Vec4 a;
    const Vec4 b{42.0f};
    const Vec4 c{1.0f, -2.0f, 3.14f, 4.0f};

    CHECK_THAT(a, EqualsVec4({0.0f, 0.0f, 0.0f, 0.0f}));
    CHECK_THAT(b, EqualsVec4({42.0f, 42.0f, 42.0f, 42.0f}));
    CHECK_THAT(c, EqualsVec4({1.0f, -2.0f, 3.14f, 4.0f}));
}

TEST_CASE("Accessor") {
    Vec4 a;

    CHECK_THAT(a.e[0], WithinRel(0.0f));
    CHECK_THAT(a.e[1], WithinRel(0.0f));
    CHECK_THAT(a.e[2], WithinRel(0.0f));
    CHECK_THAT(a.e[3], WithinRel(0.0f));

    CHECK_THAT(a[0], WithinRel(0.0f));
    CHECK_THAT(a[1], WithinRel(0.0f));
    CHECK_THAT(a[2], WithinRel(0.0f));
    CHECK_THAT(a[3], WithinRel(0.0f));

    CHECK_THAT(a.x, WithinRel(0.0f));
    CHECK_THAT(a.y, WithinRel(0.0f));
    CHECK_THAT(a.z, WithinRel(0.0f));
    CHECK_THAT(a.w, WithinRel(0.0f));

    a.x = 1.0f;

    CHECK_THAT(a.e[0], WithinRel(1.0f));
    CHECK_THAT(a.x, WithinRel(1.0f));
}

TEST_CASE("Const Accessors") {
    const Vec4 a{1.0f, 2.0f, 3.0f, 4.0f};

    CHECK_THAT(a.e[0], WithinRel(1.0f));
    CHECK_THAT(a.e[1], WithinRel(2.0f));
    CHECK_THAT(a.e[2], WithinRel(3.0f));
    CHECK_THAT(a.e[3], WithinRel(4.0f));

    CHECK_THAT(a[0], WithinRel(1.0f));
    CHECK_THAT(a[1], WithinRel(2.0f));
    CHECK_THAT(a[2], WithinRel(3.0f));
    CHECK_THAT(a[3], WithinRel(4.0f));

    CHECK_THAT(a.x, WithinRel(1.0f));
    CHECK_THAT(a.y, WithinRel(2.0f));
    CHECK_THAT(a.z, WithinRel(3.0f));
    CHECK_THAT(a.w, WithinRel(4.0f));
}

TEST_CASE("Comparators") {
    const Vec4 a{1.0f, -2.0f, 3.14f, 4.0f};

    CHECK_THAT(a, EqualsVec4({1.0f, -2.0f, 3.14f, 4.0f}));
    CHECK_THAT(a, DoesNotEqualVec4({0.0f, -2.0f, 3.14f, 4.0f}));
}

TEST_CASE("Basic Operators") {
    const Vec4 a{1.0f, 2.0f, 3.0f, 4.0f};
    const Vec4 b{2.0f, 3.0f, 4.0f, 5.0f};

    CHECK_THAT(-a, EqualsVec4({-1.0f, -2.0f, -3.0f, -4.0f}));
    CHECK_THAT(a + b, EqualsVec4({3.0f, 5.0f, 7.0f, 9.0f}));
    CHECK_THAT(a - b, EqualsVec4({-1.0f, -1.0f, -1.0f, -1.0f}));
    CHECK_THAT(a * b, EqualsVec4({2.0f, 6.0f, 12.0f, 20.0f}));
    CHECK_THAT(a / b, EqualsVec4({0.5f, 0.6666666666666666f, 0.75f, 0.8f}));
}

TEST_CASE("Geometric Functions") {
    const Vec4 a{1.0f, 2.0f, 3.0f, 4.0f};
    const Vec4 b{2.0f, 3.0f, 4.0f, 5.0f};

    CHECK_THAT(a.Dot(b), WithinRel(40.0f));
    CHECK_THAT(b.Dot(a), WithinRel(40.0f));

    CHECK_THAT(a.Length(), WithinRel(5.477225575051661f));
    CHECK_THAT(b.Length(), WithinRel(7.3484692283495345f));

    CHECK_THAT(a.Distance(b), WithinRel(2.0f));
    CHECK_THAT(b.Distance(a), WithinRel(2.0f));

    const Vec4 aNormalized = Vec4{0.18257418583505536f, 0.3651483716701107f, 0.5477225575051661f, 0.7302967433402214f};
    CHECK_THAT(a.Normalize(), EqualsVec4(aNormalized));
    CHECK_THAT(a.FastNormalize(), EqualsVec4(aNormalized, 0.0003662109375));

    const Vec4 c{1.0f, 2.0f, 3.0f, 0.0f};
    const Vec4 d{2.0f, 3.0f, 4.0f, 0.0f};
    CHECK_THAT(c.Cross(d), EqualsVec4({-1.0f, 2.0f, -1.0f, 0.0f}));

    const Vec4 i{1.0f, 0.0f, 0.0f, 0.0f};
    const Vec4 j{0.0f, 1.0f, 0.0f, 0.0f};
    const Vec4 k{0.0f, 0.0f, 1.0f, 0.0f};
    CHECK_THAT(i.Cross(i), EqualsVec4({}));
    CHECK_THAT(j.Cross(j), EqualsVec4({}));
    CHECK_THAT(k.Cross(k), EqualsVec4({}));
    CHECK_THAT(i.Cross(j), EqualsVec4(k));
    CHECK_THAT(j.Cross(k), EqualsVec4(i));
    CHECK_THAT(k.Cross(i), EqualsVec4(j));
    CHECK_THAT(j.Cross(i), EqualsVec4(-k));
    CHECK_THAT(k.Cross(j), EqualsVec4(-i));
    CHECK_THAT(i.Cross(k), EqualsVec4(-j));
}
