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

bool EqualsSimdVec::match(const glm::vec4 &other) const {
    return WithinRel(vec.X(), epsilon).match(other.x) &&
           WithinRel(vec.Y(), epsilon).match(other.y) &&
           WithinRel(vec.Z(), epsilon).match(other.z) &&
           WithinRel(vec.W(), epsilon).match(other.w);
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

bool EqualsSimdMat::match(const glm::mat4 &other) const {
    return WithinRel(mat[0], epsilon).match(other[0][0]) &&
           WithinRel(mat[1], epsilon).match(other[0][1]) &&
           WithinRel(mat[2], epsilon).match(other[0][2]) &&
           WithinRel(mat[3], epsilon).match(other[0][3]) &&
           WithinRel(mat[4], epsilon).match(other[1][0]) &&
           WithinRel(mat[5], epsilon).match(other[1][1]) &&
           WithinRel(mat[6], epsilon).match(other[1][2]) &&
           WithinRel(mat[7], epsilon).match(other[1][3]) &&
           WithinRel(mat[8], epsilon).match(other[2][0]) &&
           WithinRel(mat[9], epsilon).match(other[2][1]) &&
           WithinRel(mat[10], epsilon).match(other[2][2]) &&
           WithinRel(mat[11], epsilon).match(other[2][3]) &&
           WithinRel(mat[12], epsilon).match(other[3][0]) &&
           WithinRel(mat[13], epsilon).match(other[3][1]) &&
           WithinRel(mat[14], epsilon).match(other[3][2]) &&
           WithinRel(mat[15], epsilon).match(other[3][3]);
}
