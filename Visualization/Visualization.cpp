//
// Created by andyroiiid on 3/18/2023.
//

#include <GLFW/glfw3.h>
#include <SimdMat.h>
#include <cstdio>
#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Shader.h"
#include "VertexBuffer.h"

const char *VERTEX_SHADER_SOURCE = R"GLSL(
#version 450 core
layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec4 aNormal;

layout (location = 0) out vec4 vNormal;

layout (location = 0) uniform mat4 uModel;
layout (location = 1) uniform mat4 uView;
layout (location = 2) uniform mat4 uProjection;

void main() {
    gl_Position = uProjection * uView * uModel * aPosition;
    vNormal = aNormal;
}
)GLSL";

const char *FRAGMENT_SHADER_SOURCE = R"GLSL(
#version 450 core
layout (location = 0) in vec4 vNormal;

layout (location = 0) out vec4 fColor;

void main() {
    fColor = vNormal;
}
)GLSL";

struct Vertex {
    SimdVec position;
    SimdVec normal;
    static void SetupVertexArray(GLuint vao) {
        SetupVertexArrayFloatsAttrib(vao, 0, 0, 3, offsetof(Vertex, position));
        SetupVertexArrayFloatsAttrib(vao, 1, 0, 3, offsetof(Vertex, normal));
    }
};

using Vertices = VertexBuffer<Vertex>;

class App {
public:
    NO_MOVE_OR_COPY(App)

    App() {
        std::vector<Vertex> vertices{{{-0.5f, -0.5f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 0.0f}},
                                     {{0.5f, -0.5f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 0.0f}},
                                     {{0.0f, 0.5f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 0.0f}}};
        m_vertices = Vertices(vertices.size(), vertices.data());
    }

    ~App() = default;

    void MainLoop(GLFWwindow *window) {
        double prevTime = glfwGetTime();
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            double currTime = glfwGetTime();
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);
            Frame(static_cast<float>(currTime - prevTime), width, height);
            prevTime = currTime;
            glfwSwapBuffers(window);
        }
    }

private:
    void Frame(float DeltaTime, int width, int height) {
        m_model = SimdMat::RotateY(DeltaTime) * m_model;

        glm::mat4 lookAt = glm::lookAt(glm::vec3{1.0f, 2.0f, 3.0f},
                                       glm::vec3{0.0f, 0.0f, 0.0f},
                                       glm::vec3{0.0f, 1.0f, 0.0f});

        glm::mat4 perspective = glm::perspective(glm::radians(60.0f),
                                                 static_cast<float>(width) / static_cast<float>(height),
                                                 0.1f,
                                                 100.0f);

        glViewport(0, 0, width, height);
        glClearColor(0.4f, 0.8f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader.Use();

        m_shader.SetUniform(m_modelLocation, m_model);
        m_shader.SetUniform(m_viewLocation, lookAt);
        m_shader.SetUniform(m_projectionLocation, perspective);

        m_vertices.BindAndDraw(GL_TRIANGLES);
    }

    Vertices m_vertices;
    Shader m_shader{VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE};
    GLint m_modelLocation = m_shader.GetUniformLocation("uModel");
    GLint m_viewLocation = m_shader.GetUniformLocation("uView");
    GLint m_projectionLocation = m_shader.GetUniformLocation("uProjection");

    SimdMat m_model;
};

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(800, 600, "SimdMath", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    gladLoadGL(glfwGetProcAddress);

#define PRINT_GL_STRING(x) printf(#x " = %s\n", glGetString(x))
    PRINT_GL_STRING(GL_VERSION);
    PRINT_GL_STRING(GL_SHADING_LANGUAGE_VERSION);
    PRINT_GL_STRING(GL_VENDOR);
    PRINT_GL_STRING(GL_RENDERER);
#undef PRINT_GL_STRING

    {
        App app;
        app.MainLoop(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}
