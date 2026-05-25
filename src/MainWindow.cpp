//
// Created by rodrigo on 30/04/2026.
//

#include <glad/glad.h>

#include "../include/ElementEditingRenderer.h"
#include "../include/MainWindow.h"
#include "CameraHandler.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <iostream>
#include <ostream>
#include <random>
glm::vec3 generateRandomColor() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 255);
  return {dis(gen) / 255.0f, dis(gen) / 255.0f, dis(gen) / 255.0f};
}

// default scene has 3 objects, 2 pyramind and 1 cube

// given the scene to render, the window will render all objects
// mainWindow always initializes with a default window named "default"
void MainWindow::use(std::string *scene_name) {
  std::string default_scene_name = "default";
  if (scene_name == nullptr)
    scene_name = &default_scene_name;
  // the renderer will accept the scene and draw it in the screen
  renderer->setCurrentScene(scene_name_to_scene_object[*scene_name].get());

  std::string error;
  if (isWindowReady(&error)) {
    while (!glfwWindowShouldClose(window)) {
      float time = glfwGetTime();
      delta_time = time - last_frame;
      last_frame = time;

      renderer->setDeltaTime(delta_time);
      camera.setDeltaTime(delta_time);
      renderer->setRenderMode(gui.getCurrentState());
      // 1. POLL EVENTS FIRST
      glfwPollEvents();
      processInput();
      if (gui.getResetFlag()) {
        renderer->getCurrentScene()->resetSelectionBuffer(
            gui.getCurrentState());
        gui.setResetFlag(0);
      }

      // 2. THEN START NEW IMGUI FRAME
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      // 3. RENDER SCENE AND UI
      renderer->getCurrentScene()->setViewMatrix(camera.getCurrentViewMatrix());
      renderer->processDrawCall(main_render_pass);
      gui.showMainWindowGUI();

      glfwSwapBuffers(window);
    }
  } else {
    std::cout << error << std::endl;
  }
}

void MainWindow::cleanup() {
  if (renderer)
    renderer->cleanup();
  for (auto &[name, scene] : scene_name_to_scene_object) {
    if (scene)
      scene->cleanup();
  }
}

void MainWindow::framebufferSizeCallback(GLFWwindow *window, int width,
                                         int height) {
  glViewport(0, 0, width, height);
  MainWindow *instance =
      static_cast<MainWindow *>(glfwGetWindowUserPointer(window));

  // 4. Forward the call to the non-static member
  if (instance) {
    instance->onFramebufferSize();
  }
}

void MainWindow::onFramebufferSize() {
  renderer->setScreenSize(width, height);
  renderer->resizeFramebuffer();
}

void MainWindow::mainWindowMouseCallback(GLFWwindow *window, int button,
                                         int action, int mods) {
  // 3. Retrieve the pointer we stored earlier
  MainWindow *instance =
      static_cast<MainWindow *>(glfwGetWindowUserPointer(window));

  // 4. Forward the call to the non-static member
  if (instance) {
    instance->onMouseButton(button, action, mods);
  }
}

void MainWindow::onMouseButton(int button, int action, int mods) {
  if (ImGui::GetIO().WantCaptureMouse)
    return;

  // switches the current drawing state of the renderer
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    renderer->processDrawCall(element_detection_pass);
    auto result = renderer->meshElementDetection();
    if (!(mods & GLFW_MOD_SHIFT)) { // shift+left click, reset if the user is
                                    // not using shift
      renderer->getCurrentScene()->resetSelectionBuffer(gui.getCurrentState());
    }
    if (result) {
      auto [clicked_ID, mesh_id, empty] = *result;
      switch (gui.getCurrentState()) {
      case FACE_EDITING:
        renderer->getCurrentScene()->updateFacesSelected(clicked_ID, mesh_id);
        break;

      case VERTEX_EDITING:
        renderer->getCurrentScene()->updateVerticesSelected(clicked_ID,
                                                            mesh_id);
        break;
      case EDGE_EDITING:
        renderer->getCurrentScene()->updateEdgesSelected(clicked_ID, mesh_id);
        break;
      }
    }
  }
}

void MainWindow::mainWindowKeyCallback(GLFWwindow *window, int key,
                                       int scancode, int action, int mods) {
  // 3. Retrieve the pointer we stored earlier
  MainWindow *instance =
      static_cast<MainWindow *>(glfwGetWindowUserPointer(window));

  // 4. Forward the call to the non-static member
  if (instance) {
    instance->onKeyboardInput(window, key, scancode, action, mods);
  }
}

void MainWindow::onKeyboardInput(GLFWwindow *window, int key, int scancode,
                                 int action, int mods) {}
void MainWindow::processInput() {
  keys[UP] = (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
  keys[DOWN] = (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS);
  keys[LEFT] = (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
  keys[RIGHT] = (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
  camera.gimballCameraUpdate(keys);

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void MainWindow::mainWindowScrollCallback(GLFWwindow *window, double xoffset,
                                          double yoffset) {

  MainWindow *instance =
      static_cast<MainWindow *>(glfwGetWindowUserPointer(window));

  if (instance) {
    instance->onScrollCallback(window, xoffset, yoffset);
  }
}
void MainWindow::onScrollCallback(GLFWwindow *window, double xoffset,
                                  double yoffset) {
  camera.processZoom(-yoffset);
}

bool MainWindow::isWindowReady(std::string *out_error) const {
  auto fail = [&](const std::string &msg) {
    if (out_error)
      *out_error = msg;
    return false;
  };

  if (!window)
    return fail("No GLFW window set.");
  if (!renderer)
    return fail("No renderer set.");
  if (width <= 0 || height <= 0)
    return fail("Invalid window dimensions.");
  if (scene_name_to_scene_object.empty())
    return fail("No scenes added.");

  bool has_active_scene = false;
  for (auto &[name, scene] : scene_name_to_scene_object) {
    if (scene != nullptr) {
      has_active_scene = true;
      break;
    }
  }
  if (!has_active_scene)
    return fail("All scenes are null.");

  return true;
}