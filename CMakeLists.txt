cmake_minimum_required(VERSION 3.16)
project(VertexEditor)

set(CMAKE_CXX_STANDARD 20)

#RUN THIS FIRST TO INSTALL IMGUI
#git clone --depth 1 --branch v1.89.8 https://github.com/ocornut/imgui.git OpenGL_include/imgui

# ImGui is now located in ${SHARED_INCLUDE_DIR}/imgui

# Set your shared includes
set(SHARED_INCLUDE_DIR "OpenGL_include")

# Find ALL your global libraries
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)

# Gather all source files (excluding main.cpp)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# --- Core Library ---
# This library contains all application logic, making it easy to link with tests.
add_library(VertexEditorCore STATIC
        ${SOURCES}
        "${SHARED_INCLUDE_DIR}/glad/src/glad.c"
        ${SHARED_INCLUDE_DIR}/imgui/imgui_demo.cpp
        ${SHARED_INCLUDE_DIR}/imgui/imgui.cpp
        ${SHARED_INCLUDE_DIR}/imgui/imgui_draw.cpp
        ${SHARED_INCLUDE_DIR}/imgui/imgui_tables.cpp
        ${SHARED_INCLUDE_DIR}/imgui/imgui_widgets.cpp
        ${SHARED_INCLUDE_DIR}/imgui/backends/imgui_impl_glfw.cpp
        ${SHARED_INCLUDE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(VertexEditorCore PUBLIC
        include
        ${SHARED_INCLUDE_DIR}
        "${SHARED_INCLUDE_DIR}/glad/include"
        ${SHARED_INCLUDE_DIR}/imgui
        ${SHARED_INCLUDE_DIR}/imgui/backends
)

target_link_libraries(VertexEditorCore PUBLIC
        glfw
        assimp::assimp
        glm::glm
        ${OPENGL_LIBRARIES}
        ${CMAKE_DL_LIBS}
)

target_compile_definitions(VertexEditorCore PUBLIC
        ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
        HEADER_DIR="../include"
        IMGUI_IMPL_OPENGL_LOADER_GLAD
)

# --- Main Executable ---
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE VertexEditorCore)

# --- Testing ---
enable_testing()
add_subdirectory(editor_tests)