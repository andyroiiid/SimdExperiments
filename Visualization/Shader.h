#pragma once

#include <SimdMat.h>
#include <glad/gl.h>
#include <glm/mat4x4.hpp>

#include "Movable.h"

class Shader {
public:
    MOVABLE(Shader)

    Shader() = default;

    Shader(const char *vertexSource, const char *fragmentSource);

    ~Shader();

    void Use();

    [[nodiscard]] GLint GetUniformLocation(const char *name) const;

    void SetUniform(GLint location, const glm::mat4 &m);

    void SetUniform(GLint location, const SimdMat &m);

private:
    Movable<GLuint> m_program;
};