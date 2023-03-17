//
// Created by Andrew Huang on 3/17/2023.
//

#include "TestUtils.h"

#include <sstream>

std::ostream &operator<<(std::ostream &os, const SimdVec &v) {
    return os << "{" << v.X() << ", " << v.Y() << ", " << v.Z() << ", " << v.W() << "}";
}

std::ostream &operator<<(std::ostream &os, const SimdMat &m) {
    return os << "{\n"
              << m.c0 << ",\n"
              << m.c1 << ",\n"
              << m.c2 << ",\n"
              << m.c3 << "\n}";
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

EqualsSimdMat::EqualsSimdMat(const SimdMat &mat, float epsilon)
    : mat{mat}, epsilon{epsilon} {}

bool EqualsSimdMat::match(const SimdMat &other) const {
    return EqualsSimdVec(mat.c0, epsilon).match(other.c0) &&
           EqualsSimdVec(mat.c1, epsilon).match(other.c1) &&
           EqualsSimdVec(mat.c2, epsilon).match(other.c2) &&
           EqualsSimdVec(mat.c3, epsilon).match(other.c3);
}

std::string EqualsSimdMat::describe() const {
    std::ostringstream ss;
    ss << "Equals: " << mat << " (Epsilon = " << epsilon << ")";
    return ss.str();
}
