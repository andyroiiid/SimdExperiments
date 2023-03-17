//
// Created by Andrew Huang on 3/17/2023.
//

#pragma once

#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>

#include "SimdVec.h"

std::ostream &operator<<(std::ostream &os, const SimdVec &v);

using Catch::Matchers::WithinRel;

struct EqualsSimdVec : Catch::Matchers::MatcherGenericBase {
    explicit EqualsSimdVec(const SimdVec &vec, float epsilon = std::numeric_limits<float>::epsilon() * 100);

    bool match(const SimdVec &other) const;

    std::string describe() const override;

private:
    const SimdVec &vec;
    float epsilon;
};

struct DoesNotEqualSimdVec : Catch::Matchers::MatcherGenericBase {
    explicit DoesNotEqualSimdVec(const SimdVec &vec, float epsilon = std::numeric_limits<float>::epsilon() * 100);

    bool match(const SimdVec &other) const;

    std::string describe() const override;

private:
    const SimdVec &vec;
    float epsilon;
};
