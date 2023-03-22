//
// Created by Andrew Huang on 3/17/2023.
//

#pragma once

#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>
#include <glm/mat4x4.hpp>

#include "Mat4.h"
#include "Vec4.h"

std::ostream &operator<<(std::ostream &os, const Vec4 &v);

std::ostream &operator<<(std::ostream &os, const Mat4 &m);

using Catch::Matchers::WithinAbs;

struct EqualsSimdVec : Catch::Matchers::MatcherGenericBase {
    explicit EqualsSimdVec(const Vec4 &vec, float epsilon = std::numeric_limits<float>::epsilon() * 100);

    bool match(const Vec4 &other) const;

    bool match(const glm::vec4 &other) const;

    std::string describe() const override;

private:
    const Vec4 &vec;
    float epsilon;
};

struct DoesNotEqualSimdVec : Catch::Matchers::MatcherGenericBase {
    explicit DoesNotEqualSimdVec(const Vec4 &vec, float epsilon = std::numeric_limits<float>::epsilon() * 100);

    bool match(const Vec4 &other) const;

    std::string describe() const override;

private:
    const Vec4 &vec;
    float epsilon;
};

struct EqualsSimdMat : Catch::Matchers::MatcherGenericBase {
    explicit EqualsSimdMat(const Mat4 &mat, float epsilon = std::numeric_limits<float>::epsilon() * 100);

    bool match(const Mat4 &other) const;

    bool match(const glm::mat4 &other) const;

    std::string describe() const override;

private:
    const Mat4 &mat;
    float epsilon;
};
