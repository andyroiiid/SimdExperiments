//
// Created by andyroiiid on 3/18/2023.
//

#include <GLFW/glfw3.h>
#include <Quat.h>
#include <cstdio>
#include <glad/gl.h>
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
    vNormal = uModel * aNormal;
}
)GLSL";

const char *FRAGMENT_SHADER_SOURCE = R"GLSL(
#version 450 core
layout (location = 0) in vec4 vNormal;

layout (location = 0) out vec4 fColor;

void main() {
    fColor = vNormal * 0.5 + 0.5;
}
)GLSL";

struct Vertex {
    Vec4 position;
    Vec4 normal;
    static void SetupVertexArray(GLuint vao) {
        SetupVertexArrayFloatsAttrib(vao, 0, 0, 3, offsetof(Vertex, position));
        SetupVertexArrayFloatsAttrib(vao, 1, 0, 3, offsetof(Vertex, normal));
    }
};

using Vertices = VertexBuffer<Vertex>;

std::vector<Vertex> CreateBox(const Vec4 &min, const Vec4 &max) {
    const Vec4 p000{_mm_blend_ps(min.m, max.m, 0b0000)};
    const Vec4 p001{_mm_blend_ps(min.m, max.m, 0b0100)};
    const Vec4 p010{_mm_blend_ps(min.m, max.m, 0b0010)};
    const Vec4 p011{_mm_blend_ps(min.m, max.m, 0b0110)};
    const Vec4 p100{_mm_blend_ps(min.m, max.m, 0b0001)};
    const Vec4 p101{_mm_blend_ps(min.m, max.m, 0b0101)};
    const Vec4 p110{_mm_blend_ps(min.m, max.m, 0b0011)};
    const Vec4 p111{_mm_blend_ps(min.m, max.m, 0b0111)};

    const Vec4 npx{1, 0, 0, 0};
    const Vec4 nnx{-1, 0, 0, 0};
    const Vec4 npy{0, 1, 0, 0};
    const Vec4 nny{0, -1, 0, 0};
    const Vec4 npz{0, 0, 1, 0};
    const Vec4 nnz{0, 0, -1, 0};

    std::vector<Vertex> vertices{
            // +x
            {p101, npx},
            {p100, npx},
            {p111, npx},
            {p111, npx},
            {p100, npx},
            {p110, npx},
            // -x
            {p000, nnx},
            {p001, nnx},
            {p010, nnx},
            {p010, nnx},
            {p001, nnx},
            {p011, nnx},
            // posY
            {p011, npy},
            {p111, npy},
            {p010, npy},
            {p010, npy},
            {p111, npy},
            {p110, npy},
            // -y
            {p000, nny},
            {p100, nny},
            {p001, nny},
            {p001, nny},
            {p100, nny},
            {p101, nny},
            // +z
            {p001, npz},
            {p101, npz},
            {p011, npz},
            {p011, npz},
            {p101, npz},
            {p111, npz},
            // -z
            {p100, nnz},
            {p000, nnz},
            {p110, nnz},
            {p110, nnz},
            {p000, nnz},
            {p010, nnz}};

    return vertices;
}

class App {
public:
    NO_MOVE_OR_COPY(App)

    App() {
        std::vector<Vertex> vertices = CreateBox({-1.0f, -1.0f, -1.0f, 1.0f},
                                                 {1.0f, 1.0f, 1.0f, 1.0f});
        m_vertices = Vertices(vertices.size(), vertices.data());

        glEnable(GL_DEPTH_TEST);
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
        m_rotation = Quat{{1.0f, 1.0f, 1.0f, 0.0f}, DeltaTime} * m_rotation;

        const Mat4 lookAt = Mat4::LookAt({1.0f, 2.0f, 3.0f, 1.0f},
                                         {0.0f, 0.0f, 0.0f, 1.0f},
                                         {0.0f, 1.0f, 0.0f, 0.0f});

        const Mat4 perspective = Mat4::Perspective(M_PI / 3.0f,
                                                   static_cast<float>(width) / static_cast<float>(height),
                                                   0.1f,
                                                   100.0f);

        glViewport(0, 0, width, height);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader.Use();

        m_shader.SetUniform(m_modelLocation, m_rotation.ToMat4());
        m_shader.SetUniform(m_viewLocation, lookAt);
        m_shader.SetUniform(m_projectionLocation, perspective);

        m_vertices.BindAndDraw(GL_TRIANGLES);
    }

    Vertices m_vertices;
    Shader m_shader{VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE};
    GLint m_modelLocation = m_shader.GetUniformLocation("uModel");
    GLint m_viewLocation = m_shader.GetUniformLocation("uView");
    GLint m_projectionLocation = m_shader.GetUniformLocation("uProjection");

    Quat m_rotation;
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
