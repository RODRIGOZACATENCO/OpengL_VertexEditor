cmake_minimum_required(VERSION 3.16)
project(VertexEditor)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.89.8
)
FetchContent_MakeAvailable(imgui)

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
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
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