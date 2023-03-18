add_executable(Visualization Visualization.cpp Movable.h Shader.cpp Shader.h VertexBuffer.h)

target_compile_definitions(Visualization PUBLIC GLFW_INCLUDE_NONE)

target_link_libraries(Visualization PUBLIC glm glfw glad SimdMath)
