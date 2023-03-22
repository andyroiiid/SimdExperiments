//
// Created by Andrew Huang on 3/17/2023.
//

#pragma once

#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_templated.hpp>
#include <glm/mat4x4.hpp>

#include "Mat4.h"
#include "Quat.h"
#include "Vec4.h"

std::ostream &operator<<(std::ostream &os, const Vec4 &v);

std::ostream &operator<<(std::ostream &os, const Mat4 &m);

std::ostream &operator<<(std::ostream &os, const Quat &v);

using Catch::Matchers::WithinAbs;

struct EqualsVec4 : Catch::Matchers::MatcherGenericBase {
    explicit EqualsVec4(const Vec4 &vec, float epsilon = std::numeric_limits<float>::epsilon() * 100);

    bool match(const Vec4 &other) const;

    bool match(const glm::vec4 &other) const;

    std::string describe() const override;

private:
    const Vec4 &vec;
    float epsilon;
};

struct DoesNotEqualVec4 : Catch::Matchers::MatcherGenericBase {
    explicit DoesNotEqualVec4(const Vec4 &vec, float epsilon = std::numeric_limits<float>::epsilon() * 100);

    bool match(const Vec4 &other) const;

    std::string describe() const override;

private:
    const Vec4 &vec;
    float epsilon;
};

struct EqualsMat4 : Catch::Matchers::MatcherGenericBase {
    explicit EqualsMat4(const Mat4 &mat, float epsilon = std::numeric_limits<float>::epsilon() * 100);

    bool match(const Mat4 &other) const;

    bool match(const glm::mat4 &other) const;

    std::string describe() const override;

private:
    const Mat4 &mat;
    float epsilon;
};

struct EqualsQuat : Catch::Matchers::MatcherGenericBase {
    explicit EqualsQuat(const Quat &quat, float epsilon = std::numeric_limits<float>::epsilon() * 100);

    bool match(const Quat &other) const;

    std::string describe() const override;

private:
    const Quat &quat;
    float epsilon;
};
