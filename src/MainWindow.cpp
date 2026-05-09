//
// Created by rodrigo on 30/04/2026.
//

#include <glad/glad.h>

#include "../include/MainWindow.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/glm.hpp>
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
      renderer->mainRenderPass();
      renderer->selectionBufferPass();
      gui.showMainWindowGUI();

      // glfwPollEvents(); <-- REMOVE FROM HERE
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
    renderer->selectionBufferPass();
    auto result = renderer->meshElementDetection();
    renderer->setRenderMode(gui.getCurrentState());

    if (result) {
      auto [clicked_ID, mesh_id, empty] = *result;
      switch (gui.getCurrentState()) {
      case FACE_EDITING:
        std::cout << "Clicked on face ID: " << clicked_ID
                  << " of mesh ID: " << mesh_id << std::endl;
        for (auto i : *renderer->getCurrentScene()->getFaceSelectionArray())
          std::cout << i << " ";
        std::cout << std::endl;
        renderer->getCurrentScene()->updateFacesSelected(clicked_ID, mesh_id);
        break;

      case VERTEX_EDITING:
        std::cout << "Clicked on vertex ID: " << clicked_ID
                  << " of mesh ID: " << mesh_id << std::endl;
        for (auto i : *renderer->getCurrentScene()->getVertexSelectionArray())
          std::cout << i << " ";
        renderer->getCurrentScene()->updateVerticesSelected(clicked_ID,
                                                            mesh_id);
        std::cout << std::endl;
        break;
      case EDGE_EDITING:
        std::cout << "Clicked on edge ID: " << clicked_ID
                  << " of mesh ID: " << mesh_id << std::endl;

        for (auto i : *renderer->getCurrentScene()->getEdgeSelectionArray())
          std::cout << i << " ";
        std::cout << std::endl;
        renderer->getCurrentScene()->updateEdgesSelected(clicked_ID, mesh_id);

        break;
      }
    } else {
      std::cout << "Clicked on empty space" << std::endl;
    }
  }
}

void MainWindow::processInput() {

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
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