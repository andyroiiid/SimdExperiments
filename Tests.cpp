//
// Created by andyroiiid on 3/14/2023.
//

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>
#include <sstream>

#include "SimdVec.h"

std::ostream &operator<<(std::ostream &os, const SimdVec &v) {
    return os << "{" << v.X() << ", " << v.Y() << ", " << v.Z() << ", " << v.W() << "}";
}

using Catch::Matchers::WithinRel;

struct EqualsSimdVec : Catch::Matchers::MatcherGenericBase {
    explicit EqualsSimdVec(const SimdVec &vec, float epsilon = std::numeric_limits<float>::epsilon() * 100) : vec{vec}, epsilon{epsilon} {}

    bool match(const SimdVec &other) const {
        return WithinRel(vec.X(), epsilon).match(other.X()) &&
               WithinRel(vec.Y(), epsilon).match(other.Y()) &&
               WithinRel(vec.Z(), epsilon).match(other.Z()) &&
               WithinRel(vec.W(), epsilon).match(other.W());
    }

    std::string describe() const override {
        std::ostringstream ss;
        ss << "Equals: " << vec << " (Epsilon = " << epsilon << ")";
        return ss.str();
    }

private:
    const SimdVec &vec;
    float epsilon;
};

struct DoesNotEqualSimdVec : Catch::Matchers::MatcherGenericBase {
    explicit DoesNotEqualSimdVec(const SimdVec &vec, float epsilon = std::numeric_limits<float>::epsilon() * 100) : vec{vec}, epsilon{epsilon} {}

    bool match(const SimdVec &other) const {
        return !WithinRel(vec.X(), epsilon).match(other.X()) ||
               !WithinRel(vec.Y(), epsilon).match(other.Y()) ||
               !WithinRel(vec.Z(), epsilon).match(other.Z()) ||
               !WithinRel(vec.W(), epsilon).match(other.W());
    }

    std::string describe() const override {
        std::ostringstream ss;
        ss << "Does not equal: " << vec << " (Epsilon = " << epsilon << ")";
        return ss.str();
    }

private:
    const SimdVec &vec;
    float epsilon;
};

TEST_CASE("Construction") {
    const SimdVec a;
    const SimdVec b{42.0f};
    const SimdVec c{1.0f, -2.0f, 3.14f, 4.0f};

    CHECK_THAT(a, EqualsSimdVec({0.0f, 0.0f, 0.0f, 0.0f}));
    CHECK_THAT(b, EqualsSimdVec({42.0f, 42.0f, 42.0f, 42.0f}));
    CHECK_THAT(c, EqualsSimdVec({1.0f, -2.0f, 3.14f, 4.0f}));
}

TEST_CASE("Accessor") {
    SimdVec a;

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

TEST_CASE("Const Accessors") {
    const SimdVec a{1.0f, 2.0f, 3.0f, 4.0f};

    CHECK_THAT(a.Components()[0], WithinRel(1.0f));
    CHECK_THAT(a.Components()[1], WithinRel(2.0f));
    CHECK_THAT(a.Components()[2], WithinRel(3.0f));
    CHECK_THAT(a.Components()[3], WithinRel(4.0f));

    CHECK_THAT(a[0], WithinRel(1.0f));
    CHECK_THAT(a[1], WithinRel(2.0f));
    CHECK_THAT(a[2], WithinRel(3.0f));
    CHECK_THAT(a[3], WithinRel(4.0f));

    CHECK_THAT(a.X(), WithinRel(1.0f));
    CHECK_THAT(a.Y(), WithinRel(2.0f));
    CHECK_THAT(a.Z(), WithinRel(3.0f));
    CHECK_THAT(a.W(), WithinRel(4.0f));
}

TEST_CASE("Comparators") {
    const SimdVec a{1.0f, -2.0f, 3.14f, 4.0f};

    CHECK_THAT(a, EqualsSimdVec({1.0f, -2.0f, 3.14f, 4.0f}));
    CHECK_THAT(a, DoesNotEqualSimdVec({0.0f, -2.0f, 3.14f, 4.0f}));
}

TEST_CASE("Basic Operators") {
    const SimdVec a{1.0f, 2.0f, 3.0f, 4.0f};
    const SimdVec b{2.0f, 3.0f, 4.0f, 5.0f};

    CHECK_THAT(-a, EqualsSimdVec({-1.0f, -2.0f, -3.0f, -4.0f}));
    CHECK_THAT(a + b, EqualsSimdVec({3.0f, 5.0f, 7.0f, 9.0f}));
    CHECK_THAT(a - b, EqualsSimdVec({-1.0f, -1.0f, -1.0f, -1.0f}));
    CHECK_THAT(a * b, EqualsSimdVec({2.0f, 6.0f, 12.0f, 20.0f}));
    CHECK_THAT(a / b, EqualsSimdVec({0.5f, 0.6666666666666666f, 0.75f, 0.8f}));
}

TEST_CASE("Geometric Functions") {
    const SimdVec a{1.0f, 2.0f, 3.0f, 4.0f};
    const SimdVec b{2.0f, 3.0f, 4.0f, 5.0f};

    CHECK_THAT(a.Dot(b), WithinRel(40.0f));
    CHECK_THAT(b.Dot(a), WithinRel(40.0f));

    CHECK_THAT(a.Length(), WithinRel(5.477225575051661f));
    CHECK_THAT(b.Length(), WithinRel(7.3484692283495345f));

    CHECK_THAT(a.Distance(b), WithinRel(2.0f));
    CHECK_THAT(b.Distance(a), WithinRel(2.0f));

    const SimdVec aNormalized = SimdVec{0.18257418583505536f, 0.3651483716701107f, 0.5477225575051661f, 0.7302967433402214f};
    CHECK_THAT(a.Normalize(), EqualsSimdVec(aNormalized));
    CHECK_THAT(a.FastNormalize(), EqualsSimdVec(aNormalized, 0.0003662109375));

    const SimdVec c{1.0f, 2.0f, 3.0f, 0.0f};
    const SimdVec d{2.0f, 3.0f, 4.0f, 0.0f};
    CHECK_THAT(c.Cross(d), EqualsSimdVec({-1.0f, 2.0f, -1.0f, 0.0f}));

    const SimdVec i{1.0f, 0.0f, 0.0f, 0.0f};
    const SimdVec j{0.0f, 1.0f, 0.0f, 0.0f};
    const SimdVec k{0.0f, 0.0f, 1.0f, 0.0f};
    CHECK_THAT(i.Cross(i), EqualsSimdVec({}));
    CHECK_THAT(j.Cross(j), EqualsSimdVec({}));
    CHECK_THAT(k.Cross(k), EqualsSimdVec({}));
    CHECK_THAT(i.Cross(j), EqualsSimdVec(k));
    CHECK_THAT(j.Cross(k), EqualsSimdVec(i));
    CHECK_THAT(k.Cross(i), EqualsSimdVec(j));
    CHECK_THAT(j.Cross(i), EqualsSimdVec(-k));
    CHECK_THAT(k.Cross(j), EqualsSimdVec(-i));
    CHECK_THAT(i.Cross(k), EqualsSimdVec(-j));
}
