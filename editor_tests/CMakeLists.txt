include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(run_editor_tests
  test_mesh.cpp
  test_camera.cpp
  test_scene.cpp
)

target_link_libraries(run_editor_tests
  PRIVATE
  VertexEditorCore
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(run_editor_tests)

# Run tests automatically after building the test executable
add_custom_command(
    TARGET run_editor_tests
    POST_BUILD
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running tests automatically after build..."
)
