//
// Created by rodrigo on 04/05/2026.
//

#pragma once
#define GLFW_INCLUDE_NONE
#include "GUI.h"
#include "Scene.h"
#include "ShaderHandler.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <optional>
#include <tuple>
#include <utility>

#include "ElementEditing.h"
inline std::vector<float> quad_vertices = {
  // Positions // TexCoords
  // Triangle 1
  -1.0f, -1.0f,  0.0f, 0.0f, // Bottom Left
   1.0f, -1.0f,  1.0f, 0.0f, // Bottom Right
  -1.0f,  1.0f,  0.0f, 1.0f, // Top Left

  // Triangle 2
   1.0f, -1.0f,  1.0f, 0.0f, // Bottom Right
   1.0f,  1.0f,  1.0f, 1.0f, // Top Right
  -1.0f,  1.0f,  0.0f, 1.0f  // Top Left
};
struct FramebufferInfo {
  unsigned int FBO=0;
  unsigned int texture=0;
  unsigned int depth_texture=0;
};



/*Renderer Class makes the calls to the graphic API
 *it holds all the shaders
 **/
class ElementEditingRenderer {
private:

  Scene *current_scene;            // scene currently being drawn
  GUIState current_rendering_mode; // FACE, VERTEX OR EDGE EDITING
  GLFWwindow *window;
  int width, height;
  std::vector<std::unique_ptr<Shader>> shaders; // holds all the shaders
  float delta_time;

  std::vector<FramebufferInfo> frame_buffers;
  unsigned int main_window_VAO,main_window_VBO;

  glm::vec3 grid_origin={-5,-5,-5};
  std::vector<glm::vec3> axis_lines;
  std::vector<glm::vec3> y_axis_lines;
  std::vector<glm::vec3> z_axis_lines;
  unsigned int axis_lines_VBO,axis_lines_VAO;

public:
  ElementEditingRenderer(GLFWwindow *window, Scene *start_scene)
    : window(window), current_scene(start_scene), width(0), height(0)
  {

    initializeBuffers();

    initializeShaders();

    setupAxisLines();
  }

  // Getters and Setters
  void setScreenSize(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
  }

  void setCurrentScene(Scene *scene) {
    this->current_scene = scene;
    setViewProjectionMatrices();
  }
  Scene *getCurrentScene() { return current_scene; }

  void setRenderMode(GUIState state) { this->current_rendering_mode = state; }
  GUIState getRenderMode() const { return current_rendering_mode; }

  GLFWwindow *getWindow() const { return window; }
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  void setDeltaTime(float delta_time) { this->delta_time = delta_time; }
  // Methods

  //renders background
  void setupAxisLines();
  void renderAxisLines();
  //FOR VERTEX: render the 3 lines showing the current axis of editing
  void renderEditingAxis();
  // setups common data for all types of render
  void processDrawCall(Render_type type_of_render);
  void vertexDetectionPass();
  void elementDetectionPass();
  void mainRenderPass();

  void initializeBuffers();
  void elementDetectionFramebufferSetup(FramebufferInfo *element_detection_framebuffer_info);
  void mainColorFrameBufferSetup(FramebufferInfo *main_color_framebuffer_info);

  void initializeShaders();
  void resizeFramebuffer();
  void setViewProjectionMatrices();
  void updateModelMatrices(); // updates all given model matrices, for now
  std::pair<int, int> getCursorPositionInViewport(GLFWwindow *window);
  void cleanup();
  bool rendererIsReady(std::string *out_error = nullptr) const;
  std::optional<std::tuple<unsigned int, unsigned int, unsigned int>>
  meshElementDetection();
};

