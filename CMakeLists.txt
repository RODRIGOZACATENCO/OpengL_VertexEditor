cmake_minimum_required(VERSION 3.16)
project(VertexEditor)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

# ImGui is now located in ${SHARED_INCLUDE_DIR}/imgui

# Set your shared includes
set(SHARED_INCLUDE_DIR "OpenGL_include")
set(1 "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME}
        main.cpp
        ${SOURCES}
        "${SHARED_INCLUDE_DIR}/glad/src/glad.c"
        src/CameraHandler.cpp
        include/Mesh.h
        include/MainWindow.h

        OpenGL_include/ShaderHandler.h
        OpenGL_include/TextureHandler.h
        src/Mesh.cpp
        src/MainWindow.cpp
        src/Scene.cpp
        include/Scene.h
        src/Renderer.cpp
        include/Renderer.h

)
target_include_directories(${PROJECT_NAME} PRIVATE
        include
        ${SHARED_INCLUDE_DIR}
        "${SHARED_INCLUDE_DIR}/glad/include"
        ${HEADER_DIR}
)


# Find ALL your global libraries|
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)

# Link them
target_sources(${PROJECT_NAME} PRIVATE
        ${SHARED_INCLUDE_DIR}/imgui/imgui_demo.cpp
        ${SHARED_INCLUDE_DIR}/imgui/imgui.cpp
        ${SHARED_INCLUDE_DIR}/imgui/imgui_draw.cpp
        ${SHARED_INCLUDE_DIR}/imgui/imgui_tables.cpp
        ${SHARED_INCLUDE_DIR}/imgui/imgui_widgets.cpp
        ${SHARED_INCLUDE_DIR}/imgui/backends/imgui_impl_glfw.cpp
        ${SHARED_INCLUDE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${SHARED_INCLUDE_DIR}/imgui
        ${SHARED_INCLUDE_DIR}/imgui/backends
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        assimp::assimp
        glm::glm
        ${OPENGL_LIBRARIES}
        ${CMAKE_DL_LIBS}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
        HEADER_DIR="../include"
        IMGUI_IMPL_OPENGL_LOADER_GLAD
)