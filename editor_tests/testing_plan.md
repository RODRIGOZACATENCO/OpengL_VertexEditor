# VertexEditor Testing Implementation Plan

Based on the analysis of the project structure, here is a comprehensive plan to integrate automated tests that run during the build process.

first, analize and optimize the cmakelist file so repeated calls like src/Mesh.cpp, src/Scene.cpp and their includes are handled more efficently
do not change anything from the original source code, the unit test are independent from the source material
## 1. Project Structure Analysis
Currently, all source files (`src/*.cpp`) and `main.cpp` are compiled directly into a single executable `VertexEditor` in the root `CMakeLists.txt`. 

**Key Components to Test:**
*   **`Mesh.cpp` / `Mesh.h`**: Data loading (Assimp), vertex generation, half-edge normal calculations (which have had bugs previously), and bounding boxes. These are largely mathematical and structural, making them perfect for unit testing.
*   **`Scene.cpp` / `Scene.h`**: Scene graph management, adding/removing meshes, and handling vertex/edge selection states.
*   **`CameraHandler.cpp` / `CameraHandler.h`**: View and projection matrix math, camera movement math.
*   **`Renderer.cpp`, `MainWindow.cpp`, `GUI.cpp`**: These rely heavily on an active OpenGL/GLFW context. They will require "integration tests" where a hidden headless window is spawned, or they will need to be mocked.

## 2. CMake Build System Refactoring
To allow the test executables to use your existing code without running `main.cpp`, we need to split your CMake target.

1.  **Create a Core Library**: Modify the root `CMakeLists.txt` to compile your source code (excluding `main.cpp`) into a static library, e.g., `VertexEditorCore`.
2.  **Update Main Executable**: Link the `VertexEditor` executable to `VertexEditorCore`.
3.  **Enable Testing**: Add `enable_testing()` to the root `CMakeLists.txt`.
4.  **Include Tests**: Add `add_subdirectory(editor_tests)` to the root `CMakeLists.txt`.

## 3. Testing Framework
We will use **GoogleTest (gtest)**. It is an industry standard and can be easily downloaded automatically by CMake at build time, so you don't need to manually install dependencies.

## 4. `editor_tests` Setup
Inside the `editor_tests/` directory, we will create:

*   **`CMakeLists.txt`**: 
    *   Uses CMake's `FetchContent` to download GoogleTest.
    *   Defines a test executable (e.g., `run_editor_tests`).
    *   Links the test executable to `gtest_main`, `VertexEditorCore`, and OpenGL/Assimp dependencies.
    *   Registers the tests with CTest using `gtest_discover_tests()`.
*   **`test_mesh.cpp`**: Tests for the `Mesh` class.
    *   *Example:* Load a simple primitive (like a cube or a triangle) and assert that vertex count, index count, and computed normals match expected values.
*   **`test_scene.cpp`**: Tests for the `Scene` class.
    *   *Example:* Add a mesh to the scene, assert mesh count is 1. Select a vertex, assert selection state is accurately tracked.
*   **`test_camera.cpp`**: Tests for `CameraHandler`.
    *   *Example:* Move camera by a specific offset, assert the resulting View matrix matches mathematical expectations.

## 5. Execution
Once implemented, you can build and run tests using standard CMake commands:

```bash
# Build the project (which also builds the tests)
cmake --build build

# Run the tests
cd build && ctest --output-on-failure
```

Because `ctest` integrates with CMake, CLion will automatically detect these tests and give you green/red play buttons next to your test cases in the IDE!

---
**Next Steps:**
If this plan looks good to you, I can start by modifying the `CMakeLists.txt` to extract `VertexEditorCore` and then I'll write the initial setup and our first test case in `editor_tests/`.
