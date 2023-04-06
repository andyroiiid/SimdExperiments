//
// Created by Andrew Huang on 3/17/2023.
//

#include "TestUtils.h"

#include <sstream>

std::ostream &operator<<(std::ostream &os, const Vec4 &v) {
    return os << "{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}";
}

std::ostream &operator<<(std::ostream &os, const Mat4 &m) {
    return os << "{\n"
              << m.c0 << ",\n"
              << m.c1 << ",\n"
              << m.c2 << ",\n"
              << m.c3 << "\n}";
}

std::ostream &operator<<(std::ostream &os, const Quat &v) {
    return os << "{" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "}";
}

EqualsVec4::EqualsVec4(const Vec4 &vec, float epsilon)
    : vec{vec}, epsilon{epsilon} {}

bool EqualsVec4::match(const Vec4 &other) const {
    return WithinAbs(vec.x, epsilon).match(other.x) &&
           WithinAbs(vec.y, epsilon).match(other.y) &&
           WithinAbs(vec.z, epsilon).match(other.z) &&
           WithinAbs(vec.w, epsilon).match(other.w);
}

std::string EqualsVec4::describe() const {
    std::ostringstream ss;
    ss << "Equals: " << vec << " (Epsilon = " << epsilon << ")";
    return ss.str();
}

bool EqualsVec4::match(const glm::vec4 &other) const {
    return WithinAbs(vec.x, epsilon).match(other.x) &&
           WithinAbs(vec.y, epsilon).match(other.y) &&
           WithinAbs(vec.z, epsilon).match(other.z) &&
           WithinAbs(vec.w, epsilon).match(other.w);
}

DoesNotEqualVec4::DoesNotEqualVec4(const Vec4 &vec, float epsilon)
    : vec{vec}, epsilon{epsilon} {}

bool DoesNotEqualVec4::match(const Vec4 &other) const {
    return !WithinAbs(vec.x, epsilon).match(other.x) ||
           !WithinAbs(vec.y, epsilon).match(other.y) ||
           !WithinAbs(vec.z, epsilon).match(other.z) ||
           !WithinAbs(vec.w, epsilon).match(other.w);
}

std::string DoesNotEqualVec4::describe() const {
    std::ostringstream ss;
    ss << "Does not equal: " << vec << " (Epsilon = " << epsilon << ")";
    return ss.str();
}

EqualsMat4::EqualsMat4(const Mat4 &mat, float epsilon)
    : mat{mat}, epsilon{epsilon} {}

bool EqualsMat4::match(const Mat4 &other) const {
    return EqualsVec4(mat.c0, epsilon).match(other.c0) &&
           EqualsVec4(mat.c1, epsilon).match(other.c1) &&
           EqualsVec4(mat.c2, epsilon).match(other.c2) &&
           EqualsVec4(mat.c3, epsilon).match(other.c3);
}

std::string EqualsMat4::describe() const {
    std::ostringstream ss;
    ss << "Equals: " << mat << " (Epsilon = " << epsilon << ")";
    return ss.str();
}

bool EqualsMat4::match(const glm::mat4 &other) const {
    return WithinAbs(mat[0], epsilon).match(other[0][0]) &&
           WithinAbs(mat[1], epsilon).match(other[0][1]) &&
           WithinAbs(mat[2], epsilon).match(other[0][2]) &&
           WithinAbs(mat[3], epsilon).match(other[0][3]) &&
           WithinAbs(mat[4], epsilon).match(other[1][0]) &&
           WithinAbs(mat[5], epsilon).match(other[1][1]) &&
           WithinAbs(mat[6], epsilon).match(other[1][2]) &&
           WithinAbs(mat[7], epsilon).match(other[1][3]) &&
           WithinAbs(mat[8], epsilon).match(other[2][0]) &&
           WithinAbs(mat[9], epsilon).match(other[2][1]) &&
           WithinAbs(mat[10], epsilon).match(other[2][2]) &&
           WithinAbs(mat[11], epsilon).match(other[2][3]) &&
           WithinAbs(mat[12], epsilon).match(other[3][0]) &&
           WithinAbs(mat[13], epsilon).match(other[3][1]) &&
           WithinAbs(mat[14], epsilon).match(other[3][2]) &&
           WithinAbs(mat[15], epsilon).match(other[3][3]);
}

EqualsQuat::EqualsQuat(const Quat &quat, float epsilon)
    : quat{quat}, epsilon{epsilon} {}

bool EqualsQuat::match(const Quat &other) const {
    return WithinAbs(quat.x, epsilon).match(other.x) &&
           WithinAbs(quat.y, epsilon).match(other.y) &&
           WithinAbs(quat.z, epsilon).match(other.z) &&
           WithinAbs(quat.w, epsilon).match(other.w);
}

std::string EqualsQuat::describe() const {
    std::ostringstream ss;
    ss << "Equals: " << quat << " (Epsilon = " << epsilon << ")";
    return ss.str();
}
