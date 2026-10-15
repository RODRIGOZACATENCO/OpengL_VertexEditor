cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0135 NEW)
project(VertexEditor)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set your shared includes
set(SHARED_INCLUDE_DIR "OpenGL_include")

# Find ALL your global libraries
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)

# Collect source files instantly (No CONFIGURE_DEPENDS lag)
aux_source_directory(src SOURCES)

# --- 1. Vendor Library (Builds Once) ---
add_library(Vendor STATIC
        "${SHARED_INCLUDE_DIR}/glad/src/glad.c"
        ${SHARED_INCLUDE_DIR}/imgui/imgui.cpp
        ${SHARED_INCLUDE_DIR}/imgui/imgui_draw.cpp
        ${SHARED_INCLUDE_DIR}/imgui/imgui_tables.cpp
        ${SHARED_INCLUDE_DIR}/imgui/imgui_widgets.cpp
        ${SHARED_INCLUDE_DIR}/imgui/backends/imgui_impl_glfw.cpp
        ${SHARED_INCLUDE_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)



target_include_directories(Vendor PUBLIC
        ${SHARED_INCLUDE_DIR}
        "${SHARED_INCLUDE_DIR}/glad/include"
        ${SHARED_INCLUDE_DIR}/imgui
        ${SHARED_INCLUDE_DIR}/imgui/backends
)

target_compile_definitions(Vendor PUBLIC
        IMGUI_IMPL_OPENGL_LOADER_GLAD
)

# Skip debug info for third-party code (imgui/glad) -> smaller objects, faster linking.
# Remove this if you ever need to step into imgui internals.
if (NOT MSVC)
    target_compile_options(Vendor PRIVATE $<$<CONFIG:Debug>:-g0>)
endif ()

target_link_libraries(Vendor PUBLIC
        glfw
        ${OPENGL_LIBRARIES}
)

# --- 2. Core Library (Rebuilds Fast) ---
# FIX: Removed the redundant second add_library declaration that was causing errors.
# OBJECT library: object files are linked directly into the exe, skipping the
# slow "ar" step that rebuilt an 18 MB .a on every single change.
add_library(VertexEditorCore OBJECT
        ${SOURCES}
        OpenGL_include/JsonParser.h
        OpenGL_include/ObjHandler.h
        headers/KeyboardHandler.h
)

target_include_directories(VertexEditorCore PUBLIC
        headers
)

target_link_libraries(VertexEditorCore PUBLIC
        Vendor
        assimp::assimp
        glm::glm
        ${CMAKE_DL_LIBS}
)

target_compile_definitions(VertexEditorCore PUBLIC
        ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}/"
        HEADER_DIR="../include"
)

# OPTIMIZATION 2: Heavy Precompiled Headers
# Included the standard types and Assimp headers you are using based on your previous questions.
target_precompile_headers(VertexEditorCore PRIVATE
        <vector>
        <string>
        <memory>
        <variant>
        <unordered_map>
        <iostream>
        <glm/glm.hpp>
        <assimp/Importer.hpp>
        <assimp/scene.h>
        <assimp/postprocess.h>
)

# --- Main Executable ---
add_executable(${PROJECT_NAME} main.cpp)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"

        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/settings"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/settings")

target_link_libraries(${PROJECT_NAME} PRIVATE VertexEditorCore)

# Reuse Core's precompiled header so main.cpp doesn't re-parse assimp/glm/std headers.
target_precompile_headers(${PROJECT_NAME} REUSE_FROM VertexEditorCore)

