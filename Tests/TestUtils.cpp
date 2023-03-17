//
// Created by Andrew Huang on 3/17/2023.
//

#include "TestUtils.h"

#include <sstream>

std::ostream &operator<<(std::ostream &os, const SimdVec &v) {
    return os << "{" << v.X() << ", " << v.Y() << ", " << v.Z() << ", " << v.W() << "}";
}

EqualsSimdVec::EqualsSimdVec(const SimdVec &vec, float epsilon)
    : vec{vec}, epsilon{epsilon} {}

bool EqualsSimdVec::match(const SimdVec &other) const {
    return WithinRel(vec.X(), epsilon).match(other.X()) &&
           WithinRel(vec.Y(), epsilon).match(other.Y()) &&
           WithinRel(vec.Z(), epsilon).match(other.Z()) &&
           WithinRel(vec.W(), epsilon).match(other.W());
}

std::string EqualsSimdVec::describe() const {
    std::ostringstream ss;
    ss << "Equals: " << vec << " (Epsilon = " << epsilon << ")";
    return ss.str();
}

DoesNotEqualSimdVec::DoesNotEqualSimdVec(const SimdVec &vec, float epsilon)
    : vec{vec}, epsilon{epsilon} {}

bool DoesNotEqualSimdVec::match(const SimdVec &other) const {
    return !WithinRel(vec.X(), epsilon).match(other.X()) ||
           !WithinRel(vec.Y(), epsilon).match(other.Y()) ||
           !WithinRel(vec.Z(), epsilon).match(other.Z()) ||
           !WithinRel(vec.W(), epsilon).match(other.W());
}

std::string DoesNotEqualSimdVec::describe() const {
    std::ostringstream ss;
    ss << "Does not equal: " << vec << " (Epsilon = " << epsilon << ")";
    return ss.str();
}
