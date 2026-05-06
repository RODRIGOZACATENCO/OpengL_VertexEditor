# VertexEditor

A 3D mesh editor built with OpenGL, ImGui, and Assimp.

## Dependencies

The following must be installed on your system before building:

- CMake 3.16+
- A C++20 compiler (GCC, Clang, or MSVC)
- OpenGL
- [GLFW3](https://www.glfw.org/)
- [Assimp](https://github.com/assimp/assimp)
- [GLM](https://github.com/g-truc/glm)

ImGui is either fetched automatically or vendored locally (see below).

## Building

### 1. Clone the repository

```bash
git clone https://github.com/youruser/VertexEditor.git
cd VertexEditor
```

### 2. Configure

**If you already have ImGui** placed under `third_party/imgui`:

```bash
cmake -S . -B build
```

**If you need CMake to download ImGui for you** (requires internet):

```bash
cmake -S . -B build -DFETCH_DEPENDENCIES=ON
```

### 3. Build

```bash
cmake --build build
```

The binary will be placed in the `build/` directory.

## Building in CLion

1. Open the project folder in CLion.
2. Go to **Settings > Build, Execution, Deployment > CMake**.
3. If you need ImGui downloaded, add `-DFETCH_DEPENDENCIES=ON` to the **CMake options** field.
4. Click **Apply**, then reload the CMake project.

## Building in VS Code

Install the following extensions before opening the project:

- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

Then:

1. Open the project folder in VS Code.
2. When prompted, select a kit (your compiler, e.g. GCC or Clang).
3. If you need ImGui downloaded, create or edit `.vscode/settings.json` and add:

```json
{
    "cmake.configureArgs": ["-DFETCH_DEPENDENCIES=ON"]
}
```

4. Open the Command Palette (`Ctrl+Shift+P`) and run **CMake: Configure**.
5. Run **CMake: Build** or press `F7`.

The binary will be placed in the `build/` directory.

## Notes

- `FETCH_DEPENDENCIES` only needs to be set once. After the first build, ImGui is cached in `_deps/` and will not be re-downloaded unless you wipe the build folder.
- If you reset or delete the build directory, make sure the flag is still set in your CMake options before rebuilding.
