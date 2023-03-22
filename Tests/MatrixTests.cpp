//
// Created by andyroiiid on 3/17/2023.
//

#include <catch2/catch_test_macros.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "TestUtils.h"

TEST_CASE("Construction") {
    const Mat4 identity;

    CHECK_THAT(identity, EqualsSimdMat({1.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f,
                                        0.0f, 0.0f, 0.0f, 1.0f}));

    CHECK_THAT(identity.c0, EqualsSimdVec({1.0f, 0.0f, 0.0f, 0.0f}));
    CHECK_THAT(identity.c1, EqualsSimdVec({0.0f, 1.0f, 0.0f, 0.0f}));
    CHECK_THAT(identity.c2, EqualsSimdVec({0.0f, 0.0f, 1.0f, 0.0f}));
    CHECK_THAT(identity.c3, EqualsSimdVec({0.0f, 0.0f, 0.0f, 1.0f}));

    const Vec4 iBasis{1.0f, 2.0f, 3.0f, 4.0f};
    const Vec4 jBasis{5.0f, 6.0f, 7.0f, 8.0f};
    const Vec4 kBasis{9.0f, 10.0f, 11.0f, 12.0f};
    const Vec4 translation{13.0f, 14.0f, 15.0f, 16.0f};

    const Mat4 mat{iBasis, jBasis, kBasis, translation};
    CHECK_THAT(mat, EqualsSimdMat({1.0f, 2.0f, 3.0f, 4.0f,
                                   5.0f, 6.0f, 7.0f, 8.0f,
                                   9.0f, 10.0f, 11.0f, 12.0f,
                                   13.0f, 14.0f, 15.0f, 16.0f}));
}

TEST_CASE("Operators") {
    const Mat4 identity;
    const Vec4 v{1.0f, 2.0f, 3.0f, 4.0f};
    const Mat4 m{1.0f, 2.0f, 3.0f, 4.0f,
                 5.0f, 6.0f, 7.0f, 8.0f,
                 9.0f, 10.0f, 11.0f, 12.0f,
                 13.0f, 14.0f, 15.0f, 16.0f};

    CHECK_THAT(m.Transpose(), EqualsSimdMat({1.0f, 5.0f, 9.0f, 13.0f,
                                             2.0f, 6.0f, 10.0f, 14.0f,
                                             3.0f, 7.0f, 11.0f, 15.0f,
                                             4.0f, 8.0f, 12.0f, 16.0f}));

    CHECK_THAT(identity * v, EqualsSimdVec(v));
    CHECK_THAT(v * identity, EqualsSimdVec(v));

    CHECK_THAT(m * v, EqualsSimdVec({90.0f, 100.0f, 110.0f, 120.0f}));
    CHECK_THAT(v * m, EqualsSimdVec({30.0f, 70.0f, 110.0f, 150.0f}));

    CHECK_THAT(m * m, EqualsSimdMat({90.0f, 100.0f, 110.0f, 120.0f,
                                     202.0f, 228.0f, 254.0f, 280.0f,
                                     314.0f, 356.0f, 398.0f, 440.0f,
                                     426.0f, 484.0f, 542.0f, 600.0f}));

    glm::vec4 v2{1.0f, 2.0f, 3.0f, 4.0f};
    glm::mat4 m2{1.0f, 2.0f, 3.0f, 4.0f,
                 5.0f, 6.0f, 7.0f, 8.0f,
                 9.0f, 10.0f, 11.0f, 12.0f,
                 13.0f, 14.0f, 15.0f, 16.0f};

    CHECK_THAT(m2 * v2, EqualsSimdVec({90.0f, 100.0f, 110.0f, 120.0f}));
    CHECK_THAT(v2 * m2, EqualsSimdVec({30.0f, 70.0f, 110.0f, 150.0f}));
    CHECK_THAT(m2 * m2, EqualsSimdMat({90.0f, 100.0f, 110.0f, 120.0f,
                                       202.0f, 228.0f, 254.0f, 280.0f,
                                       314.0f, 356.0f, 398.0f, 440.0f,
                                       426.0f, 484.0f, 542.0f, 600.0f}));
}

TEST_CASE("Static Creates") {
    {
        const Vec4 v{1.0f, 2.0f, 3.0f, 1.0f};
        const Mat4 m = Mat4::Translate({4.0f, 5.0f, 6.0f, 1.0f});
        CHECK_THAT(m * v, EqualsSimdVec({5.0f, 7.0f, 9.0f, 1.0f}));
    }

    {
        const Vec4 v{1.0f, 2.0f, 3.0f, 1.0f};
        const Mat4 m = Mat4::Scale(4.0f);
        CHECK_THAT(m * v, EqualsSimdVec({4.0f, 8.0f, 12.0f, 1.0f}));
    }

    {
        const Vec4 v{1.0f, 2.0f, 3.0f, 1.0f};
        const Mat4 m = Mat4::Scale({4.0f, 5.0f, 6.0f, 1.0f});
        CHECK_THAT(m * v, EqualsSimdVec({4.0f, 10.0f, 18.0f, 1.0f}));
    }

    {
        const Vec4 v{1.0f, 2.0f, 3.0f, 1.0f};
        const Mat4 m = Mat4::RotateX(M_PI_2);
        CHECK_THAT(m * v, EqualsSimdVec({1.0f, -3.0f, 2.0f, 1.0f}));
    }

    {
        const Vec4 v{1.0f, 2.0f, 3.0f, 1.0f};
        const Mat4 m = Mat4::RotateY(M_PI_2);
        CHECK_THAT(m * v, EqualsSimdVec({3.0f, 2.0f, -1.0f, 1.0f}));
    }

    {
        const Vec4 v{1.0f, 2.0f, 3.0f, 1.0f};
        const Mat4 m = Mat4::RotateZ(M_PI_2);
        CHECK_THAT(m * v, EqualsSimdVec({-2.0f, 1.0f, 3.0f, 1.0f}));
    }

    {
        const glm::mat4 m1 = glm::lookAt(glm::vec3{1.0f, 2.0f, 3.0f},
                                         glm::vec3{0.0f, 0.0f, 0.0f},
                                         glm::vec3{0.0f, 1.0f, 0.0f});
        const Mat4 m2 = Mat4::LookAt({1.0f, 2.0f, 3.0f, 1.0f},
                                     {0.0f, 0.0f, 0.0f, 1.0f},
                                     {0.0f, 1.0f, 0.0f, 0.0f});
        CHECK_THAT(m1, EqualsSimdMat(m2));
    }

    {
        const glm::mat4 m1 = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);
        const Mat4 m2 = Mat4::Perspective(M_PI / 3.0f, 1.0f, 0.1f, 100.0f);
        CHECK_THAT(m1, EqualsSimdMat(m2));
    }
}
