// Created by rodrigo on 30/04/2026.
//

#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/detail/qualifier.hpp>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "GUI.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Scene.h"

inline std::vector<float> cube_vertices = {
    -1, -1, -1, // 0 bottom left back
    -1, -1, 1,  // 1 bottom left front
    1,  -1, -1, // 2 bottom right back
    1,  -1, 1,  // 3 bottom right front
    -1, 1,  -1, // 4 top left back
    -1, 1,  1,  // 5 top left front
    1,  1,  -1, // 6 top right back
    1,  1,  1,  // 7 top right front
};

inline std::vector<int> faces = {
    0, 2, 1, 1, 2, 3, // Bottom
    0, 4, 2, 2, 4, 6, // Back
    2, 6, 3, 3, 6, 7, // Right
    1, 3, 5, 3, 7, 5, // Front
    5, 7, 4, 6, 4, 7, // Top
    0, 1, 4, 5, 4, 1, // Left
};
inline std::vector<float> piramid_vertices = {
    -1, -1, -1, // 0 bottom left back
    -1, -1, 1,  // 1 bottom left front
    1,  -1, -1, // 2 bottom right back
    1,  -1, 1,  // 3 bottom right front
    0,  1,  0,  // 4 apex
};

inline std::vector<int> piramid_faces = {
    0, 2, 1, 1, 2, 3, // Bottom
    0, 1, 4,          // Left
    1, 3, 4,          // Front
    3, 2, 4,          // Right
    2, 0, 4,          // Back
};
/*vertex editor main window
 *render pass that shows  the object
 *color picking pass that renders color ID's
 *gimball camera that rotates around the object-MISSING
 *soft simple lighting around the object
 */

/*Main window handles the interface between the gui and the user input
 *activates the correct renders, etc
 */

class MainWindow {
private:
  int width, height; // window dimensions
  GLFWwindow *window;
  GUI gui;
  std::unique_ptr<Renderer> renderer;
  std::map<std::string, std::unique_ptr<Scene>> scene_name_to_scene_object;
  bool has_scene_changed = false;
  float delta_time = 0.0f;
	float last_frame = 0.0f;

public:

  MainWindow(GLFWwindow *window) : window(window), width(0), height(0) {
    glfwGetFramebufferSize(window, &width, &height);
    auto pyramid = std::make_unique<Mesh>(&piramid_vertices, &piramid_faces);
    auto cube = std::make_unique<Mesh>(&cube_vertices, &faces);
    // starts with a default scene
    glm::mat4 view =
        glm::lookAt(glm::vec3(0, 1, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    auto default_scene = std::make_unique<Scene>(
        view, projection); // initialize the default scene

    default_scene->addMesh(std::move(pyramid), "pyramid", glm::mat4(1.0f));
    default_scene->addMesh(std::move(cube), "cube", glm::mat4(1.0f));
    gui.main_state.isFaceSelectionActive =
        true; // sets the initial state of the face selection button to active
    gui.setState(FACE_EDITING);

    renderer = std::make_unique<Renderer>(window, default_scene.get());
    scene_name_to_scene_object["default"] = std::move(default_scene);

    renderer->setCurrentScene(scene_name_to_scene_object["default"].get());
    renderer->setScreenSize(width, height);
    renderer->setRenderMode(FACE_EDITING);

    glfwSetWindowUserPointer(window, this);
    glfwSetMouseButtonCallback(window, mainWindowMouseCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  }

  // Getters and Setters
  GLFWwindow *getWindow() const { return window; }
  GUI &getGui() { return gui; }
  Renderer *getRenderer() { return renderer.get(); }
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  void setWindowSize(int w, int h) {
    width = w;
    height = h;
  }
  float getDeltaTime(){return delta_time;}
  void setDeltaTime(float delta_time){this->delta_time=delta_time;}
  float getLAstFrame(){return last_frame;}
  void setLastFrame(float last_frame){this->last_frame=last_frame;}
  // Methods
  bool isWindowReady(std::string *out_error = nullptr) const;
  static void mainWindowMouseCallback(GLFWwindow *window, int button,
                                      int action, int mods);
  void onMouseButton(int button, int action, int mods);
  static void framebufferSizeCallback(GLFWwindow *window, int width,
                                      int height);
  void onFramebufferSize();
  void use(std::string *scene_name = nullptr);
  void updateModelMatrices(
      std::vector<std::pair<Mesh *, glm::mat4>> *updated_matrices);
  std::optional<std::tuple<unsigned int, unsigned int, unsigned int>>
  faceDetection();
  std::pair<int, int> getCursorPositionInViewport(GLFWwindow *window);
  void cleanup();

  void addScene(std::string name, std::unique_ptr<Scene> scene) {
    scene_name_to_scene_object[name] = std::move(scene);
  }
  void processInput();
};