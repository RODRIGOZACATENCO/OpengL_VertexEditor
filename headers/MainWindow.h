// Created by rodrigo on 30/04/2026.
//

#pragma once

#define GLFW_INCLUDE_NONE

#include "CameraHandler.h"
#include <GLFW/glfw3.h>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "ElementEditing.h"
#include "ElementEditingRenderer.h"
#include "GUI.h"
#include "JsonParser.h"
#include "Mesh.h"
#include "ObjHandler.h"
#include "Scene.h"

#include <vector>
#include <glm/glm.hpp>
#include "KeyboardHandler.h"
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
  CameraHandler camera;
  JsonParser json_parser;
  ObjHandler obj_parser;
  int width, height; // window dimensions
  GLFWwindow* window;
  GUI gui;
  std::unique_ptr<KeyboardHandler> keyboard;
  std::unique_ptr<ElementEditingRenderer> renderer;
  std::unique_ptr<ElementEditing> element_editing;
  std::map<std::string, std::unique_ptr<Scene>> scene_name_to_object;
  std::string current_scene = "default";
  glm::mat4 projection;

  bool has_scene_changed = false;
  float delta_time = 0.0f;
  float last_frame = 0.0f;

  bool keys[1024] = {false};

public:
  MainWindow(GLFWwindow* window) :gui(window), camera(window), window(window), width(0), height(0) {
    glfwGetFramebufferSize(window, &width, &height);
    keyboard=std::make_unique<KeyboardHandler>();
    camera.setKeyboard(keyboard.get());
    gui.setCamera(&camera);
    projection = glm::perspective(
      glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    gui.setState(FACE_EDITING);
    gui.setCameraMode(GIMBALL);
    camera.setCurrentCameraMode(GIMBALL);
    gui.setKeyboard(keyboard.get());
    load_scene("default");
    renderer = std::make_unique<ElementEditingRenderer>(window, scene_name_to_object["default"].get());

    renderer->setScreenSize(width, height);
    renderer->setRenderMode(FACE_EDITING);
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, mainWindowKeyCallback);
    glfwSetMouseButtonCallback(window, mainWindowMouseCallback);
    glfwSetCursorPosCallback(window,mainWindowMousePosCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetScrollCallback(window, mainWindowScrollCallback);
    element_editing = std::make_unique<ElementEditing>(scene_name_to_object["default"].get());
  }

  void load_scene(const std::string& scene_name="default");
  // Getters and Setters
  GLFWwindow* getWindow() const { return window; }
  GUI& getGui() { return gui; }
  ElementEditingRenderer* getRenderer() { return renderer.get(); }
  int getWidth() const { return width; }
  int getHeight() const { return height; }

  void setWindowSize(int w, int h) {
    width = w;
    height = h;
  }

  float getDeltaTime() { return delta_time; }
  void setDeltaTime(float delta_time) { this->delta_time = delta_time; }
  float getLAstFrame() { return last_frame; }
  void setLastFrame(float last_frame) { this->last_frame = last_frame; }

  // Methods

  static void mainWindowMouseCallback(GLFWwindow* window, int button,
                                      int action, int mods);

  static void framebufferSizeCallback(GLFWwindow* window, int width,
                                      int height);

  static void mainWindowKeyCallback(GLFWwindow* window, int key, int scancode,
                                    int action, int mods);

  static void mainWindowScrollCallback(GLFWwindow* window, double xoffset,
                                       double yoffset);
  static  void mainWindowMousePosCallback(GLFWwindow *window, double xposIn, double yposIn);
  void onMouseMovement(double xposIn, double yposIn);
  void onFramebufferSize();

  void onMouseButton(int button, int action, int mods);

  void onKeyboardInput(GLFWwindow* window, int key, int scancode, int action,
                       int mods);

  void onScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

  void use(std::string* scene_name = nullptr);

  void updateModelMatrices(
    std::vector<std::pair<Mesh*, glm::mat4>>* updated_matrices);

  std::optional<std::tuple<unsigned int, unsigned int, unsigned int>>
  faceDetection();

  std::pair<int, int> getCursorPositionInViewport(GLFWwindow* window);

  void cleanup();

  void addScene(const std::string& name, std::unique_ptr<Scene> scene) { scene_name_to_object[name] = std::move(scene); }

  glm::vec2 getMouseNDC(GLFWwindow* window);

  void process_camera_movement();
};
